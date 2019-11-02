/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
/**********************************************
***********************************************/
#include <QtGlobal>

#include "AlgorithmAgentFollowingDriverModelImplementation.h"
#include "Common/lateralSignal.h"
#include "Common/secondaryDriverTasksSignal.h"
#include "Common/accelerationSignal.h"

AlgorithmAgentFollowingDriverModelImplementation::AlgorithmAgentFollowingDriverModelImplementation(
        std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        WorldInterface *world,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface*> *observations,
        const CallbackInterface *callbacks,
        AgentInterface *agent) :
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
    if (parameters->GetParametersDouble().count("VelocityWish") > 0)
    {
        vWish = parameters->GetParametersDouble().at("VelocityWish");
    }
    if (parameters->GetParametersDouble().count("Delta") > 0)
    {
        delta = parameters->GetParametersDouble().at("Delta");
    }
    if (parameters->GetParametersDouble().count("TGapWish") > 0)
    {
        tGapWish = parameters->GetParametersDouble().at("TGapWish");
    }
    if (parameters->GetParametersDouble().count("MinDistance") > 0)
    {
        minDistance = parameters->GetParametersDouble().at("MinDistance");
    }
    if (parameters->GetParametersDouble().count("MaxAcceleration") > 0)
    {
        maxAcceleration = parameters->GetParametersDouble().at("MaxAcceleration");
    }
    if (parameters->GetParametersDouble().count("MaxDeceleration") > 0)
    {
        decelerationWish = parameters->GetParametersDouble().at("MaxDeceleration");
    }
}

AlgorithmAgentFollowingDriverModelImplementation::~AlgorithmAgentFollowingDriverModelImplementation()
{}

void AlgorithmAgentFollowingDriverModelImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);
    if (localLinkId == 0)
    {
        // from SensorDriver
        const std::shared_ptr<SensorDriverSignal const> signal = std::dynamic_pointer_cast<SensorDriverSignal const>(data);
        if(!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        ownVehicleInformation = signal->GetOwnVehicleInformation();
        geometryInformation = signal->GetGeometryInformation();
        surroundingObjects = signal->GetSurroundingObjects();
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmAgentFollowingDriverModelImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try
        {
            data = std::make_shared<LateralSignal const>(
                        out_laneWidth,
                        out_lateral_displacement, // lateral deviation
                        out_lateral_gain_displacement,
                        out_lateral_heading_error,
                        out_lateral_gain_heading_error,
                        out_curvature,
                        componentState);
        }
        catch(const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    else if(localLinkId == 1)
    {
        try
        {
            data = std::make_shared<SecondaryDriverTasksSignal const>(
                                        out_indicatorState,
                                        out_hornSwitch,
                                        out_headLight,
                                        out_highBeamLight,
                                        out_flasher,
                                        componentState);
        }
        catch(const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    else if(localLinkId == 2)
    {
        try
        {
            data = std::make_shared<AccelerationSignal const>(componentState, out_longitudinal_acc);
        }
        catch(const std::bad_alloc&)
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

void AlgorithmAgentFollowingDriverModelImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    out_longitudinal_speed = ownVehicleInformation.velocity;
    out_curvature = geometryInformation.laneEgo.curvature;
    out_laneWidth = geometryInformation.laneEgo.width;
    out_lateral_speed = 0;
    out_lateral_frequency = std::sqrt(lateralDynamicConstants.lateralAcceleration / geometryInformation.laneEgo.width);
    out_lateral_damping = lateralDynamicConstants.zeta;
    out_lateral_displacement = -ownVehicleInformation.lateralPosition;
    out_lateral_heading_error = -ownVehicleInformation.heading;
    out_lateral_gain_heading_error = lateralDynamicConstants.gainHeadingError;

    const auto& frontAgent = surroundingObjects.objectFront;
    double decelerationCoeff = 0.0;

    if (frontAgent.exist)
    {
        auto vDelta = std::abs(ownVehicleInformation.velocity - frontAgent.velocity);
        auto effectiveMinimumGap = minDistance + ownVehicleInformation.velocity*tGapWish + (ownVehicleInformation.velocity*vDelta)/2 * std::sqrt(maxAcceleration * decelerationWish);
        decelerationCoeff = std::pow(effectiveMinimumGap/frontAgent.relativeLongitudinalDistance, 2);
    }

    auto freeRoadCoeff = 1.0 - std::pow(ownVehicleInformation.velocity/vWish, delta);
    auto intelligentDriverModelAcceleration = maxAcceleration * (freeRoadCoeff - decelerationCoeff);

    if(intelligentDriverModelAcceleration >= 0)
    {
        out_longitudinal_acc = std::min(maxAcceleration, intelligentDriverModelAcceleration);
    }
    else
    {
        auto maxDeceleration = -decelerationWish;
        out_longitudinal_acc = std::max(maxDeceleration, intelligentDriverModelAcceleration);
    }
}

