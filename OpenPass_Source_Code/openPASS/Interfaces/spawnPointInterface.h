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
//! @file  SpawnPointInterface.h
//! @brief This file contains the interface of the spawn points to interact
//!        with the framework.
//-----------------------------------------------------------------------------

#pragma once

#include <string>

#include "Interfaces/parameterInterface.h"
#include "Interfaces/callbackInterface.h"
#include "Interfaces/agentBlueprintInterface.h"

class WorldInterface;
//-----------------------------------------------------------------------------
//! This class represents the functionality of a spawn item (agent) which will
//! be spawned by a spawn point
//-----------------------------------------------------------------------------
class SpawnItemParameterInterface
{
public:
    SpawnItemParameterInterface() = default;
    SpawnItemParameterInterface(const SpawnItemParameterInterface&) = delete;
    SpawnItemParameterInterface(SpawnItemParameterInterface&&) = delete;
    SpawnItemParameterInterface& operator=(const SpawnItemParameterInterface&) = delete;
    SpawnItemParameterInterface& operator=(SpawnItemParameterInterface&&) = delete;
    virtual ~SpawnItemParameterInterface() = default;

    //-----------------------------------------------------------------------------
    //! Sets the x-coordinate of the agent to be spawned
    //!
    //! @param[in]     positionX    X-coordinate
    //-----------------------------------------------------------------------------
    virtual void SetPositionX(double positionX) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the y-coordinate of the agent to be spawned
    //!
    //! @param[in]     positionY    Y-coordinate
    //-----------------------------------------------------------------------------
    virtual void SetPositionY(double positionY) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the forward velocity of the agent to be spawned
    //!
    //! @param[in]     velocity    Forward velocity
    //-----------------------------------------------------------------------------
    virtual void SetVelocity(double velocity) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the forward acceleration of the agent to be spawned
    //!
    //! @param[in]     acceleration    Forward acceleration
    //-----------------------------------------------------------------------------
    virtual void SetAcceleration(double acceleration) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the gear of the agent to be spawned
    //!
    //! @param[in]     gear    current/calculated gear
    //-----------------------------------------------------------------------------
    virtual void SetGear(double gear) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the yaw angle of the agent to be spawned
    //!
    //! @param[in]     yawAngle    Agent orientation (0 points to east)
    //-----------------------------------------------------------------------------
    virtual void SetYaw(double yawAngle) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the next time when the agent will be spawned
    //!
    //! @param[in]     nextTimeOffset    Time offset counted from the current
    //!                                  scheduling time
    //-----------------------------------------------------------------------------
    virtual void SetNextTimeOffset(int nextTimeOffset) = 0;

    //-----------------------------------------------------------------------------
    //! Selectes the agent to be spawned next within the configured agent array
    //!
    //! @param[in]     index    Index within configured agent array
    //-----------------------------------------------------------------------------
    virtual void SetIndex(int index) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the vehicle type of the agent to be spawned
    //!
    //! @param[in]     vehicleModel    vehicleModel of agent
    //-----------------------------------------------------------------------------
    virtual void SetVehicleModel(std::string vehicleModel) = 0;

    virtual double GetPositionX() const = 0;

    virtual double GetPositionY() const = 0;

    virtual double GetVelocity() const = 0;

    virtual double GetAcceleration() const = 0;

    virtual double GetYaw() const = 0;

    virtual std::string GetVehicleModel() const = 0;
};

//-----------------------------------------------------------------------------
//! This class represents a spawn point
//-----------------------------------------------------------------------------
class SpawnPointInterface
{
public:    
    SpawnPointInterface(WorldInterface *world,
                        const ParameterInterface *parameters,
                        const CallbackInterface *callbacks) :
        world(world),
        parameters(parameters),
        callbacks(callbacks)
    {}
    SpawnPointInterface(const SpawnPointInterface&) = delete;
    SpawnPointInterface(SpawnPointInterface&&) = delete;
    SpawnPointInterface& operator=(const SpawnPointInterface&) = delete;
    SpawnPointInterface& operator=(SpawnPointInterface&&) = delete;
    virtual ~SpawnPointInterface() = default;

    //-----------------------------------------------------------------------------
    //! Fills all parameters of the agentBlueprint with with help of the agentsampler.
    //!
    //! @param[out] agentBlueprint is used to create an agent.
    //! @return     true if AgentBlueprint was filled out successfully
    //-----------------------------------------------------------------------------
    virtual bool GenerateAgent(AgentBlueprintInterface* agentBlueprint) = 0;

    //virtual void SetSpawnItem(SpawnItemParameterInterface &spawnItem, int maxIndex) = 0;

protected:
    //-----------------------------------------------------------------------------
    //! Retrieves the world.
    //!
    //! @return                  World (scenery and agents)
    //-----------------------------------------------------------------------------
    WorldInterface *GetWorld() const
    {
        return world;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the configuration parameters of the module.
    //!
    //! @return                  Configuration parameters
    //-----------------------------------------------------------------------------
    const ParameterInterface *GetParameters() const
    {
        return parameters;
    }

    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro.
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    WorldInterface *world;                //!< References the world of the framework
    const ParameterInterface *parameters; //!< References the configuration parameters
    const CallbackInterface *callbacks;   //!< References the callback functions of the framework    
};


