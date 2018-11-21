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

#include <list>
#include <map>
#include <tuple>
#include "worldInterface.h"
#include "observationInterface.h"
#include "vector2d.h"

namespace SimulationSlave
{

class RunResult : public RunResultInterface
{
public:
    RunResult() = default;
    RunResult(const RunResult&) = delete;
    RunResult(RunResult&&) = delete;
    RunResult& operator=(const RunResult&) = delete;
    RunResult& operator=(RunResult&&) = delete;
    virtual ~RunResult() = default;

    void Clear();

    bool AddCollision(const AgentInterface* agent,
                      Common::Vector2d &positionAgent,
                      double distanceOnBorderAgent,
                      const AgentInterface* other,
                      Common::Vector2d &positionOther,
                      double distanceOnBorderOther);
    void SetCollision()
    {
        result |= maskCollision;
    }

    void SetTimeOver()
    {
        result |= maskTimeOver;
    }

    // model callbacks
    virtual void SetEndCondition()
    {
        result |= maskEndCondition;
    }

    virtual const std::vector<Collision> *GetCollisions() const
    {
        return &collisions;
    }

    virtual bool IsCollision() const
    {
        return 0 != (result & maskCollision);
    }

    virtual bool IsTimeOver() const
    {
        return 0 != (result & maskTimeOver);
    }

    virtual bool IsEndCondition() const
    {
        return 0 != (result & maskEndCondition);
    }

private:
    const std::uint32_t maskCollision = 0x1;
    const std::uint32_t maskTimeOver = 0x2;
    const std::uint32_t maskEndCondition = 0x4;

    std::uint32_t result = 0;
    std::vector<Collision> collisions;
};

} // namespace SimulationSlave

#endif // RUNRESULT_H
