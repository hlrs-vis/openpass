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
/** @file  CollisionDetector.h
*	@brief Detects wether agents collided.
*
*   This manipulator detects wether agents collided with either other agents or traffic objects.
*   Once a collision is detected a CollisionEvent is created and forwarded to the EventNetwork. */
//-----------------------------------------------------------------------------

#pragma once

#include "EventDetectorCommonBase.h"
#include "Common/boostGeometryCommon.h"

typedef enum
{
    UpperLeft = 0,
    UpperRight,
    LowerRight,
    LowerLeft,
    NumberCorners
} CornerType;

typedef enum
{
    UpperEdge = 0,
    RightEdge,
    LowerEdge,
    LeftEdge,
    NumberEdges
} EdgeType;

typedef enum
{
    Right = 0,
    Up,
    NumberNormals
} NormalType;

typedef enum
{
    ProjectedFirst = 0,
    ProjectedSecond,
    NumberProjectedOwnAxis,
    ProjectedThird = 2,
    ProjectedFourth,
    NumberProjectedOpponentAxis
} ProjectedType;

static const double ROTATION_EPS = 0.0001;

//-----------------------------------------------------------------------------
/** \brief This class detectes wether a collision happen in the simulation.
*   \details This class detects wether an agent collided with either another agent
*   or a traffic object. In case a collision happend an event is created.
*
* 	\ingroup EventDetector */
//-----------------------------------------------------------------------------
class CollisionDetector : public EventDetectorCommonBase
{
public:
    CollisionDetector(WorldInterface *world,
                      ParameterInterface* parameters,
                      SimulationSlave::EventNetworkInterface *eventNetwork,
                      const CallbackInterface *callbacks,
                      StochasticsInterface *stochastics);

    virtual ~CollisionDetector() = default;

    /*!
    * \brief Triggers the functionality of this class
    *
    * \details Trigger gets called each cycle timestep.
    * This function is repsonsible for creating events
    *
    * @param[in]     time    Current time.
    */
    virtual void Trigger(int time);

    /*!
     * \brief CalculateAgentGeometry
     * Calculate the geometry of an agent and provide geometrical vectors.
     *
     * \param[in] agent             agent reference
     * \param[out] position         position vector
     * \param[out] resultCorners    vector of corners
     * \param[out] resultNormals    vector of normals
     */
    void CalculateWorldObjectGeometry(const WorldObjectInterface *worldObject,
                                      const Common::Vector2d &position,
                                      std::array<Common::Vector2d, 4> &resultCorners,
                                      std::array<Common::Vector2d, 2> &resultNormals);

    /*!
     * \brief GetAgentGeometry
     * Retrieve geometrical vectors of the agent.
     *
     * \param[in] agent                 agent reference
     * \param[out] resultCorners        vector of corners
     * \param[out] resultNormals        vector of normals
     */
    void GetWorldObjectGeometry(const WorldObjectInterface *worldObject,
                                std::array<Common::Vector2d, 4> &resultCorners,
                                std::array<Common::Vector2d, 2> &resultNormals);

    /*!
     * \brief GetMinMax4
     * Retrieve the max and min value of an array of four.
     *
     * \param[in] input             input array with four double values
     * \param[out] maxValue         maximal value
     * \param[out] minValue         minimal value
     */
    void GetMinMax4(std::array<double, 4> &input,
                    double &maxValue,
                    double &minValue);

     /*!
     * \brief GetMinMax2
     * Retrieve the max and min value of an array of two.
     *
     * \param[in] input             input array with two double values
     * \param[out] maxValue         maximal value
     * \param[out] minValue         minimal value
     */
    void GetMinMax2(std::array<double, 2> &input,
                    double &maxValue,
                    double &minValue);

    //-----------------------------------------------------------------------------
    /*! Calculates distance of point of impact based on agent geometry.
    *
    * @param[in]  agent          pointer to agent
    * @param[in]  corner         corner from which cornerDistance is measured
    *                            (clockwise)
    * @param[in]  cornerDistance distance from previous corner to point of impact
    * @param[out] result         distance from upper left corner on agent border
    *                            to point of impact
    * @return                    true for success */
    //-----------------------------------------------------------------------------
    bool CalculateDistOnBorder(const WorldObjectInterface *worldObject,
                               int corner,
                               double cornerDistance,
                               double &result);

