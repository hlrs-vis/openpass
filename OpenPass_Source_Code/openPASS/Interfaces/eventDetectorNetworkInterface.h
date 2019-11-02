/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "Interfaces/eventNetworkInterface.h"
#include "Interfaces/scenarioInterface.h"

namespace SimulationSlave
{

//Forward declarations
class EventDetector;

class EventDetectorNetworkInterface
{
public:
    EventDetectorNetworkInterface() = default;
    EventDetectorNetworkInterface(const EventDetectorNetworkInterface&) = delete;
    EventDetectorNetworkInterface(EventDetectorNetworkInterface&&) = delete;
    EventDetectorNetworkInterface& operator=(const EventDetectorNetworkInterface&) = delete;
    EventDetectorNetworkInterface& operator=(EventDetectorNetworkInterface&&) = delete;
    ~EventDetectorNetworkInterface() = default;

    virtual bool Instantiate(const std::string libraryPath,
                     ScenarioInterface *scenario,
                     EventNetworkInterface* eventNetwork,
                     StochasticsInterface *stochastics) = 0;

    virtual void Clear() = 0;

    virtual std::vector<const EventDetector*> GetEventDetectors() = 0;

    virtual void ResetAll() = 0;
};

}// namespace SimulationSlave


