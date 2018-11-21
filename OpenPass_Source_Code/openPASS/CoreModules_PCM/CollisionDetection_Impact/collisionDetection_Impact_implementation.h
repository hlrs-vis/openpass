/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef COLLISIONDETECTION_SHALGORITHM_IMPLEMENTATION_H
#define COLLISIONDETECTION_SHALGORITHM_IMPLEMENTATION_H

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES

#include <list>
#include <array>
#include <math.h>
#include "collisionDetectionInterface.h"
#include "callbackInterface.h"
#include "polygon.h"
#include "postCrashDynamic.h"

/**
* \addtogroup CoreModules_PCM openPASS CoreModules pcm
* @{
* \addtogroup CollisionDetection_Impact
*
* \brief Collision detection module to detect a collision between two agents and calculate post-crash dynamics.
*
* The collision detection module implements a CollisionDetection_Interface which is used
* by the framework. It is used to detect collisions between agents.
*
* The algorithms for collision detection and calculation of the post-crash dynamics are based
* on algorithms provided by Harald Kolk (TU Graz).
*
* The algorithm for collision dection is based on the Sutherland–Hodgman algorithm for
* polygon clipping.
*
* The algorithm for calculating the post-crash-dynamics is based on conservation of momentum
* and angular momentum. It works in 2D and is only valid for non-sliding collisions.
*
* Note that this module is provisional regarding several aspect. openPASS needs to be
* changed in order to improve the following:
* - The post-crash dynamics is currently only calculated and written to the log-file since
*   openPASS does not allow to forward the results to dynamic-components and
*   observation modules, currently.
* - This module needs several parameters (coeffRest, interFriction and timeOfPenetration).
*   openPASS currently does not allow to set parameters for this module in the config files.
*   Therefore, these parameters are hard-coded.
* - The calculation of the post-crash dynamics is based on the assumptions that the time
*   where the collision starts is known since the post-crash dynamics shall be calculated a
*   certain time ( given by the parameter timeOfPenetration ) after the first contact of the
*   vehicles.
*   Currently, this module does not know anything about sample rates etc. which makes it
*   difficult to find the correct time to calculate the post-crash dynamics.
* - The algorithm for the post-crash dynamics uses the pre-crash yaw velocities of both agents.
*   Since the yaw velocity is not available for the agents currently, the yaw velocities are
*   set to zero here.
*
* Note:
* - distOnBorder is not calculated in this module and is set to -1 in the runResult.
* - The method GetVertexTimes assumes that the intersection of the two vehicles is rather small.
*   It has not been checked if the method covers all possible cases for bigger intersections which
*   are possible in openPASS depending on the cycle times.
*
* \see CollisionDetectionInterface
*
* @}
*/

/*!
 * \copydoc CollisionDetection_Impact
 * \ingroup CollisionDetection_Impact
 */
class CollisionDetection_Impact_Implementation : public CollisionDetectionInterface
{
public:
    CollisionDetection_Impact_Implementation(const CallbackInterface *callbacks)
        : callbacks(callbacks)
    {}
    CollisionDetection_Impact_Implementation(const CollisionDetection_Impact_Implementation &) = delete;
    CollisionDetection_Impact_Implementation(CollisionDetection_Impact_Implementation &&) = delete;
    CollisionDetection_Impact_Implementation &operator=(const CollisionDetection_Impact_Implementation
                                                        &) = delete;
    CollisionDetection_Impact_Implementation &operator=(CollisionDetection_Impact_Implementation &&) =
        delete;

    ~CollisionDetection_Impact_Implementation();

    /*!
     * \brief sets a map of agents for collision detection
     *
     * \param agents      map of agents
     */
    void SetAgents(const std::map<int, const AgentInterface *> &agents);

    /*!
     * \brief handles collisions
     * Checks if collisions have happened and if true, creates results for collisions.
     *
     * \param[out] runResult                reference to storage location
     * \param[out] isCollision              Flag if a collision happened
     * \return                              False if an error occurred, true otherwise
     */
    bool HandleCollisionsInAgents(RunResultInterface &runResult, bool &isCollision);

protected:
    /*!
     * \brief logs messages
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
     * \brief get corners of one agent
     * Agents are treated as rectangles. Agent corners are calculated from
     * agent geometry parameters, current position and current yaw angle.
     *
     * \param[in] agent                 agent reference
     * \return                          agent corners
     */
    std::vector<Common::Vector2d> GetAgentCorners(const AgentInterface *agent);

