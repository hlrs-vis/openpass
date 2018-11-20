/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  CommonSceneryHelper.h
//! @brief Helper functionality to receive specific data from the world
//-----------------------------------------------------------------------------

#pragma once

#include "agentInterface.h"
#include "worldInterface.h"
#include "WorldData.h"


namespace CommonSceneryHelper
{
//-----------------------------------------------------------------------------
//! @brief Containing static functions to obtain information from the world
//!
//! Contains only static functions that can be called by sensors to obtain
//! information on their surrounding from the virtual world
//-----------------------------------------------------------------------------
class SceneryHelper
{
public:
    static int GetPotentialLaneIdLeft(int laneId);
    static int GetPotentialLaneIdRight(int laneId);

    static int GetIdOfLastCarInLane(
            int iLane,
            WorldInterface* world);

    static int GetIdOfLastObjectInLane(
            int iLane,
            WorldInterface* world);


    //-----------------------------------------------------------------------------
    //! @brief Returns information on the vehicle closest to the end of a lane
    //!
    //! @param [in]   iLane     lane number
    //! @param [in]   world     pointer to the WorldInterface containing the virtual world
    //!
    //! @return information on the found car
    //-----------------------------------------------------------------------------
    static int GetIdOfFirstCarInLane(
            int iLane,
            WorldInterface *world);

    static int GetIdOfFirstObjectInLane(
            int iLane,
            WorldInterface *world);


    static void AssignCarInfo(AgentInterface *agentInterface, double accSensDist);
    //static double GetDistanceToEgoP(AgentInterface *agentInterface);

    static double GetVelocityLatP(AgentInterface *agentInterface);
    static double GetVelocityAbsP(AgentInterface *agentInterface);

    //! Get pointers to front vehicles.
    static double GetPositionLatP(const AgentInterface *agentInterface);

    //-----------------------------------------------------------------------------
    //! @brief Recursively callable helper function to determine the distance from the current agent to the end of its current lane
    //!
    //! This function returns INFINITY if no end of lane is found within sightDistance
    //!
    //! @param [in]   lane               the current lane
    //! @param [in]   remainingDistance  the remaining maximal distance the end of lane is looked for
    //!
    //! @return the distance from the current agent to the end of its current lane
    //-----------------------------------------------------------------------------
    static double GetDistanceToEndOfLaneHelper(OWL::CLane* lane,
                                               double distanceOfInterest,
                                               double currentPosition);

    //-----------------------------------------------------------------------------
    //! @brief Returns the agent with the lowest distance to start
    //!
    //! This function returns the closest agent to start
    //!
    //! @param [in]   std::vector<const AgentInterface*>*   pointer to an vector filled with agents
    //!
    //! @return closest agent to start
    //-----------------------------------------------------------------------------
    static const AgentInterface* GetLastAgent(std::vector<const AgentInterface*> *agents);

    static const AgentInterface* GetFirstAgent(std::vector<const AgentInterface *> *agents);
};

}
