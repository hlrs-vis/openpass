/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <QtGlobal>

#include "Algorithm_agentFollowingDriverModel_implementation.h"
#include "lateralDriverSignal.h"
#include "secondaryDriverTasksSignal.h"
#include "brakingSignal.h"

AlgorithmAgentFollowingDriverModelImplementation::AlgorithmAgentFollowingDriverModelImplementation(
        int componentId,
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
        componentId,
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
}

AlgorithmAgentFollowingDriverModelImplementation::~AlgorithmAgentFollowingDriverModelImplementation()
{}

void AlgorithmAgentFollowingDriverModelImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void AlgorithmAgentFollowingDriverModelImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try
        {
            data = std::make_shared<LateralDriverSignal const>(
                        out_curvature,
                        out_laneWidth,
                        out_lateral_speed,
                        out_longitudinal_speed,
                        out_notifyCollision,
                        out_lateral_frequency,
                        out_lateral_damping,
                        out_lateral_displacement,
                        out_lateral_heading_error,
                        out_lateral_gain_heading_error,
                        out_isLaneChangeAborted,
                        componentState,
                        GetComponentId());
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
                                                      GetComponentId(),
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
            data = std::make_shared<BrakingSignal const>(GetComponentId(), componentState, out_longitudinal_acc);
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

    auto vEgo = GetAgent()->GetVelocityX();
    auto laneWidth = GetAgent()->GetLaneWidth();
    auto roadPosition = GetAgent()->GetRoadPosition();

    out_longitudinal_speed = vEgo;
    out_notifyCollision = GetAgent()->GetCollisionPartners().size() > 0;
    out_curvature = GetAgent()->GetCurvatureInDistance(0);
    out_laneWidth = laneWidth;
    out_lateral_speed = 0;
    out_lateral_frequency = std::sqrt(lateralDynamicConstants.lateralAcceleration / laneWidth);
    out_lateral_damping = lateralDynamicConstants.zeta;
    out_lateral_displacement = -roadPosition.t;
    out_lateral_heading_error = -roadPosition.hdg;
    out_lateral_gain_heading_error = lateralDynamicConstants.gainHeadingError;
    out_isLaneChangeAborted = false;

    const AgentInterface* frontAgent = GetAgent()->GetAgentInFront(GetAgent()->GetAgentLaneId());
    double deceleration = 0;

    if (frontAgent)
    {
        auto vFront = frontAgent->GetVelocityX();
        auto currentGap = frontAgent->GetRoadPosition().s - GetAgent()->GetRoadPosition().s;
        auto vDelta = std::abs(vEgo - vFront);
        auto effectiveMinimumGap = minDistance + vEgo*tGapWish + (vEgo*vDelta)/2 * std::sqrt(maxAcceleration * decelerationWish);
        deceleration = std::pow(effectiveMinimumGap/currentGap, 2);
    }

    auto freeRoadAcceleration = 1.0 - std::pow(vEgo/vWish, delta);
    auto intelligentDriverModelAcceleration = freeRoadAcceleration - deceleration;

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
