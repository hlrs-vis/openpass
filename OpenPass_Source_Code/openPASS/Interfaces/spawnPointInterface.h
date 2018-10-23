/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnPointInterface.h
//! @brief This file contains the interface of the spawn points to interact
//!        with the framework.
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINTINTERFACE_H
#define SPAWNPOINTINTERFACE_H

#include <string>
#include "parameterInterface.h"
#include "stochasticsInterface.h"

class WorldInterface;
//-----------------------------------------------------------------------------
//! This class represents the functionality of a spawn item (agent) which will
//! be spawned by a spawn point
//-----------------------------------------------------------------------------
class SpawnItemParameterInterface
{
public:
    SpawnItemParameterInterface() = default;
    SpawnItemParameterInterface(const SpawnItemParameterInterface &) = delete;
    SpawnItemParameterInterface(SpawnItemParameterInterface &&) = delete;
    SpawnItemParameterInterface &operator=(const SpawnItemParameterInterface &) = delete;
    SpawnItemParameterInterface &operator=(SpawnItemParameterInterface &&) = delete;
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
    //! @param[in]     velocityX    Forward velocity
    //-----------------------------------------------------------------------------
    virtual void SetVelocityX(double velocityX) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the sideward velocity of the agent to be spawned
    //!
    //! @param[in]     velocityY    Sideward velocity
    //-----------------------------------------------------------------------------
    virtual void SetVelocityY(double velocityY) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the forward acceleration of the agent to be spawned
    //!
    //! @param[in]     accelerationX    Forward acceleration
    //-----------------------------------------------------------------------------
    virtual void SetAccelerationX(double accelerationX) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the sideward acceleration of the agent to be spawned
    //!
    //! @param[in]     accelerationY    Sideward acceleration
    //-----------------------------------------------------------------------------
    virtual void SetAccelerationY(double accelerationY) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the yaw angle of the agent to be spawned
    //!
    //! @param[in]     yawAngle    Agent orientation (0 points to east)
    //-----------------------------------------------------------------------------
    virtual void SetYawAngle(double yawAngle) = 0;

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

    virtual double GetPositionX() const = 0;

    virtual double GetPositionY() const = 0;

    virtual double GetVelocityX() const = 0;

    virtual double GetVelocityY() const = 0;

    virtual double GetAccelerationX() const = 0;

    virtual double GetAccelerationY() const = 0;

    virtual double GetYawAngle() const = 0;
};

//-----------------------------------------------------------------------------
//! This class represents a spawn point
//-----------------------------------------------------------------------------
class SpawnPointInterface
{
public:
    SpawnPointInterface(StochasticsInterface *stochastics,
                        WorldInterface *world,
                        const ParameterInterface *parameters,
                        const CallbackInterface *callbacks) :
        stochastics(stochastics),
        world(world),
        parameters(parameters),
        callbacks(callbacks)
    {}
    SpawnPointInterface(const SpawnPointInterface &) = delete;
    SpawnPointInterface(SpawnPointInterface &&) = delete;
    SpawnPointInterface &operator=(const SpawnPointInterface &) = delete;
    SpawnPointInterface &operator=(SpawnPointInterface &&) = delete;
    virtual ~SpawnPointInterface() = default;

    //-----------------------------------------------------------------------------
    //! Loads the spawn item (agent) into the spawn point which will be spawned next.
    //!
    //! @param[out]    spawnItem agent configuration
    //! @param[in]     maxIndex  maximum index of agent array configuration (spawn
    //!                          point implementation must not exceed this index)
    //-----------------------------------------------------------------------------
    virtual void SetSpawnItem(SpawnItemParameterInterface &spawnItem,
                              int maxIndex) = 0;

protected:
    //-----------------------------------------------------------------------------
    //! Retrieves the stochastics functionality.
    //!
    //! @return                  Stochastics functionality of framework
    //-----------------------------------------------------------------------------
    StochasticsInterface *GetStochastics() const
    {
        return stochastics;
    }

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
        if (callbacks) {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    StochasticsInterface *stochastics;    //!< References stochastics functionality of the framework
    WorldInterface *world;                //!< References the world of the framework
    const ParameterInterface *parameters; //!< References the configuration parameters
    const CallbackInterface *callbacks;   //!< References the callback functions of the framework
};

#endif // SPAWNPOINTINTERFACE_H
