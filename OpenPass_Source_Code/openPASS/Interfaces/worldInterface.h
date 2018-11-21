/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  worldInterface.h
//! @brief This file provides the interface of the scenery and dynamic objects
//!        to custom libraries.
//-----------------------------------------------------------------------------

#ifndef WORLDINTERFACE_H
#define WORLDINTERFACE_H

#include <map>
#include <functional>
#include <list>
#include "agentInterface.h"
#include "globalDefinitions.h"
#include "sceneryInterface.h"
#include "callbackInterface.h"

//-----------------------------------------------------------------------------
//! Provides access to world representation
//-----------------------------------------------------------------------------
class WorldInterface
{
public:
    WorldInterface() = default;
    WorldInterface(const WorldInterface &) = delete;
    WorldInterface(WorldInterface &&) = delete;
    WorldInterface &operator=(const WorldInterface &) = delete;
    WorldInterface &operator=(WorldInterface &&) = delete;
    virtual ~WorldInterface() = default;

    //-----------------------------------------------------------------------------
    //! Retrieves global view on complete world (from which a sensor can retrieve
    //! arbitrary information)
    //!
    //! @return                global view
    //-----------------------------------------------------------------------------
    virtual void *GetGlobalDrivingView() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves static objects (roads and junctions)
    //!
    //! @return                reference to scenery
    //-----------------------------------------------------------------------------
    virtual void *GetGlobalObjects() = 0;

    //-----------------------------------------------------------------------------
    //! Sets time of day (hour) for the simulation run
    //!
    //! @return                time of day [0-23]
    //-----------------------------------------------------------------------------
    virtual void SetTimeOfDay(int timeOfDay) = 0;

    //-----------------------------------------------------------------------------
    //! Sets weekday for the simulation run
    //!
    //! @return                weekday
    //-----------------------------------------------------------------------------
    virtual void SetWeekday(Weekday weekday) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the OSI world data
    //!
    //! @return                Pointer OSI world data structure
    //-----------------------------------------------------------------------------
    virtual void* GetWorldData() = 0;
    
    //-----------------------------------------------------------------------------
    //! Retrieves time of day (hour)
    //!
    //! @return                time of day [0, 23]
    //-----------------------------------------------------------------------------
    virtual int GetTimeOfDay() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves weekday
    //!
    //! @return                weekday
    //-----------------------------------------------------------------------------
    virtual Weekday GetWeekday() const = 0;

    //-----------------------------------------------------------------------------
    //! Sets the world parameters like weekday, library
    //! @param[in]     worldParamter  parameter to setup world
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetParameter(WorldParameter *worldParameter) = 0;

    //-----------------------------------------------------------------------------
    //! Clear the world, reset internal states
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void Clear() = 0;

    //-----------------------------------------------------------------------------
    //! Create internal global driving view
    //!
    //! @return                weekday
    //-----------------------------------------------------------------------------
    virtual bool CreateGlobalDrivingView() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves specific agent
    //! @param[in]  id         id of agent
    //!
    //! @return                Agent reference
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetAgent(int id) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves all agents
    //!
    //! @return                Mapping of ids to agents
    //-----------------------------------------------------------------------------
    virtual const std::map<int, const AgentInterface *> &GetAgents() const = 0;

    //-----------------------------------------------------------------------------
    //! Add agent to world
    //! @param[in]  id         id of agent
    //! @param[in]  agent      agent reference
    //!
    //! @return                true if successful
    //-----------------------------------------------------------------------------
    virtual bool AddAgent(int id, AgentInterface *agent) = 0;

    //-----------------------------------------------------------------------------
    //! queue functions and values to update agent when SyncGlobalData is called
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void QueueAgentUpdate(std::function<void(double)> func,
                                  double val) = 0;

    //-----------------------------------------------------------------------------
    //! Add agent to list to be removed when SyncGlobalData is called
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void QueueAgentRemove(const AgentInterface *agent) = 0;

    //-----------------------------------------------------------------------------
    //! Update all agents / Remove agents and sync data of all agents within world
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SyncGlobalData() = 0;

    //-----------------------------------------------------------------------------
    //! Create a scenery in world.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool CreateScenery(SceneryInterface &scenery) = 0;

    //-----------------------------------------------------------------------------
    //! Create an agentAdapter for an agent to communicate between the agent of the
    //! framework and the world.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual AgentInterface *CreateAgentAdapterForAgent() = 0;

    //-----------------------------------------------------------------------------
    //! Returns one agent which is set to be special.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetSpecialAgent() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the last car in lane. Return nullptr if no car is found.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetLastCarInlane(int laneNumber) = 0;

    //-----------------------------------------------------------------------------
    //! Returns an agentInterface which is a bicycle. Returns nullptr if no bicyle
    //! found.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetBicycle() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the postion by the distance and the lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual Position GetPositionByDistanceAndLane(double distance,
                                                  int laneNumber) const = 0;

    //-----------------------------------------------------------------------------
    //! Tries to create an internal scenery from a given file.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool CreateWorldScenery(const  std::string &sceneryFilename) = 0;

    //-----------------------------------------------------------------------------
    //! Tries to create an internal scenario from a given file.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool CreateWorldScenario(const  std::string &scenarioFilename) = 0;
};

#endif // WORLDINTERFACE_H
