/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/** \addtogroup EventDetector
* @{
* \brief This file contains the basic EventDetector functionality.
*
* \details This file contains the basic EventDetector functionality.
* An EventDetector acts as Trigger for specific events. Those events are then forwarded
* to the EventNetwork.
* The EventDetector uses its access to the world and the framework to find events.
* Each EventDetector can have its own cycle time.
*
* \section MODULENAME_ExternalParameters External parameters
* name | meaning
* -----|------
* triggeringAgents  | Determines what agents can trigger the EventDetector.
* sequenceName      | Name of the sequence the EventDetector belongs to.
* agentGroupName    | In case there are no triggeringAgents an EventDetector can trigger to a ceratin agent group.
*
*
* @} */

#pragma once

#include <unordered_map>

#include "Interfaces/parameterInterface.h"
#include "Interfaces/worldInterface.h"
#include "Interfaces/eventDetectorInterface.h"

#include "Common/agentBasedEvent.h"
#include "Common/collisionEvent.h"

#define SCENARIO_AGENT "ScenarioAgent"

const std::unordered_map<std::string, AgentCategory> agentCategoryMap = {{"Ego", AgentCategory::Ego},
                                                                         {"Scenario", AgentCategory::Scenario},
                                                                         {"Common", AgentCategory::Common},
                                                                         {"Any", AgentCategory::Any}};

//-----------------------------------------------------------------------------
/** \brief This is the parent class for all EventDetectors providing the basic functionality.
*
* 	\ingroup EventDetector */
//-----------------------------------------------------------------------------
class EventDetectorCommonBase : public EventDetectorInterface
{
public:
    EventDetectorCommonBase(WorldInterface *world,
                            ParameterInterface* parameters,
                            SimulationSlave::EventNetworkInterface* eventNetwork,
                            const CallbackInterface *callbacks,
                            StochasticsInterface *stochastics);
    EventDetectorCommonBase(const EventDetectorCommonBase&) = delete;
    EventDetectorCommonBase(EventDetectorCommonBase&&) = delete;
    EventDetectorCommonBase& operator=(const EventDetectorCommonBase&) = delete;
    EventDetectorCommonBase& operator=(EventDetectorCommonBase&&) = delete;
    ~EventDetectorCommonBase();

    /*!
    * \brief Triggers the functionality of this class
    *
    * \details Trigger gets called each cycle timestep.
    * This function is repsonsible for creating events
    *
    * @param[in]     time    Current time.
    */
    virtual void Trigger(int time) = 0;

    /*!
    * \brief Returns the cycle time.
    *
    * @return     cycle time.
    */
    virtual int GetCycleTime();

    /*!
    * \brief Resets the initial state of the EventDetector.
    *
    */
    virtual void Reset();

protected:
    /*!
     * \brief Log
     * Provides callback to LOG() macro
     *
     * \param[in] logLevel          Importance of log
     * \param[in] file              Name of file where log is called
     * \param[in] line              Line within file where log is called
     * \param[in] message           Message to log
     */
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message);

    /*!
     * \brief Returns all agents that can trigger the EventDetector
     * \details     First looks wether specific agents names were set as actors.
     *              If not returns the agent group which trigger the EventDetector.
     *
     * \return      Agents which trigger the EventDetector.
     */
    std::list<AgentInterface*> GetTriggeringScenarioAgents();
    std::list<AgentInterface*> GetActingScenarioAgents();


    EventDefinitions::EventType eventType;

    WorldInterface *world {nullptr};
    SimulationSlave::EventNetworkInterface* eventNetwork {nullptr};
    const CallbackInterface *callbacks {nullptr};
    ParameterInterface* parameters {nullptr};
    StochasticsInterface* stochastics {nullptr};

    int cycleTime = 1000;
    std::vector<std::string> triggeringAgents;
    std::vector<std::string> actingAgents;
    std::string sequenceName {""};
    AgentCategory agentCategory = AgentCategory::Any;

    const std::string COMPONENTNAME {"EventDetector"};
};


