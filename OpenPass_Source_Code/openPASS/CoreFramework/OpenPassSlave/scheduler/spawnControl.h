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
/** \file  SpawnControl.h
*   \brief SpawnControl generates new agents all component tasks for an given agent
*   \details new agents are parametrizised via BluePrint
*            and new spawning timestamps are stored
*/
//-----------------------------------------------------------------------------

#pragma once

#include "tasks.h"
#include "agentBlueprint.h"
#include "agentParser.h"
#include "spawnPoint.h"
#include "Interfaces/spawnControlInterface.h"
#include <list>

namespace SimulationSlave {

class Agent;
class SpawnPointNetworkInterface;

struct SpawnAgentParameters
{
    SpawnPoint* spawnPoint;
    AgentBlueprintInterface* agentBlueprint;
    int nextSpawnTime;
    int holdbackTime;
};

//-----------------------------------------------------------------------------
/** \brief triggers spawning and generate new agent
*
*   \ingroup OpenPassSlave
*/
//-----------------------------------------------------------------------------
class SpawnControl : public SpawnControlInterface
{
public:
    SpawnControl(SpawnPointNetworkInterface *spawnPointNetwork,
                 WorldInterface *world,
                 const int cycleTime);
    virtual ~SpawnControl();

    /*!
    * \brief Execute
    *
    * \details start function of spawnControl triggers spawning
    *
    *
    * @param[in]     int    timestamp
    */
    bool Execute(int timestamp) override;

    /*!
    * \brief PullNewAgents
    *
    * \details clear list after returning them
    *
    * @return    list of agents   new spawned agents
    */
    std::list<const Agent*> PullNewAgents() override;

    SpawnControlError GetError() const override;


    /*!
    * \brief ReviseVelocity
    *
    * \details Validates if a crash with an potential front agent
    *          on the same lane may be imminent and reduces velocity
    *          of placed vehicle if possible
    *
    * @param[in, out]   agentBlueprint
    * @return true if no crash can be guaranteed within TTB
    */
    bool AdaptVelocityForAgentBlueprint(AgentBlueprintInterface *agentBlueprint);


    /*!
    * \brief Check if new agent will likely collide and calculate needed hold back time
    *
    * \details Validates if a crash with an potential front agent
    *          on the same lane may be imminent and calculates the necessary hold back time (max 5000ms),
    *          so that there won't be a crash. Returns -1 if holding back the agent can not avoid the crash.
    *
    * @param[in, out]   agentBlueprint
    * @return holdBackTime
    */
    int CalculateHoldbackTime(AgentBlueprintInterface *agentBlueprint);

private:

    std::list<SpawnAgentParameters> spawnPoints;
    std::list<const Agent*> newAgents;

    SpawnPointNetworkInterface *spawnPointNetwork = nullptr;
    WorldInterface *world = nullptr;

    SpawnControlError spawnError = {SpawnControlError::NoError};

    const int cycleTime;

    /*!
    * \brief Spawn
    *
    * \details spawns agent, called by Execute()
    *
    * @param[in]     SpawnAgentParameters
    * @param[in]     int   timestamp
    */
    bool Spawn(SpawnAgentParameters* spawnAgentParams, int timestamp);

    /*!
    * \brief GenerateNextBlueprint
    *
    * \details new AgentBlueprint, next spawn time
    *
    * @param[in]     SpawnAgentParameters
    * @param[in]     int   timestamp
    */
    bool GenerateNextBlueprint(SpawnAgentParameters& spawnAgentParams, int timestamp);

    /*!
    * \brief InstantiateAgent
    *
    * \details build new agent
    *
    * @param[in]     SpawnAgentParameters
    * @param[in]     int   timestamp
    * @return        Agent
    */
    bool InstantiateAgent(int timestamp, SpawnAgentParameters& spawnAgentParams);
};

/*!
* \brief isScenarioMember
*
* \details validates if parameters contains an ego or scenario car
*
* @param    spawnAgentParams    parameters defining type of agent
* @return   true if parameters belong to ego or scenario cars
*/
bool isScenarioMember(const SpawnAgentParameters* spawnAgentParams);

/*!
* \brief drivingCorridorDoesNotOverlap
*
* \details validates if the driving corridor overlaps
* this check extends the current bounding box of an ego and
* an opponent by the assumed motion within a given time to brake
* (assuming homogenous motion) and checks for overlap
*
* @param assumedTtb     assumed time to brake
* @param vFront         velocity of front car
* @param aFront         acceleration of front car
* @param fullBrakingDistanceEgo braking distance of ego
* @param ego            blueprint of agent, which shall be set
* @param opponent       manifastation of opponent (already in world)
* @return true if extended bounding boxes do not overlap
*/
bool drivingCorridorDoesNotOverlap(const double assumedTtb,
                             const double vFront,
                             const double aFront,
                             const double fullBrakingDistanceEgo,
                             AgentBlueprintInterface *egoAgentBlueprint,
                             const WorldObjectInterface* opponent);

} //namespace SimulationSlave
