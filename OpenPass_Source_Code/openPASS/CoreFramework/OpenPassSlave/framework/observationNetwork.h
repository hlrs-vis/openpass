/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  ObservationNetwork.h
//! @brief This file implements the container of all observation modules.
//-----------------------------------------------------------------------------

#pragma once

#include <map>

#include "CoreFramework/CoreShare/log.h"
#include "Interfaces/worldInterface.h"
#include "Interfaces/observationNetworkInterface.h"

namespace SimulationSlave {

class ObservationBinding;
class ObservationModule;
class Stochastics;
class RunResult;

class ObservationNetwork : public ObservationNetworkInterface
{
public:
    ObservationNetwork(ObservationBinding* binding) :
        binding(binding)
    {}
    ObservationNetwork(const ObservationNetwork&) = delete;
    ObservationNetwork(ObservationNetwork&&) = delete;
    ObservationNetwork& operator=(const ObservationNetwork&) = delete;
    ObservationNetwork& operator=(ObservationNetwork&&) = delete;

    virtual ~ObservationNetwork() override;

    virtual bool Instantiate(const std::map<int, ObservationInstance>& observationInstances,
                             StochasticsInterface* stochastics,
                             WorldInterface* world,
                             EventNetworkInterface* eventNetwork) override;
    virtual const std::map<int, ObservationModule*>& GetObservationModules() override;

    virtual bool InitAll(const std::string& path) override;
    virtual bool InitRun() override;
    virtual bool UpdateTimeStep(int time, RunResult& runResult) override;
    virtual bool FinalizeRun(const RunResult& result) override;
    virtual bool FinalizeAll() override;
    virtual void Clear() override;

private:
    ObservationBinding* binding;
    std::map<int, ObservationModule*> modules {};
};

} // namespace SimulationSlave


