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
//! @file  Evaluation_Pcm/collisionDetection.h
//! @brief This file contains the definitions for the collision detection class
//!
//! The separating axis theorem is used to identify a collision.
//-----------------------------------------------------------------------------

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <list>
#include <array>
#include "agent.h"
#include "runResult.h"

//-----------------------------------------------------------------------------
//! Class for the collision detection
//-----------------------------------------------------------------------------
class CollisionDetection
{
public:
    //-----------------------------------------------------------------------------
    //! Standard constructor
    //-----------------------------------------------------------------------------
    CollisionDetection() = default;

    // removing operators
    CollisionDetection(const CollisionDetection &) = delete;
    CollisionDetection(CollisionDetection &&) = delete;
    CollisionDetection &operator=(const CollisionDetection &) = delete;
    CollisionDetection &operator=(CollisionDetection &&) = delete;

    //-----------------------------------------------------------------------------
    //! Standard destructor
    //-----------------------------------------------------------------------------
    virtual ~CollisionDetection() = default;

    //-----------------------------------------------------------------------------
    //! Stores collision parameters (using separating axes theorem)
    //!
    //! @param[in]  agent                   pointer to agent
    //! @param[in]  other                   pointer to other agent
    //! @return                             returns false in case of collision and
    //!                                     true otherwise
    //-----------------------------------------------------------------------------
    static bool IsCollision(const CD_Agent *agent,
                            const CD_Agent *other,
                            bool &isCollision);


    //-----------------------------------------------------------------------------
    //! Stores collision parameters
    //!
    //! @param[in]  agent                   pointer to agent
    //! @param[in]  other                   pointer to other agent
    //! @param[out] runResult               reference to storage location
    //! @return                     true on success
    //-----------------------------------------------------------------------------
    static bool CreateResult(const CD_Agent *agent,
                             const CD_Agent *other,
                             RunResult &runResult);

private:
    //-----------------------------------------------------------------------------
    //! Function calculate agent geometry for a specific position
    //!
    //! @param[in]  agent           pointer to the agent
    //! @param[in]  position        position of the center of gravity of the agent
    //! @param[out] resultCorners   the corners of the agent's position
    //! @param[out] resultNormals   the normals of the lines connecting the corners
    //-----------------------------------------------------------------------------
    static void CalculateAgentGeometry(const CD_Agent *agent,
                                       const Common::Vector2d &position,
                                       std::array<Common::Vector2d, 4> &resultCorners,
                                       std::array<Common::Vector2d, 2> &resultNormals);

    //-----------------------------------------------------------------------------
    //! Function calculate agent geometry for the position of the agent
    //!
    //! @param[in]  agent           pointer to the agent
    //! @param[out] resultCorners   the corners of the agent's position
    //! @param[out] resultNormals   the normals of the lines connecting the corners
    //-----------------------------------------------------------------------------
    static void GetAgentGeometry(const CD_Agent *agent,
                                 std::array<Common::Vector2d, 4> &resultCorners,
                                 std::array<Common::Vector2d, 2> &resultNormals);

    //-----------------------------------------------------------------------------
    //! Function calculating the minimum and the maximum of 4 values
    //!
    //! @param[in]  input           vector with four entries
    //! @param[out] maxValue        maximum value of the inputs
    //! @param[out] minValue        minimal value of the inputs
    //-----------------------------------------------------------------------------
    static void GetMinMax4(std::array<double, 4> &input,
                           double &maxValue,
                           double &minValue);

    //-----------------------------------------------------------------------------
    //! Function calculating the minimum and the maximum of 2 values
    //!
    //! @param[in]  input           vector with two entries
    //! @param[out] maxValue        maximum value of the inputs
    //! @param[out] minValue        minimal value of the inputs
    //-----------------------------------------------------------------------------
    static void GetMinMax2(std::array<double, 2> &input,
                           double &maxValue,
                           double &minValue);

    //-----------------------------------------------------------------------------
    //! Calculates distance of point of impact based on agent geometry.
    //!
    //! @param[in]  agent          pointer to agent
    //! @param[in]  corner         corner from which cornerDistance is measured
    //!                            (clockwise)
    //! @param[out] cornerDistance distance from previous corner to point of impact
    //! @return                    distance from upper left corner on agent border
    //!                            to point of impact
    //-----------------------------------------------------------------------------
    static bool CalculateDistOnBorder(const CD_Agent *agent,
                                      int corner,
                                      double cornerDistance,
                                      double &result);

    //-----------------------------------------------------------------------------
    //! Calculates parameters of intersection point
    //!
    //! The velocity of the first agent is transformed relatively to the second
    //! agent whose position is kept constant. After this transformation each
    //! corner of the fist agent is tested for collision separately. The corner
    //! which penetrates first into the body of the second agent determines the
    //! result.
    //!
    //! @param[in]  agent                    pointer to first agent
    //! @param[in]  other                    pointer to second agent
    //! @param[out] intersected              indicates if intersection occurred
    //! @param[out] otherDistanceFromCorner  indicates distance on border of second
    //!                                      agent where first agent penetrated
    //!                                      (measured clockwise from previous
    //!                                      corner)
    //! @param[out] otherFromCorner          number of start corner of edge where
    //!                                      first agent penetrated (counted
    //!                                      clockwise)
    //! @param[out] agentDistanceFromInitial scaling value of direction vector
    //!                                      until first agent penetrates into
    //!                                      second agent (measured from initial
    //!                                      position of first agent after relative
    //!                                      transformation)
    //! @param[out] agentCOG                 position of center of gravity of first
    //!                                      agent at point of collision
    //! @param[out] otherCOG                 position of center of gravity of second
    //!                                      agent at point of collision
    //! @param[out] agentPenetratingCorner   corner of first agent which penetrated
    //!                                      into second agent
    //! @return                              indicates if intersection point could
    //!                                      be calculated correctly
    //-----------------------------------------------------------------------------
    static bool CalculateIntersectionPoints(const CD_Agent *agent,
                                            const CD_Agent *other,
                                            bool &intersected,
                                            Common::Vector2d &intersectionPoint,
                                            double &otherDistanceFromCorner,
                                            int &otherFromCorner,
                                            double &agentDistanceFromInitial,
                                            Common::Vector2d &agentCOG,
                                            Common::Vector2d &otherCOG,
                                            int &agentPenetratingCorner);

    //-----------------------------------------------------------------------------
    //! Calculates parameters of point of contact
    //!
    //! @param[in]  agent                   pointer to agent
    //! @param[in]  other                   pointer to other agent
    //! @param[out] resultAgentDistOnBorder distance of point of contact on agent
    //!                                     border (counted clockwise from upper
    //!                                     left corner)
    //! @param[out] resultOtherDistOnBorder distance of point of contact on other
    //!                                     agents border (counted clockwise from
    //!                                     upper left corner)
    //! @param[out] resultAgentCOG          position of center of gravity of agent
    //!                                     at point of contact
    //! @param[out] resultOtherCOG          position of center of gravity of agent
    //!                                     at point of contact
    //! @return                             indicates if impact parameters could be
    //!                                     calculated correctly
    //-----------------------------------------------------------------------------
    static bool CalculatePointOfContact(const CD_Agent *agent,
                                        const CD_Agent *other,
                                        double &resultAgentDistOnBorder,
                                        double &resultOtherDistOnBorder,
                                        Common::Vector2d &resultAgentCOG,
                                        Common::Vector2d &resultOtherCOG);
};

#endif // COLLISIONDETECTION_H
