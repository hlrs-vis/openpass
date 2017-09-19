/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  world.h
//! @brief This file includes the static and dynamic objects used during a
//!        simulation run.
//-----------------------------------------------------------------------------

#ifndef WORLD_H
#define WORLD_H

#include "worldInterface.h"
#include "worldBinding.h"

namespace SimulationSlave
{

class World : public WorldInterface
{
public:
    World(WorldBinding *worldBinding) :
        worldBinding(worldBinding)
    {}

    World(const World&) = delete;
    World(World&&) = delete;
    World& operator=(const World&) = delete;
    World& operator=(World&&) = delete;

    virtual ~World() = default;

    bool AddAgent(int id, AgentInterface *agent){
        return implementation->AddAgent(id, agent);
    }

    const AgentInterface *GetAgent(int id) const{
        return implementation->GetAgent(id);
    }
    const std::map<int, const AgentInterface*> &GetAgents() const{
        return implementation->GetAgents();
    }

    // framework internal methods to access members without restrictions
    void SetParameter(WorldParameter *worldParameter){
        return implementation->SetParameter(worldParameter);
    }

    void Clear(){
        return implementation->Clear();
    }

    bool CreateGlobalDrivingView(){
        return implementation->CreateGlobalDrivingView();
    }

    // model callbacks
    void SetTimeOfDay(int timeOfDay){
        return implementation->SetTimeOfDay(timeOfDay);
    }

    int GetTimeOfDay() const{
        return implementation->GetTimeOfDay();
    }

    void SetWeekday(Weekday weekday){
        return implementation->SetWeekday(weekday);
    }

    Weekday GetWeekday() const{
        return implementation->GetWeekday();
    }

    void* GetGlobalDrivingView(){
        return implementation->GetGlobalDrivingView();
    }

    void* GetGlobalObjects(){
        return implementation->GetGlobalObjects();
    }

    void QueueAgentUpdate(std::function<void(double)> func,
                              double val){
        return implementation->QueueAgentUpdate(func, val);
    }

    void QueueAgentRemove(const AgentInterface *agent){
        return implementation->QueueAgentRemove(agent);
    }

    void SyncGlobalData(){
        return implementation->SyncGlobalData();
    }

    bool CreateScenery(SceneryInterface &scenery){
        return implementation->CreateScenery(scenery);
    }

    AgentInterface* CreateAgentAdapterForAgent(){
        return implementation->CreateAgentAdapterForAgent();
    }

    const AgentInterface *GetSpecialAgent(){
        return implementation->GetSpecialAgent();
    }

    const AgentInterface *GetLastCarInlane(int laneNumber){
        return implementation->GetLastCarInlane(laneNumber);
    }

    const AgentInterface *GetBicycle() const {
        return implementation->GetBicycle();
    }

    Position GetPositionByDistanceAndLane(double distance, int laneNumber) const{
        return implementation->GetPositionByDistanceAndLane(distance, laneNumber);
    }

    bool CreateWorldScenery(const  std::string &sceneryFilename){
        return implementation->CreateWorldScenery(sceneryFilename);
    }

    //-----------------------------------------------------------------------------
    //! Instantiate the world by creating a WorldInterface out of a world library
    //! with given world parameters
    //! @param[in]  worldParameter  parameters of world inclusive library path
    //!
    //! @return                true if successful
    //-----------------------------------------------------------------------------
    bool Instantiate(WorldParameter *worldParameter)
    {
        if((!worldBinding) || (!worldParameter)){
            return false;
        }else if(!implementation){
            implementation = worldBinding->Instantiate(worldParameter);
            if(!implementation){
                return false;
            }
        }
        return true;
    }

    //-----------------------------------------------------------------------------
    //! Returns true if world was already instantiated
    //!
    //! @return                true if world was already instantiated
    //-----------------------------------------------------------------------------
    bool isInstantiated(){
        return implementation;
    }

private:
    WorldBinding *worldBinding = nullptr;
    WorldInterface *implementation = nullptr;
};

} // namespace SimulationSlave

#endif // WORLD_H