    //-----------------------------------------------------------------------------
    /*! Calculates parameters of intersection point
    *
    * The velocity of the first agent is transformed relatively to the second
    * agent whose position is kept constant. After this transformation each
    * corner of the fist agent is tested for collision separately. The corner
    * which penetrates first into the body of the second agent determines the
    * result.
    *
    * @param[in]  agent                    pointer to first agent
    * @param[in]  other                    pointer to second agent
    * @param[out] intersectionPoint        point of intersection
    * @param[out] intersected              indicates if intersection occurred
    * @param[out] otherDistanceFromCorner  indicates distance on border of second
    *                                      agent where first agent penetrated
    *                                      (measured clockwise from previous
    *                                      corner)
    * @param[out] otherFromCorner          number of start corner of edge where
    *                                      first agent penetrated (counted
    *                                      clockwise)
    * @param[out] agentDistanceFromInitial scaling value of direction vector
    *                                      until first agent penetrates into
    *                                      second agent (measured from initial
    *                                      position of first agent after relative
    *                                      transformation)
    * @param[out] agentReferencePoint      position of reference point of first
    *                                      agent at point of collision
    * @param[out] otherReferencePoint      position of reference point of second
    *                                      agent at point of collision
    * @param[out] agentPenetratingCorner   corner of first agent which penetrated
    *                                      into second agent
    * @return                              indicates if intersection point could
    *                                      be calculated correctly */
    //-----------------------------------------------------------------------------
    bool CalculateIntersectionPoints(const WorldObjectInterface *agent,
                                     const WorldObjectInterface *other,
                                     bool &intersected,
                                     Common::Vector2d &intersectionPoint,
                                     double &otherDistanceFromCorner,
                                     int &otherFromCorner,
                                     double &agentDistanceFromInitial,
                                     Common::Vector2d &agentReferencePoint,
                                     Common::Vector2d &otherReferencePoint,
                                     int &agentPenetratingCorner);

    //-----------------------------------------------------------------------------
    /*! Calculates parameters of point of contact
    *
    * @param[in]  agent                   pointer to agent
    * @param[in]  other                   pointer to other agent
    * @param[out] resultAgentDistOnBorder distance of point of contact on agent
    *                                     border (counted clockwise from upper
    *                                     left corner)
    * @param[out] resultOtherDistOnBorder distance of point of contact on other
    *                                     agents border (counted clockwise from
    *                                     upper left corner)
    * @param[out] resultAgentReferencePoint position of center of gravity of agent
    *                                       at point of contact
    * @param[out] resultOtherReferencePoint position of center of gravity of agent
    *                                       at point of contact
    * @return                             indicates if impact parameters could be
    *                                     calculated correctly */
    //-----------------------------------------------------------------------------
    bool CalculatePointOfContact(AgentInterface *agent,
                                 const WorldObjectInterface *other,
                                 double &resultAgentDistOnBorder,
                                 double &resultOtherDistOnBorder,
                                 Common::Vector2d &resultAgentReferencePoint,
                                 Common::Vector2d &resultOtherReferencePoint);

    //-----------------------------------------------------------------------------
    /*! Check For Collision between worldObjects
    *
    * @param[in]  other          pointer to worldObject
    * @param[in]  agent          pointer to agent
    * @param[in]  agentCorners   agent corners
    * @param[in]  agentNormals   agent normals
    *
    * @return                    true when collision detected */
    //-----------------------------------------------------------------------------
    bool DetectCollision(const WorldObjectInterface *other,
                         AgentInterface *agent,
                         std::array<Common::Vector2d, NumberCorners> agentCorners,
                         std::array<Common::Vector2d, NumberNormals> agentNormals);

    const std::map<int, AgentInterface*> *agents = nullptr;
    const std::vector<const TrafficObjectInterface*> *trafficObjects = nullptr;

private:
    //-----------------------------------------------------------------------------
    /*! Check For Collision between worldObjects
    *
    * @param[in]  agent          pointer to agent
    * @param[in]  agentNormals   world object normals
    * @param[in]  agentCorners   world object corners
    * @param[in]  other          pointer to other agent/object
    * @param[in]  otherNormals   world object normals
    * @param[in]  otherCorners   world object corners
    *
    * @return                    true when collision detected */
    //-----------------------------------------------------------------------------
    bool DetectIntersectionOfTwoWorldObjects(const WorldObjectInterface *agent,
                                             std::array<Common::Vector2d, NumberNormals> agentNormals,
                                             std::array<Common::Vector2d, NumberCorners> agentCorners,
                                             const WorldObjectInterface *other, std::array<Common::Vector2d, NumberNormals> otherNormals,
                                             std::array<Common::Vector2d, NumberCorners> otherCorners);

    //-----------------------------------------------------------------------------
    /*! Creates a CollisionEvent and inserts it into the event network
    *
    * @param[in]  time
    * @param[in]  agent          pointer to agent
    * @param[in]  other          pointer to other world object */
    //-----------------------------------------------------------------------------
    void DetectedCollisionWithObject(int time,
                                     AgentInterface *agent,
                                     const WorldObjectInterface *other);

    //-----------------------------------------------------------------------------
    /*! Creates a CollisionEvent and inserts it into the event network
    *
    * @param[in]  time
    * @param[in]  agent          pointer to agent
    * @param[in]  other          pointer to other agent */
    //-----------------------------------------------------------------------------
    void DetectedCollisionWithAgent(int time,
                                    AgentInterface *agent,
                                    AgentInterface *other);

};