    /*!
     * \brief Calculates points of intersection of two rectangles
     * The intersection area of two rectangles is a polygon. This method calculates the corners
     * of the intersection polygon.
     *
     * The algorithm is based on the Sutherland-Hodgman-Algorith for polygon clipping and
     * MATLAB-code provided by Harald Kolk (TU Graz). It assumes the corners to be
     * ordered conter-clockwise.
     *
     * See also: http://stackoverflow.com/questions/13101288/intersection-of-two-convex-polygons
     *
     * \param agentCorner1[in]      corners of first agent
     * \param agentCorner2[in]      corners second agent
     * \return                      vector of of intersection points
     */
    std::vector<Common::Vector2d> CalculateAllIntersectionPoints(std::vector<Common::Vector2d> corners1,
                                                                 std::vector<Common::Vector2d> corners2);
    /*!
     * \brief Calculates intersection point of two vectors
     * The calculation is based on describing the vectors using the Hesse normal form.
     * \param n1[in]                    normal vector of 1st vector
     * \param n2[in]                    normal vector of 2nd vestor
     * \param d1[in]                    distance from point of origin of first vector
     * \param d2[in]                    distance from point of origin of second vector
     * \param intersectionPoint[out]    vector intersection point
     * \return                          flag indicating if calculation could be done correctly
     */
    bool GetIntersectionPoint(Common::Vector2d n1, Common::Vector2d n2, double d1, double d2,
                              Common::Vector2d &intersectionPoint);

    /*!
     * \brief Calculates contact plane of two colliding agents
     *
     * PC-Crash method for setting the contact plane. Simply sets the plane as
     * line through two nodes of vertex type = 2, moved parallel to the centroid
     * of the intersection poygon.
     *
     * The contact plane is described by the point of impact (beeing
     * the centroid of the intersection polygon) and the angle of the contact plane
     * (line in 2D).
     *
     * The algorithm is based on MATLAB-code provided by Harald Kolk (TU Graz).
     *
     * \param[in] intersection      intersection polygon of the two colliding agent
     * \param[in] vertexTypes       vector of vertex types (see GetVertexTypes)
     * \param[out] pointOfImpact    Centroid of the intersecting polygon
     * \param[out] phi              Angle of the contact plane
     * \return                      flag indicating if contact plane could be calculated correctly
     */
    bool CalculatePlaneOfContact(Polygon intersection,
                                 std::vector<int> vertexTypes,
                                 Common::Vector2d &pointOfImpact,
                                 double &phi);
    /*!
     * \brief Calculate post-crash dynamics
     * The post-crash dynamics is calculated based on conservation of momentum
     * and angular momentum. It works in 2D and is only valid for non-sliding collisions.
     *
     * The algorithm is based on MATLAB-code provided by Harald Kolk (TU Graz).
     *
     * \param[in] cog1                      center of gravity of first agent
     * \param[in] agent1                    first agent
     * \param[in] cog2                      center of gravity of second agent
     * \param[in] agent2                    second agent
     * \param[in] poi                       point of impact (2d-vector)
     * \param[in] phi                       angle of contact plane [rad]
     * \param[out] postCrashDynamic1        PostCrashDynamic of first agent
     * \param[out] postCrashDynamic2        PostCrashDynamic of second agent
     * \return                              true if calculation was successful
     */
    bool CalculatePostCrashDynamic(Common::Vector2d cog1, const AgentInterface *agent1,
                                   Common::Vector2d cog2, const AgentInterface *agent2,
                                   Common::Vector2d poi,
                                   double phi,
                                   PostCrashDynamic *&postCrashDynamic1,
                                   PostCrashDynamic *&postCrashDynamic2);

    /*!
     * \brief calculates time of first constact
     * The time of the first contact of two colliding agents is estimated.
     * It is assumed that the velocity vectors between the time of first contact and
     * the current time (where a collision has been detected) are constant.
     * Yaw velocities are not taken into account since they are not provided by openPASS currently.
     *
     * The time is given relative to the current time. The time at this timestep is 0, so the
     * time in the past will be negative.
     *
     * \param[in] agent1                 first agent to consider
     * \param[in] agent2                 other agent to consider
     * \param[out] firstTimeContact      time of first contact --> negative since in the past
     * \return                           flag indicating if calculation could be done correctly
     */
    bool GetTimeOfFirstContact(const AgentInterface *agent1, const AgentInterface *agent2,
                               int &timeFirstContact);

