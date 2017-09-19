/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  runConfig.h
//! @brief This file contains the internal representation of the simulation run
//!        configuration.
//-----------------------------------------------------------------------------

#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <string>
#include <list>
#include <map>
#include "globalDefinitions.h"
#include "parameters.h"

namespace SimulationCommon
{

class RunConfig
{
public:
    //-----------------------------------------------------------------------------
    //! Instance of an observation module as specified in the runConfiguration.xml
    //! with its fileds ID, libraryName and parameters
    //-----------------------------------------------------------------------------
    class ObservationInstance
    {
    public:
        ObservationInstance(int id, const std::string &libraryName) : id(id), libraryName(libraryName)
        {}
        ObservationInstance(const ObservationInstance&) = delete;
        ObservationInstance(ObservationInstance&&) = delete;
        ObservationInstance& operator=(const ObservationInstance&) = delete;
        ObservationInstance& operator=(ObservationInstance&&) = delete;
        virtual ~ObservationInstance() = default;

        int GetId() const
        {
            return id;
        }

        const std::string &GetLibraryName() const
        {
            return libraryName;
        }

        ObservationParameters &GetObservationParameters()
        {
            return parameters;
        }

    private:
        int id;
        const std::string libraryName;
        ObservationParameters parameters;
    };

    class SpawnPointInstance
    {
    public:
        SpawnPointInstance(int id, const std::string &libraryName) :
            id(id),
            libraryName(libraryName)
        {}
        SpawnPointInstance(const SpawnPointInstance&) = delete;
        SpawnPointInstance(SpawnPointInstance&&) = delete;
        SpawnPointInstance& operator=(const SpawnPointInstance&) = delete;
        SpawnPointInstance& operator=(SpawnPointInstance&&) = delete;
        virtual ~SpawnPointInstance() = default;

        int GetId() const
        {
            return id;
        }

        const std::string &GetLibraryName() const
        {
            return libraryName;
        }

        const std::list<int> &GetAgentRefs() const
        {
            return agentRefs;
        }

        SpawnPointParameters &GetSpawnPointParameters()
        {
            return parameters;
        }

        void AddAgentRef(int ref)
        {
            agentRefs.push_back(ref);
        }

    private:
        int id;
        const std::string libraryName;
        std::list<int> agentRefs;
        SpawnPointParameters parameters;
    };



    //-----------------------------------------------------------------------------
    //! Instance of an stochastics module as specified in the runConfiguration.xml
    //! with its fileds libraryName
    //-----------------------------------------------------------------------------
    class StochasticsInstance
    {
    public:
        StochasticsInstance(const std::string &libraryName) : libraryName(libraryName)
        {}
        StochasticsInstance(const StochasticsInstance&) = delete;
        StochasticsInstance(StochasticsInstance&&) = delete;
        StochasticsInstance& operator=(const StochasticsInstance&) = delete;
        StochasticsInstance& operator=(StochasticsInstance&&) = delete;
        virtual ~StochasticsInstance() = default;

        const std::string &GetLibraryName() const
        {
            return libraryName;
        }

    private:
        const std::string libraryName;
    };

    //-----------------------------------------------------------------------------
    //! Instance of an stochastics module as specified in the runConfiguration.xml
    //! with its fileds libraryName
    //-----------------------------------------------------------------------------
    class CollisionDetectionInstance
    {
    public:
        CollisionDetectionInstance(const std::string &libraryName) : libraryName(libraryName)
        {}
        CollisionDetectionInstance(const CollisionDetectionInstance&) = delete;
        CollisionDetectionInstance(CollisionDetectionInstance&&) = delete;
        CollisionDetectionInstance& operator=(const CollisionDetectionInstance&) = delete;
        CollisionDetectionInstance& operator=(CollisionDetectionInstance&&) = delete;
        virtual ~CollisionDetectionInstance() = default;

        const std::string &GetLibraryName() const
        {
            return libraryName;
        }

    private:
        const std::string libraryName;
    };

    RunConfig(int startTime,
              int endTime,
              int numberInvocations,
              std::uint32_t randomSeed,
              WorldParameter *worldParameter,
              StochasticsInstance *stochasticsInstance,
              CollisionDetectionInstance *collisionDetectionInstance);
    RunConfig(const RunConfig&) = delete;
    RunConfig(RunConfig&&) = delete;
    RunConfig& operator=(const RunConfig&) = delete;
    RunConfig& operator=(RunConfig&&) = delete;
    virtual ~RunConfig();

    bool AddAgentSpawnItem(const AgentSpawnItem *agentSpawnItem);
    bool AddSpawnPointInstance(SpawnPointInstance *spawnPointInstance);
    bool AddObservationInstance(int observationId, ObservationInstance *observationInstance);
    int GetStartTime() const;
    int GetEndTime() const;
    int GetNumberInvocations() const;
    std::uint32_t GetRandomSeed() const;
    WorldParameter *GetWorldParameter() const;
    StochasticsInstance *GetStochasticsInstance() const;
    CollisionDetectionInstance *GetCollisionDetectionInstance() const;
    const std::list<const AgentSpawnItem*> &GetAgentSpawnItems() const;
    const std::list<SpawnPointInstance*> &GetSpawnPointInstances() const;
    const std::map<int, ObservationInstance*> &GetObservationInstances() const;

private:
    int startTime;
    int endTime;
    int numberInvocations;
    std::uint32_t randomSeed;
    std::list<const AgentSpawnItem*> agentSpawnItems;
    std::list<SpawnPointInstance*> spawnPointInstances;
    std::map<int, ObservationInstance*> observationInstances;
    WorldParameter *worldParameter = nullptr;
    StochasticsInstance *stochasticsInstance = nullptr;
    CollisionDetectionInstance *collisionDetectionInstance = nullptr;
};

} // namespace SimulationCommon

#endif // RUNCONFIG_H
