/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  collisionDetection.h
//! @brief This file contains the internal representation of the
//!        collisionDetection. For more information see collisionDetection.dll
//!        documentation.
//-----------------------------------------------------------------------------

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <list>
#include <array>
#include "vector2d.h"
#include "collisionDetectionInterface.h"
#include "runResultInterface.h"
#include "collisionDetectionBinding.h"
#include "collisionDetectionLibrary.h"

namespace SimulationSlave
{

class CollisionDetection : public CollisionDetectionInterface
{
public:
    CollisionDetection(CollisionDetectionBinding *collisionDetectionBinding) :
        collisionDetectionBinding(collisionDetectionBinding)
    {}
    CollisionDetection(const CollisionDetection&) = delete;
    CollisionDetection(CollisionDetection&&) = delete;
    CollisionDetection& operator=(const CollisionDetection&) = delete;
    CollisionDetection& operator=(CollisionDetection&&) = delete;
    virtual ~CollisionDetection() = default;

    void SetAgents(const std::map<int, const AgentInterface*> &agents){
        return implementation->SetAgents(agents);
    }

    bool HandleCollisionsInAgents(RunResultInterface &runResult, bool &isCollision){
        return implementation->HandleCollisionsInAgents(runResult, isCollision);
    }

    bool Instantiate(SimulationCommon::RunConfig::CollisionDetectionInstance *collisionDetectionInstance)
    {
    if((!collisionDetectionBinding) || (!collisionDetectionInstance)){
        return false;
    }else if(!implementation){
        implementation = collisionDetectionBinding->Instantiate(collisionDetectionInstance);
        if(!implementation){
            return false;
        }
    }
    return true;
    }

private:
    CollisionDetectionBinding *collisionDetectionBinding = nullptr;
    CollisionDetectionInterface *implementation = nullptr;

};

} // namespace SimulationSlave

#endif // COLLISIONDETECTION_H