    /*!
     * \brief Calculates vertex types of intersection polygon
     *
     * Vertex types:
     * 1: vertex can be found among the vertices of the intersecting polyons.
     * 2: vertex is the intersection of two edges of the two original polygons.
     * 3: Special case for perfectly straight impacts with parallel intersecting edges.
     *
     * Note:
     * This algorithm assumes that the intersection of the two vehicles is rather small. It has not
     * been checked if it covers all possible cases for bigger intersections which are possible in
     * openPASS depending on the cycle times.
     *
     * The algorithm is based on MATLAB-code provided by Harald Kolk (TU Graz).
     *
     * \param[in] vertices1              vertices of rectangle of agent1
     * \param[in] vertices2              vertices of rectangle of agent2
     * \param[in] verticesIntersection   vertices of intersection polygon
     * \return                           vertex types of vertices of intersection polygon
     */
    std::vector<int> GetVertexTypes(std::vector<Common::Vector2d> vertices1,
                                    std::vector<Common::Vector2d> vertices2,
                                    std::vector<Common::Vector2d> verticesIntersection);

    /*!
     * \brief Get vector of the agent velocity
     * Retrieve the vector of the velocity of an agent in the global coordinate system.
     * \param[in] agent                 agent
     * \return                          vector (x,y) of the velocity of the agent
     */
    Common::Vector2d GetAgentVelocityVector(const AgentInterface *agent);

    /*!
     * \brief Logs data of the post-crash dynamics to the log-file
     * Writes the data of a PostCrashDynamic object in the log file.
     *
     * Note: this is a workaround since there is currently no way to write it to the results-file.
     *
     * \param[in] postCrashDynamic      PostCrashDynamic Object which shall be logged
     * \param[in] id                    agent id
     */
    void LogPostCrashDynamic(PostCrashDynamic *postCrashDynamic, int id);

    /*!
     * \brief shifts 2 polygons and checks if the shifted polygons are intersecting
     * Shift two polygons along their shift vectors and check if they are intersecting each other.
     *
     * \param[in] polygon1                  first polygon
     * \param[in] polygon2                  second polygon
     * \param[in] shiftVector1              shift vector of first polygon
     * \param[in] shiftVector2              shift vector of seond polygon
     * \return                              true if they are intersecting each other after shifting, false otherwise
     */
    bool ShiftPolygonsAndCheckIntersection(Polygon polygon1, Polygon polygon2,
                                           Common::Vector2d shiftVector1, Common::Vector2d shiftVector2);

    /*!
     * \brief Get geometry data at collision time
     * Calculate the geometry parameters of the collision of two agents at a time
     * (time of first contact + penetration time).
     * Geometry data comprise
     * - center of gravity of both agents
     * - paramters describing the contact plane (point of impact and angle)
     *
     * \param[in]  agent1                   first agent
     * \param[in]  agent2                   second agent
     * \param[out] cog1                     center of gravity of first agent
     * \param[out] cog2                     center of gravity of second agent
     * \param[out] pointOfImpact            vector of the point of impact
     * \param[out] phi                      angle of the contact plane
     * \return                              true if calculation was successful
     */
    bool GetCollisionPosition(const AgentInterface *agent1, const AgentInterface *agent2,
                              Common::Vector2d &cog1, Common::Vector2d &cog2, Common::Vector2d &pointOfImpact,
                              double &phi);

    /*!
     * \brief Calculate and store collision data
     * Calculates collision time and post-crash dynamics and stores results.
     *
     * \param[in]  agent1                  1st agent
     * \param[in]  agent2                  2nd agent
     * \param[out] postCrashDynamic1       postCrashDynamic for first agent
     * \param[out] postCrashDynamic2       postCrashDynamic for second agent
     * \param[out] runResult               runResult object
     * \return                             true if result is created successfully
     */
    bool CreateResult(const AgentInterface *agent1,
                      const AgentInterface *agent2,
                      PostCrashDynamic *&postCrashDynamic1,
                      PostCrashDynamic *&postCrashDynamic2,
                      RunResultInterface &runResult);

    /*!
     * \brief Store the postCrashDynamic data
     *
     * \param[in] agentId                  id of agent
     * \param[in] postCrashDynamic         postCrashDynamic data to store
     */
    void AddPostCrashData(int agentId, PostCrashDynamic *postCrashDynamic);

    const std::map<int, const AgentInterface *> *agents = nullptr;
    const CallbackInterface *callbacks;

    //! map to store all postCrashDynamic data of all agents
    std::map<int, std::vector<PostCrashDynamic *>> postCrashData;

    /** \addtogroup CollisionDetection_Impact
     *  @{
     *      \name Internal Parameter
     *      Parameter which are set internally.\n
     *      (Parameters for model of post-crash dynamics)
     *      @{
     */
    double coeffRest = 0.3;      //!< coefficient of restitution [1]
    double interFriction = 0.3;  //!< Inter-vehicle friction [1]
    /**
     *      @}
     *  @}
     */
};

#endif // COLLISIONDETECTION_SHALGORITHM_IMPLEMENTATION_H
