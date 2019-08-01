/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \brief ObjectDetectorBase.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include <cassert>
#include "objectDetectorBase.h"

ObjectDetectorBase::ObjectDetectorBase(
    std::string componentName,
    bool isInit,
    int priority,
    int offsetTime,
    int responseTime,
    int cycleTime,
    StochasticsInterface* stochastics,
    WorldInterface* world,
    const ParameterInterface* parameters,
    const std::map<int, ObservationInterface*>* observations,
    const CallbackInterface* callbacks,
    AgentInterface* agent) :
    SensorInterface(
        componentName,
        isInit,
        priority,
        offsetTime,
        responseTime,
        cycleTime,
        stochastics,
        world,
        parameters,
        observations,
        callbacks,
        agent)
{
    try
    {
        ParseBasicParameter();
    }
    catch (const std::out_of_range& e)
    {
        std::string msg{"Unable to retrieve sensor parameter: "};
        msg += e.what();
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }

    try
    {
        _observer = GetObservations()->at(0);
        if (_observer == nullptr) { throw std::runtime_error(""); }
    }
    catch (...)
    {
        const std::string msg = COMPONENTNAME + " invalid observation module setup";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}

void ObjectDetectorBase::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const>& data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " (component " << GetComponentName() << ", agent " << GetAgent()->GetId() <<
        ", output data for local link " << localLinkId << ": ";
    LOG(CbkLogLevel::Debug, log.str());

    if (localLinkId == 0)
    {
        // to SensorFusion
        try
        {
            data = std::make_shared<SensorDataSignal const>(
                       sensorData);
        }
        catch (const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

Position ObjectDetectorBase::GetAbsolutePosition()
{
    Position absolutePosition;

    double posX = GetAgent()->GetPositionX();
    double posY = GetAgent()->GetPositionY();
    double yaw = GetAgent()->GetYaw();

    absolutePosition.xPos = posX + position.longitudinal * std::cos(yaw) - position.lateral * std::sin(yaw);
    absolutePosition.yPos = posY + position.longitudinal * std::sin(yaw) + position.lateral * std::cos(yaw);
    absolutePosition.yawAngle = yaw;

    return absolutePosition;
}

osi3::SensorData ObjectDetectorBase::ApplyLatency(int time, osi3::SensorData currentSensorData)
{
    detectedObjectsBuffer.push_back({time + latencyInMs, currentSensorData});

    osi3::SensorData detectedObjectsByLatency;

    while (!detectedObjectsBuffer.empty() && time >= detectedObjectsBuffer.front().first)
    {
        detectedObjectsByLatency = detectedObjectsBuffer.front().second;
        detectedObjectsBuffer.pop_front();
    }

    return detectedObjectsByLatency;
}

bool ObjectDetectorBase::HasDetectionError()
{
    if (failureProbability == 0.0)
    {
        return false;
    }
    return GetStochastics()->GetUniformDistributed(0, 1) < failureProbability;
}

double ObjectDetectorBase::ApplyMeasurementError(double value)
{
    if (failureProbability == 0.0)
    {
        return value;
    }
    return value * GetStochastics()->GetLogNormalDistributed(1.0, failureProbability);
}

osi3::SensorViewConfiguration ObjectDetectorBase::GenerateSensorViewConfiguration()
{
    osi3::SensorViewConfiguration viewConfiguration;

    viewConfiguration.mutable_sensor_id()->set_value(static_cast<unsigned int>(id));

    viewConfiguration.mutable_mounting_position()->mutable_orientation()->set_pitch(position.pitch);
    viewConfiguration.mutable_mounting_position()->mutable_orientation()->set_roll(position.roll);
    viewConfiguration.mutable_mounting_position()->mutable_orientation()->set_yaw(position.yaw);

    viewConfiguration.mutable_mounting_position()->mutable_position()->set_x(position.longitudinal);
    viewConfiguration.mutable_mounting_position()->mutable_position()->set_y(position.lateral);
    viewConfiguration.mutable_mounting_position()->mutable_position()->set_z(position.height);

    return viewConfiguration;
}

polygon_t ObjectDetectorBase::CalculateBoundingBox(const osi3::Dimension3d dimension, const  osi3::Vector3d position, const osi3::Orientation3d orientation)
{
    double halfLength = dimension.length() / 2.0;
    double halfWidth = dimension.width() / 2.0;
    double rotation = orientation.yaw();

    double x = position.x();
    double y = position.y();


    point_t boxPoints[]
    {
        {-halfLength, -halfWidth},
        {-halfLength,  halfWidth},
        {halfLength,  halfWidth},
        {halfLength, -halfWidth},
        {-halfLength, -halfWidth}
    };

    polygon_t box;
    polygon_t boxTemp;
    bg::append(box, boxPoints);

    bt::translate_transformer<double, 2, 2> translate(x, y);

    // rotation in mathematical negativ order (boost) -> invert to match
    bt::rotate_transformer<bg::radian, double, 2, 2> rotate(-rotation);

    bg::transform(box, boxTemp, rotate);
    bg::transform(boxTemp, box, translate);
    bg::correct(box);

    return box;
}

point_t ObjectDetectorBase::TransformPointToLocalCoordinates(point_t point, point_t sensorPositionGlobal, double yaw)
{
    point_t tempPoint;
    point_t pointLocal;
    bt::translate_transformer<double, 2, 2> sensorPositionTranslate(-sensorPositionGlobal.x(), -sensorPositionGlobal.y());
    bg::transform(point, tempPoint, sensorPositionTranslate);
    bt::rotate_transformer<bg::radian, double, 2, 2> sensorRotation(position.yaw + yaw);
    bg::transform(tempPoint, pointLocal, sensorRotation);

    return pointLocal;
}

polygon_t ObjectDetectorBase::TransformPolygonToGlobalCoordinates(polygon_t polygon, point_t sensorPositionGlobal,
        double yaw)
{
    polygon_t tempBox;
    polygon_t polygonGlobal;
    bt::rotate_transformer<bg::radian, double, 2, 2> sensorRotation(-position.yaw - yaw);
    bg::transform(polygon, tempBox, sensorRotation);
    bt::translate_transformer<double, 2, 2> sensorPositionTranslate(sensorPositionGlobal.x(), sensorPositionGlobal.y());
    bg::transform(tempBox, polygonGlobal, sensorPositionTranslate);

    return polygonGlobal;
}

point_t ObjectDetectorBase::CalculateGlobalSensorPosition(point_t vehiclePosition, double yaw)
{
    point_t sensorPositionVehicle{position.longitudinal, position.lateral};
    point_t sensorPositionGlobal;
    point_t tempPoint;
    bt::rotate_transformer<bg::radian, double, 2, 2> vehicleRotation(-yaw);
    bg::transform(sensorPositionVehicle, tempPoint, vehicleRotation);
    bt::translate_transformer<double, 2, 2> vehicleTranslate(vehiclePosition.x(), vehiclePosition.y());
    bg::transform(tempPoint, sensorPositionGlobal, vehicleTranslate);

    return sensorPositionGlobal;
}

point_t ObjectDetectorBase::CalculateRelativeVector(const point_t absolute, const point_t own, double yaw)
{
    point_t difference{absolute};
    bg::subtract_point(difference, own);
    point_t result;
    bt::rotate_transformer<bg::radian, double, 2, 2> sensorRotation(position.yaw + yaw);
    bg::transform(difference, result, sensorRotation);
    return result;
}

void ObjectDetectorBase::ParseBasicParameter()
{
    const auto& parameters = GetParameters();
    const auto& doubleParameters = parameters->GetParametersDouble();

    id = parameters->GetParametersInt().at("Id");
    failureProbability = doubleParameters.at("FailureProbability") < MIN_FAILURE_PROBABILITY ?
                         0.0 : doubleParameters.at("FailureProbability");
    latency = doubleParameters.at("Latency");
    latencyInMs = static_cast<int>(latency * 1000.0);

    position.longitudinal = doubleParameters.at("Longitudinal");
    position.lateral = doubleParameters.at("Lateral");
    position.height = doubleParameters.at("Height");
    position.pitch = doubleParameters.at("Pitch");
    position.yaw = doubleParameters.at("Yaw");
    position.roll = doubleParameters.at("Roll");
}
