/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  runResultInterface.h
//! @brief This file contains the interface to gain access to the results of a
//!        run.
//!
//! Important for the collision detection library.
//-----------------------------------------------------------------------------

#ifndef RUNRESULTINTERFACE
#define RUNRESULTINTERFACE

#include <math.h>
#include "vector2d.h"
#include "agentInterface.h"


//! Provides access to run results
class RunResultInterface
{
public:
    class OpponentItem
    {
    public:
        OpponentItem(const AgentInterface *agent,
                     double yawAngle,
                     Common::Vector2d position,
                     Common::Vector2d velocity,
                     double distanceOnBorder):
            agent(agent),
            yawAngle(yawAngle),
            position(position),
            velocity(velocity),
            distanceOnBorder(distanceOnBorder)
        {}

        OpponentItem(const AgentInterface *agent,
                     Common::Vector2d position,
                     double distanceOnBorder):
            agent(agent),
            yawAngle(agent->GetYawAngle()),
            position(position),
            velocity(Common::Vector2d(agent->GetVelocityX(), agent->GetVelocityY())),
            distanceOnBorder(distanceOnBorder)
        {}

        const AgentInterface *agent = nullptr;
        double yawAngle = 0.0;
        Common::Vector2d position = Common::Vector2d(0.0, 0.0);
        Common::Vector2d velocity = Common::Vector2d(0.0, 0.0);
        double distanceOnBorder = 0.0; // distance from upper left corner on agent border to point of impact
    };

    class Collision
    {
    public:
        Collision(OpponentItem opponent1,
                  OpponentItem opponent2):
            opponent1(opponent1),
            opponent2(opponent2)
        {
            Common::Vector2d vVector1(opponent1.velocity);
            Common::Vector2d vVector2(opponent2.velocity);
            vVector1.Rotate(opponent1.yawAngle);
            vVector2.Rotate(opponent2.yawAngle);

            Common::Vector2d diffV = vVector1 - vVector2;
            relativeVelocity = diffV.Length();
            double l1 = opponent1.velocity.Length();
            double l2 = opponent2.velocity.Length();

            relativeAngle = (l1!=0 && l2!=0)? ::acos(vVector1.Dot(vVector2) / l1 / l2):
                                              ::abs(opponent1.agent->GetYawAngle() - opponent2.agent->GetYawAngle());
        }

        OpponentItem opponent1;
        OpponentItem opponent2;

        double relativeVelocity;
        double relativeAngle;
    };

    RunResultInterface() = default;
    RunResultInterface(const RunResultInterface&) = delete;
    RunResultInterface(RunResultInterface&&) = delete;
    RunResultInterface& operator=(const RunResultInterface&) = delete;
    RunResultInterface& operator=(RunResultInterface&&) = delete;
    virtual ~RunResultInterface() = default;

    //-----------------------------------------------------------------------------
    //! Indicate end of simulation run by observation module
    //-----------------------------------------------------------------------------
    virtual void SetEndCondition()
    {}

    //-----------------------------------------------------------------------------
    //! Provides the agents which collided in the simulation run
    //!
    //! @return                List of agents
    //-----------------------------------------------------------------------------
    virtual const std::vector<Collision> *GetCollisions() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the position of the agent COGs at the moment of contact
    //!
    //! @return                Mapping of agents to x-/y-coordinates
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, std::tuple<double, double>> *GetPositions() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the velocities of the agents at the moment of contact
    //!
    //! @return                Mapping of agents to forward-/sideward-velocities
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, std::tuple<double, double>> *GetVelocities() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the yaw angles of the agents at the moment of contact
    //!
    //! @return                Mapping of agents to yaw angles
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, double> *GetYawAngles() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the distances on the border of the agents at the moment of
    //! contact. The are counted clock-wise starting from the upper left corner of
    //! the boundary box of an agent and ending at the point of contact.
    //!
    //! @return                Mapping of agents to
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, double> *GetDistances() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Indicates if the simulation ended by a collision
    //!
    //! @return                True if the simulation ended by collision
    //-----------------------------------------------------------------------------
    virtual bool IsCollision() const
    {
        return false;
    }

    //-----------------------------------------------------------------------------
    //! Indicates if the simulation ended by exceeding the end time
    //!
    //! @return                True if the simulation ended by time over
    //-----------------------------------------------------------------------------
    virtual bool IsTimeOver() const
    {
        return false;
    }

    //-----------------------------------------------------------------------------
    //! Indicates if the simulation ended by an observation module
    //!
    //! @return                True if the simulation ended by the end condition of
    //!                        an observation module
    //-----------------------------------------------------------------------------
    virtual bool IsEndCondition() const
    {
        return false;
    }

    //-----------------------------------------------------------------------------
    //! Adds a collision to a list of collisions with information about the collided
    //! agents.
    //!
    //! @return                True if successful
    //-----------------------------------------------------------------------------
    virtual bool AddCollision(const AgentInterface* agent,
                      Common::Vector2d &positionAgent,
                      double distanceOnBorderAgent,
                      const AgentInterface* other,
                      Common::Vector2d &positionOther,
                      double distanceOnBorderOther) = 0;

    //-----------------------------------------------------------------------------
    //! Sets a collision flag.
    //-----------------------------------------------------------------------------
    virtual void SetCollision() = 0;
};

#endif // RUNRESULTINTERFACE

