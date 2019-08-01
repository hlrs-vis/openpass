/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <memory>
#include <qglobal.h>
#include "egoSensor_implementation.h"

EgoSensor_Implementation::EgoSensor_Implementation(int componentId,
                                                   bool isInit,
                                                   int priority,
                                                   int offsetTime,
                                                   int responseTime,
                                                   int cycleTime,
                                                   StochasticsInterface *stochastics,
                                                   WorldInterface *world,
                                                   const ParameterInterface *parameters,
                                                   const std::map<int, ObservationInterface *> *evaluations,
                                                   const CallbackInterface *callbacks,
                                                   AgentInterface *agent) :
    SensorInterface(componentId,
                    isInit,
                    priority,
                    offsetTime,
                    responseTime,
                    cycleTime,
                    stochastics,
                    world,
                    parameters,
                    evaluations,
                    callbacks,
                    agent),
    isInitDone(false)
{
    positionX.SetValue(0.0);
    positionY.SetValue(0.0);
    velocityX.SetValue(0.0);
    velocityY.SetValue(0.0);
    accelerationX.SetValue(0.0);
    accelerationY.SetValue(0.0);
    yawAngle.SetValue(0.0);
    maximalXCoordinate.SetValue(0.0);
    minimalXCoordinate.SetValue(0.0);
    maximalYCoordinate.SetValue(0.0);
    minimalYCoordinate.SetValue(0.0);
    width.SetValue(0.0);
    length.SetValue(0.0);
    weight.SetValue(0.0);
    wheelbase.SetValue(0.0);
    trackWidth.SetValue(0.0);
    distanceCOGtoLeadingEdge.SetValue(0.0);
    distanceCOGtoFrontAxle.SetValue(0.0);
    heightCOG.SetValue(0.0);
    momentInertiaYaw.SetValue(0.0);
    momentInertiaPitch.SetValue(0.0);
    momentInertiaRoll.SetValue(0.0);
    frictionCoeff.SetValue(0.0);
}

void EgoSensor_Implementation::UpdateInput(int localLinkId,
                                           const std::shared_ptr<SignalInterface const> &data, int time)
{
    // no input necessary
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);

}

void EgoSensor_Implementation::UpdateOutput(int localLinkId,
                                            std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateOutput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = outputPorts.at(localLinkId)->GetSignalValue(data);

    if (success)
    {
        log << COMPONENTNAME << " UpdateOutput successful";
        LOG(CbkLogLevel::Debug, log.str());
    }
    else
    {
        log << COMPONENTNAME << " UpdateOutput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

bool EgoSensor_Implementation::initAfterInitsAreSet()
{
    width.SetValue(GetAgent()->GetWidth());
    length.SetValue(GetAgent()->GetLength());
    weight.SetValue(GetAgent()->GetWeight());
    wheelbase.SetValue(GetAgent()->GetWheelbase());
    trackWidth.SetValue(GetAgent()->GetTrackWidth());
    distanceCOGtoLeadingEdge.SetValue(GetAgent()->GetDistanceCOGtoLeadingEdge());
    distanceCOGtoFrontAxle.SetValue(GetAgent()->GetDistanceCOGtoFrontAxle());
    heightCOG.SetValue(GetAgent()->GetHeightCOG());
    momentInertiaYaw.SetValue(GetAgent()->GetMomentInertiaYaw());
    momentInertiaPitch.SetValue(GetAgent()->GetMomentInertiaPitch());
    momentInertiaRoll.SetValue(GetAgent()->GetMomentInertiaRoll());
    frictionCoeff.SetValue(GetAgent()->GetFrictionCoeff());

    return true;
}

void EgoSensor_Implementation::Trigger(int time)
{
    Q_UNUSED(time);

    if (!isInitDone)
    {
        bool initSuccessful = initAfterInitsAreSet();
        if (!initSuccessful)
        {
            return;
        }
        isInitDone = true;
    }
    double in_positionX = GetAgent()->GetPositionX();
    positionX.SetValue(in_positionX);
    double in_positionY = GetAgent()->GetPositionY();
    positionY.SetValue(in_positionY);
    velocityX.SetValue(GetAgent()->GetVelocityX());
    velocityY.SetValue(GetAgent()->GetVelocityY());
    velocityAbsolute.SetValue(GetAgent()->GetVelocityAbsolute());
    accelerationX.SetValue(GetAgent()->GetAccelerationX());
    accelerationY.SetValue(GetAgent()->GetAccelerationY());
    accelerationAbsolute.SetValue(GetAgent()->GetAccelerationAbsolute());
    double in_yawAngle = GetAgent()->GetYawAngle();
    yawAngle.SetValue(in_yawAngle);

    double sinPsi = sin(in_yawAngle);
    double cosPsi = cos(in_yawAngle);

    double in_length = GetAgent()->GetLength();
    double in_width = GetAgent()->GetWidth();
    double dist2Front = (GetAgent()->GetVehicleType() == AgentVehicleType::Pedestrian) ? in_length / 2 :
                        GetAgent()->GetDistanceCOGtoFrontAxle();

    maximalXCoordinate.SetValue(in_positionX + in_width / 2 * fabs( sinPsi ) + ( (
                                                                                     cosPsi > 0) ? dist2Front *cosPsi : (dist2Front - in_length) * cosPsi ));
    minimalXCoordinate.SetValue(in_positionX - in_width / 2 * fabs( sinPsi ) - ( (cosPsi > 0) ?
                                                                                 (in_length - dist2Front) * cosPsi : dist2Front * (-cosPsi) ));
    maximalYCoordinate.SetValue(in_positionY + in_width / 2 * fabs( cosPsi ) + ( (
                                                                                     sinPsi > 0) ? dist2Front *sinPsi : (dist2Front - in_length) * sinPsi ));
    minimalYCoordinate.SetValue(in_positionY - in_width / 2 * fabs( cosPsi ) - ( (sinPsi > 0) ?
                                                                                 (in_length - dist2Front) * sinPsi : dist2Front * (-sinPsi) ));

}
