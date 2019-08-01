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
#include "algorithm_selector_implementation.h"

Algorithm_Selector_Implementation::Algorithm_Selector_Implementation(int componentId,
                                                                     bool isInit,
                                                                     int priority,
                                                                     int offsetTime,
                                                                     int responseTime,
                                                                     int cycleTime,
                                                                     StochasticsInterface *stochastics,
                                                                     const ParameterInterface *parameters, const std::map<int, ObservationInterface *> *observations,
                                                                     const CallbackInterface *callbacks,
                                                                     int agentId) :
    AlgorithmInterface(componentId,
                       isInit,
                       priority,
                       offsetTime,
                       responseTime,
                       cycleTime,
                       stochastics,
                       parameters,
                       observations,
                       callbacks,
                       agentId)
{
    std::stringstream log;
    log << "Constructing Algorithm_Selector";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    driverThrottle.SetDefaultValue(0.0);
    driverBrake.SetDefaultValue(0.0);
    driverSteering.SetDefaultValue(0.0);

    brakeAssistActive.SetDefaultValue(false);
    brakeAssistBrake.SetDefaultValue(0.0);

    laneAssistActive.SetDefaultValue(false);
    laneAssistBrakeSuperpose.SetDefaultValue(std::vector<double> {0.0, 0.0, 0.0, 0.0});

    evasiveSteeringtActive.SetDefaultValue(false);
    evasiveSteeringAngle.SetDefaultValue(0.0);

    collisionOccured.SetDefaultValue(false);

    log << "Construction of Algorithm_Selector successful";
    LOG(CbkLogLevel::Debug, log.str());
}

Algorithm_Selector_Implementation::~Algorithm_Selector_Implementation()
{

}

void Algorithm_Selector_Implementation::UpdateInput(int localLinkId,
                                                    const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateInput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = inputPorts.at(localLinkId)->SetSignalValue(data);

    if (success)
    {
        log << COMPONENTNAME << " UpdateInput successful";
        LOG(CbkLogLevel::Debug, log.str());
    }
    else
    {
        log << COMPONENTNAME << " UpdateInput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

void Algorithm_Selector_Implementation::UpdateOutput(int localLinkId,
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

void Algorithm_Selector_Implementation::Trigger(int time)
{
    Q_UNUSED(time);

    selector.Perform(driverThrottle.GetValue(),
                     driverBrake.GetValue(),
                     driverSteering.GetValue(),
                     brakeAssistActive.GetValue(),
                     brakeAssistBrake.GetValue(),
                     laneAssistActive.GetValue(),
                     laneAssistBrakeSuperpose.GetValue(),
                     evasiveSteeringtActive.GetValue(),
                     evasiveSteeringAngle.GetValue(),
                     collisionOccured.GetValue());

    resultingThrottle.SetValue(selector.GetResultingThrottle());
    resultingBrake.SetValue(selector.GetResultingBrake());
    resultingSteering.SetValue(selector.GetResultingSteering());
    resultingBrakeSuperpose.SetValue(selector.GetResultingBrakeSuperpose());

    return;
}
