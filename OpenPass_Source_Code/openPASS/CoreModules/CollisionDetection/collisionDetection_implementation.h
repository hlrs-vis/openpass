/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef COLLISIONDETECTION_IMPLEMENTATION_H
#define COLLISIONDETECTION_IMPLEMENTATION_H

#include <list>
#include <array>
#include "collisionDetectionInterface.h"
#include "callbackInterface.h"

/**
* \addtogroup CoreModules_Basic openPASS CoreModules basic
* @{
* \addtogroup CollisionDetection
* @{
* \brief detect a collision between two agents
*
* The CollisionDetection module implements a CollisionDetectionInterface which is used
* by the framework.
* It is used to detect several collisions between agents.
* The results are written in a RunResultInterface given by the framework.
*
* \section collisionDetection_inputs Inputs
* name | meaning
* -----|---------
* callbacks | CallbackInterface to send log information to the framework
*
* \section collisionDetection_outputs Outputs
 name | meaning
* -----|---------
* *CollisionDetectionInterface | Provides a reference to an implementation of a CollisionDetectionInterface to the framework.
*
* \section collisionDetection_configParameters Parameters to be specified in runConfiguration.xml
* tag | meaning
* -----|---------
* CollisionDetection | Name of collision Detection library. "CollisionDetection" by default.
*
* \see CollisionDetectionInterface
*
*   @}
*   @} */

/*!
 * \brief The CollisionDetection_Implementation class implements a CollisionDetectionInterface
 *
 * The CollisionDetection module implements a CollisionDetectionInterface which is used
 * by the framework.
 * It is used to detect several collisions between agents.
 * The results are written in a RunResultInterface given by the framework.
 *
 * \ingroup CollisionDetection
 */
class CollisionDetection_Implementation : public CollisionDetectionInterface
{
public:
    CollisionDetection_Implementation(const CallbackInterface *callbacks)
        : callbacks(callbacks)
    {}
    CollisionDetection_Implementation(const CollisionDetection_Implementation &) = delete;
    CollisionDetection_Implementation(CollisionDetection_Implementation &&) = delete;
    CollisionDetection_Implementation &operator=(const CollisionDetection_Implementation &) = delete;
    CollisionDetection_Implementation &operator=(CollisionDetection_Implementation &&) = delete;
    virtual ~CollisionDetection_Implementation() = default;

    /*!
     * \brief SetAgents
     * Provides the collision detection access to all agents who shall be checked for
     * collisions.
     *
     * \param[in] agents        map of agents
     */
    void SetAgents(const std::map<int, const AgentInterface *> &agents);

    /*!
     * \brief HandleCollisionsInAgents
     * Checks if a collision happened. Implements the separating axes theorem.
     *
     * \param[out] runResult                reference to storage location
     * \param[out] isCollision              Flag if a collision happened
     * \return                              False if an error occurred, true otherwise
     */
    bool HandleCollisionsInAgents(RunResultInterface &runResult, bool &isCollision);

protected:
    /*!
     * \brief Log
     * Provides callback to LOG() macro
     *
     * \param[in] logLevel          Importance of log
     * \param[in] file              Name of file where log is called
     * \param[in] line              Line within file where log is called
     * \param[in] message           Message to log
     */
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if (callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    /*!
     * \brief CalculateAgentGeometry
     * Calculate the geometry of an agent and provide geometrical vectors.
     *
     * \param[in] agent             agent reference
     * \param[out] position         position vector
     * \param[out] resultCorners    vector of corners
     * \param[out] resultNormals    vector of normals
     */
    void CalculateAgentGeometry(const AgentInterface *agent,
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
    void GetAgentGeometry(const AgentInterface *agent,
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
    //! Calculates distance of point of impact based on agent geometry.
    //!
    //! @param[in]  agent          pointer to agent
    //! @param[in]  corner         corner from which cornerDistance is measured
    //!                            (clockwise)
    //! @param[in]  cornerDistance distance from previous corner to point of impact
    //! @param[out] result         distance from upper left corner on agent border
    //!                            to point of impact
    //! @return                    true for success
    //-----------------------------------------------------------------------------
    bool CalculateDistOnBorder(const AgentInterface *agent,
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
    //! @param[out] intersectionPoint        point of intersection
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
    bool CalculateIntersectionPoints(const AgentInterface *agent,
                                     const AgentInterface *other,
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
    bool CalculatePointOfContact(const AgentInterface *agent,
                                 const AgentInterface *other,
                                 double &resultAgentDistOnBorder,
                                 double &resultOtherDistOnBorder,
                                 Common::Vector2d &resultAgentCOG,
                                 Common::Vector2d &resultOtherCOG);

    //-----------------------------------------------------------------------------
    //! Stores collision parameters
    //!
    //! @param[in]  agent                   pointer to agent
    //! @param[in]  other                   pointer to other agent
    //! @param[out] runResult               reference to storage location
    //-----------------------------------------------------------------------------
    bool CreateResult(const AgentInterface *agent,
                      const AgentInterface *other,
                      RunResultInterface &runResult);

    const std::map<int, const AgentInterface *> *agents = nullptr;
    const CallbackInterface *callbacks;
};

#endif // COLLISIONDETECTION_IMPLEMENTATION_H
