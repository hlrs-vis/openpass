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
//! @file  runResult.h
//! @brief This file represents the result of a simulation run.
//-----------------------------------------------------------------------------

#ifndef RUNRESULT_H
#define RUNRESULT_H

#include <QtGlobal>
#include <list>
#include <map>
#include "worldInterface.h"
#include "observationInterface.h"

//-----------------------------------------------------------------------------
//! Class that has all informations on the run result
//-----------------------------------------------------------------------------
class RunResult : public RunResultInterface
{
public:
    //-----------------------------------------------------------------------------
    //! Standard constructor
    //-----------------------------------------------------------------------------
    RunResult() = default;

    // removing operators
    RunResult(const RunResult &) = delete;
    RunResult(RunResult &&) = delete;
    RunResult &operator=(const RunResult &) = delete;
    RunResult &operator=(RunResult &&) = delete;

    //-----------------------------------------------------------------------------
    //! Standard destructor
    //-----------------------------------------------------------------------------
    virtual ~RunResult() = default;

    //-----------------------------------------------------------------------------
    //! Function adding another agent to the run result that is involved in a
    //! collision
    //!
    //! @param[in]  agent       The agent that should be added (containing all static information)
    //! @param[in]  position    The position of that agent
    //! @param[in]  yawAngle    The yaw angle of that agent
    //! @param[in]  velocityX   The lateral velocity of that agent
    //! @param[in]  velocityy   The vertical velocity of that agent
    //! @param[in]  distance    The 1d distance of the collision point on the boundary of the agent
    //-----------------------------------------------------------------------------
    bool AddCollisionAgent(const AgentInterface *agent,
                           Common::Vector2d &position,
                           double yawAngle,
                           double velocityX,
                           double velocityY,
                           double distance);

    //-----------------------------------------------------------------------------
    //! Function setting the status to 'collision happened'
    //-----------------------------------------------------------------------------
    void SetCollision()
    {
        result |= maskCollision;
    }

    //-----------------------------------------------------------------------------
    //! Function setting the status to 'time over'
    //-----------------------------------------------------------------------------
    void SetTimeOver()
    {
        result |= maskTimeOver;
    }

    //-----------------------------------------------------------------------------
    //! Function setting the status to 'end due to evaluation module'
    //-----------------------------------------------------------------------------
    virtual void SetEndCondition()
    {
        result |= maskEndCondition;
    }

    //-----------------------------------------------------------------------------
    //! Getter-Function for the collision agents
    //! @return         The collision agents
    //-----------------------------------------------------------------------------
    virtual const std::list<const AgentInterface *> *GetCollisionAgents() const
    {
        return &agents;
    }

    //-----------------------------------------------------------------------------
    //! Getter-Function for the map of agent-pointers to their 2d-positions
    //! @return         The map of agent-pointers to their 2d-positions
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface *, std::tuple<double, double> > *GetPositions() const
    {
        return &positions;
    }

    //-----------------------------------------------------------------------------
    //! Getter-Function for the map of agent-pointers to their 2d-velocities
    //! @return         The map of agent-pointers to their 2d-velocities
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface *, std::tuple<double, double> > *GetVelocities() const
    {
        return &velocities;
    }

    //-----------------------------------------------------------------------------
    //! Getter-Function for the map of agent-pointers to their yaw angles
    //! @return         The map of agent-pointers to their yaw angles
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface *, double> *GetYawAngles() const
    {
        return &yawAngles;
    }

    //-----------------------------------------------------------------------------
    //! Getter-Function for the map of agent-pointers to their 1d-on-boundary-distances
    //! @return         The map of agent-pointers to their 1d-on-boundary-distances
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface *, double> *GetDistances() const
    {
        return &distances;
    }

    //-----------------------------------------------------------------------------
    //! Function returning whether the situation ended due to 'collision happened'
    //! @return         true, if end due to 'collision happened'
    //-----------------------------------------------------------------------------
    virtual bool IsCollision() const
    {
        return 0 != (result & maskCollision);
    }

    //-----------------------------------------------------------------------------
    //! Function returning whether the situation ended due to 'time over'
    //! @return         true, if end due to 'time over'
    //-----------------------------------------------------------------------------
    virtual bool IsTimeOver() const
    {
        return 0 != (result & maskTimeOver);
    }

    //-----------------------------------------------------------------------------
    //! Function returning whether the situation ended due to 'evaluation module''
    //! @return         true, if end due to 'evaluation module'
    //-----------------------------------------------------------------------------
    virtual bool IsEndCondition() const
    {
        return 0 != (result & maskEndCondition);
    }

    virtual bool AddCollision(const AgentInterface *agent,
                              Common::Vector2d &positionAgent,
                              double distanceOnBorderAgent,
                              const AgentInterface *other,
                              Common::Vector2d &positionOther,
                              double distanceOnBorderOther)
    {
        Q_UNUSED(agent);
        Q_UNUSED(positionAgent);
        Q_UNUSED(distanceOnBorderAgent);
        Q_UNUSED(other);
        Q_UNUSED(positionOther);
        Q_UNUSED(distanceOnBorderOther);
        return false;
    }

private:
    const std::uint32_t maskCollision =
        0x1;        //!< mask for the result flag marking 'collision happened'
    const std::uint32_t maskTimeOver = 0x2;         //!< mask for the result flag marking 'time over'
    const std::uint32_t maskEndCondition =
        0x4;     //!< mask for the result flag marking 'end due to evaluation module'

    std::uint32_t result = 0;                       //!< result flag
    std::list<const AgentInterface *>
    agents;                                   //!< list of agents provoking a collision
    std::map<const AgentInterface *, std::tuple<double, double>>
                                                              positions;     //!< map of agent-pointers to their 2d-positions
    std::map<const AgentInterface *, std::tuple<double, double>>
                                                              velocities;    //!< map of agent-pointers to their 2d-velocities
    std::map<const AgentInterface *, double>
    yawAngles;                         //!< map of agent-pointers to their yaw angles
    std::map<const AgentInterface *, double>
    distances;                         //!< map of agent-pointers to their 1d-on-boundary-distances
};

#endif // RUNRESULT_H
