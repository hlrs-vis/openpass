/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/*!
* \file  ObservationLogImplementation.h
* \brief Adds the RunStatistic information to the simulation output.
* \details  Writes the RunStatistic information into the simulation output.
*           Also manages the stop reasons of the simulation.
*/
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <tuple>
#include <QFile>
#include <QTextStream>
#include "runStatistic.h"
#include "observationCyclics.h"
#include "observationFileHandler.h"

//-----------------------------------------------------------------------------
/** \brief This class adds the RunStatistic information to the simulation output.
*   \details This class inherits the ObservationLogGeneric which creates the basic simulation output
*            and adds the RunStatistic information to the output.
*            This class also manages the stop reasons of the simulation.
*
*   \ingroup ObservationLog
*/
//-----------------------------------------------------------------------------
class ObservationLogImplementation : ObservationInterface
{
public:
    const std::string COMPONENTNAME = "ObservationLog";

    ObservationLogImplementation(SimulationSlave::EventNetworkInterface* eventNetwork,
                                 StochasticsInterface* stochastics,
                                 WorldInterface* world,
                                 const ParameterInterface* parameters,
                                 const CallbackInterface* callbacks);
    ObservationLogImplementation(const ObservationLogImplementation&) = delete;
    ObservationLogImplementation(ObservationLogImplementation&&) = delete;
    ObservationLogImplementation& operator=(const ObservationLogImplementation&) = delete;
    ObservationLogImplementation& operator=(ObservationLogImplementation&&) = delete;
    virtual ~ObservationLogImplementation() override = default;

    virtual void Insert(int time, int agentId, LoggingGroup group, const std::string& key, const std::string& value) override;
    virtual void InsertEvent(std::shared_ptr<EventInterface> event) override;
    virtual void SlavePreHook(const std::string& path) override;
    virtual void SlavePreRunHook() override;
    virtual void SlavePostRunHook(const RunResultInterface& runResult) override;
    virtual void SlaveUpdateHook(int, RunResultInterface&) override {}
    virtual void MasterPreHook() override {}
    virtual void MasterPostHook(const std::string&) override {}
    virtual void SlavePostHook() override;

    //-----------------------------------------------------------------------------
    /*!
    * \brief Calculates the ego followers at simulation start
    */
    //-----------------------------------------------------------------------------
    virtual void GatherFollowers() override;

    //-----------------------------------------------------------------------------
    /*!
    * \brief Insert the id of the agent into the list of followers of it is behind the ego
    */
    //-----------------------------------------------------------------------------
    virtual void InformObserverOnSpawn(AgentInterface* agent) override;

    virtual const std::string SlaveResultFile() override
    {
        return "";
    }

private:
    RunStatistic runStatistic = RunStatistic(-1);
    std::vector<LoggingGroup> loggingGroups{LoggingGroup::Trace};
    ObservationCyclics cyclics;
    ObservationFileHandler fileHandler;
    SimulationSlave::EventNetworkInterface* eventNetwork;
};


