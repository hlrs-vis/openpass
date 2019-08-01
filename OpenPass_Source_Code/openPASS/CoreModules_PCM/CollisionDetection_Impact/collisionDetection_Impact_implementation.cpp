/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <cassert>
#include <limits>
#include <array>

#include "collisionDetection_Impact_implementation.h"

namespace {

// counter-clockwise for Sutherland-Hodgman Algorithm
typedef enum {
    UpperLeft = 0,
    LowerLeft,
    LowerRight,
    UpperRight,
    NumberCorners
} CornerType;

} // namespace

CollisionDetection_Impact_Implementation::~CollisionDetection_Impact_Implementation()
{
    for (auto postCrashVectorIterator : postCrashData) {
        std::vector<PostCrashDynamic *> postCrashVector = postCrashVectorIterator.second;
        for (auto *postCrashDynamic : postCrashVector) {
            delete postCrashDynamic;
        }
    }
    postCrashData.clear();
}

void CollisionDetection_Impact_Implementation::SetAgents(const std::map<int, const AgentInterface *>
                                                         &agents)
{
    this->agents = &agents;
}

std::vector<Common::Vector2d> CollisionDetection_Impact_Implementation::GetAgentCorners(
    const AgentInterface *agent)
{
    Common::Vector2d agentPosition(agent->GetPositionX(), agent->GetPositionY());

    double agentLength = agent->GetLength();
    double agentWidthHalf = agent->GetWidth() / 2;
    double agentDistanceCenter = agent->GetDistanceCOGtoLeadingEdge();
    double agentAngle = agent->GetYawAngle();

    std::vector<Common::Vector2d> resultCorners;
    for (int i = 0; i < NumberCorners; ++i) {
        resultCorners.push_back(Common::Vector2d(0, 0));
    }

    // upper left corner for angle == 0
    resultCorners[UpperLeft].x = agentDistanceCenter - agentLength;
    resultCorners[UpperLeft].y = agentWidthHalf;

    // upper right corner for angle == 0
    resultCorners[UpperRight].x = agentDistanceCenter;
    resultCorners[UpperRight].y = agentWidthHalf;

    // lower right corner for angle == 0
    resultCorners[LowerRight].x = agentDistanceCenter;
    resultCorners[LowerRight].y = -agentWidthHalf;

    // lower left corner for angle == 0
    resultCorners[LowerLeft].x = agentDistanceCenter - agentLength;
    resultCorners[LowerLeft].y = -agentWidthHalf;

    // transform corners with actual angle and translate to position
    for (int i = 0; i < NumberCorners; ++i) {
        resultCorners[i].Rotate(agentAngle);
        resultCorners[i].Translate(agentPosition.x, agentPosition.y);
    }

    return resultCorners;
}

std::vector<Common::Vector2d>
CollisionDetection_Impact_Implementation::CalculateAllIntersectionPoints(
    std::vector<Common::Vector2d> corners1,
    std::vector<Common::Vector2d> corners2)
{
    // Sutherland-Hodgman-Algorith for polygon clipping
    for (unsigned int i1 = 0; i1 < corners1.size(); i1++) { // loop over 1st polygon, must be convex

        // resulting polygon
        std::vector<Common::Vector2d> cornersIntersection;
        // indices of neighboring corners of polygon1
        int i1_1 = i1;                                 // 1st polygon, 1st corner
        int i1_2 = ( i1 + 1 ) % corners1.size();       // 1st polygon, 2nd corner

        // calculate normalized normal on polygon edge and distance for Hesse normal form
        Common::Vector2d n1;
        n1.x = corners1[i1_2].y - corners1[i1_1].y;
        n1.y = - ( corners1[i1_2].x - corners1[i1_1].x );
        n1.Norm();
        double d1 = n1.Dot( corners1[i1_1] );

        for (unsigned int i2 = 0; i2 < corners2.size(); i2++) { // loop over 2nd polygon
            int i2_1 = i2;                            // 2nd polygon, 1st corner
            int i2_2 = ( i2 + 1 ) % corners2.size();  // 2nd polygon, 2nd corner

            // calculate normalized normal on polygon edge and distance for Hesse normal form
            Common::Vector2d n2;
            n2.x = corners2[i2_2].y - corners2[i2_1].y;
            n2.y = - ( corners2[i2_2].x - corners2[i2_1].x );
            n2.Norm();
            double d2 = n2.Dot( corners2[i2_1] );

            // check if edges are parallel
            bool areParallel     = fabs( n1.x - n2.x ) < std::numeric_limits<double>::epsilon()
                                   && fabs( n1.y - n2.y ) < std::numeric_limits<double>::epsilon();
            bool areAntiParallel = fabs( n1.x + n2.x ) < std::numeric_limits<double>::epsilon()
                                   && fabs( n1.y + n2.y ) < std::numeric_limits<double>::epsilon();
            if ( areParallel ||  areAntiParallel) {
                if ( n1.Dot( corners2[i2_1]) <= d1 ) {
                    // first point of the edge of polygon2 is inside the edge of polygon1
                    // => add first point of polygon2
                    cornersIntersection.push_back( corners2[i2_1] );
                    continue;
                }
            }

            Common::Vector2d intersectionPoint;
            GetIntersectionPoint(n1, n2, d1, d2, intersectionPoint);

            double dist1, dist2;
            dist1 = n1.Dot( corners2[i2_1] ) - d1;
            dist2 = n1.Dot( corners2[i2_2] ) - d1;
            if (dist1 <= 0 && dist2 > 0 ) {
                // inside -> outside
                cornersIntersection.push_back( corners2[i2_1] );
                cornersIntersection.push_back( intersectionPoint );
            } else if (dist1 > 0 && dist2 > 0) {
                // outside -> outside
                continue;
            } else if ( dist1 > 0 && dist2 <= 0) {
                // outside -> inside
                cornersIntersection.push_back( intersectionPoint );
            } else {
                // inside -> inside
                cornersIntersection.push_back( corners2[i2_1] );
            }
        }
        corners2 = cornersIntersection;
    }

    return corners2;
}

bool CollisionDetection_Impact_Implementation::CalculatePlaneOfContact(Polygon intersection,
                                                                       std::vector<int> vertexTypes, Common::Vector2d &pointOfImpact, double &phi)
{
    if (!intersection.CalculateCentroid(pointOfImpact)) {
        return false;
    }

    if (intersection.GetNumberOfVertices() != vertexTypes.size()) {
        return false;
    }

    std::vector<double> i2; // indices of vertices with vertexType = 2
    std::vector<double> i3; // indices of vertices with vertexType = 3

    for ( int iV = 0; iV < intersection.GetNumberOfVertices(); iV++ ) {
        if ( vertexTypes[iV] == 2 ) {
            i2.push_back( iV );
        } else if ( vertexTypes[iV] == 3 ) {
            i3.push_back( iV );
        }
    }

    std::vector<Common::Vector2d> vertices = intersection.GetVertices();
    Common::Vector2d vecPlane;
    if ( i2.size() == 2 ) {
        vecPlane = vertices[ i2[0] ] - ( vertices[ i2[1] ]);
    } else if ( i3.size() > 1 ) {
        vecPlane = vertices[ i3[0] ] - ( vertices[ i3[1] ]);
    } else {
        return false;
    }

    if ( vecPlane.x >= 0 ) {
        phi = atan( vecPlane.y / vecPlane.x );
    } else if ( vecPlane.x < 0 && vecPlane.y >= 0 ) {
        phi = atan( vecPlane.y / vecPlane.x ) + M_PI;
    } else if ( vecPlane.x < 0 && vecPlane.y < 0 ) {
        phi = atan( vecPlane.y / vecPlane.x ) - M_PI;
    }
    if ( phi < 0 ) {
        phi = phi + M_PI;
    }

    return true;
}

bool CollisionDetection_Impact_Implementation::CalculatePostCrashDynamic(Common::Vector2d cog1,
                                                                         const AgentInterface *agent1,
                                                                         Common::Vector2d cog2, const AgentInterface *agent2,
                                                                         Common::Vector2d poi, double phi,
                                                                         PostCrashDynamic *&postCrashDynamic1, PostCrashDynamic *&postCrashDynamic2)
{
    // input parameters of agent 1
    double yaw1 = agent1->GetYawAngle();                         // yaw angle [rad]
    double yawVel1 = 0;                                          // pre crash yaw velocity [rad/s]
    double velX1 =
        agent1->GetVelocityX();                       // x-velocity in the local coordinate system of the agent
    double velY1 =
        agent1->GetVelocityY();                       // y-velocity in the local coordinate system of the agent
    double vel1  = sqrt( velX1 * velX1 + velY1 *
                         velY1 );        // absolute velocity of first vehicle [m/s]
    double velDir1 =
        yaw1;                                       // velocity direction of first vehicle [rad]
    double mass1 = agent1->GetWeight();                          // mass of first vehicle [kg]
    double momentIntertiaYaw1 =
        agent1->GetMomentInertiaYaw();   // moment of inertia 1st vehicle [km*m^2]


    // input parameters of agent 2
    double yaw2 = agent2->GetYawAngle();                         // yaw angle [rad]
    double yawVel2 = 0;                                          // pre crash yaw velocity [rad/s]
    double velX2 =
        agent2->GetVelocityX();                       // x-velocity in the local coordinate system of the agent
    double velY2 =
        agent2->GetVelocityY();                       // y-velocity in the local coordinate system of the agent
    double vel2  = sqrt( velX2 * velX2 + velY2 *
                         velY2 );        // absolute velocity of 2nd vehicle [m/s]
    double velDir2 =
        yaw2;                                       // velocity direction of 2nd vehicle [rad]
    double mass2 = agent2->GetWeight();                          // mass of 2nd vehicle [kg]
    double momentIntertiaYaw2 =
        agent2->GetMomentInertiaYaw();   // moment of inertia 2nd vehicle [km*m^2]

    // new coordinate system axis: tangent and normal to contact plane
    Common::Vector2d tang = Common::Vector2d( cos(phi), sin(phi) );
    Common::Vector2d normal = Common::Vector2d( -sin(phi), cos(phi) );
    // distance of centers of gravity from point of impact in normal and
    // tangential direction: coordinates in new coordinate system
    double n1 = normal.x * cog1.x + normal.y * cog1.y - normal.x * poi.x - normal.y * poi.y;
    double n2 = normal.x * cog2.x + normal.y * cog2.y - normal.x * poi.x - normal.y * poi.y;
    double t1 = tang.x * cog1.x + tang.y * cog1.y - tang.x * poi.x - tang.y * poi.y;
    double t2 = tang.x * cog2.x + tang.y * cog2.y - tang.x * poi.x - tang.y * poi.y;
    // pre crash velocity vector, global coordinate system
    Common::Vector2d v1 = Common::Vector2d( vel1 * cos(velDir1) , vel1 * sin(velDir1) );
    Common::Vector2d v2 = Common::Vector2d( vel2 * cos(velDir2) , vel2 * sin(velDir2) );

    // COG-velocity in normal direction
    double v1norm_cog = normal.x * v1.x + normal.y * v1.y;
    double v2norm_cog = normal.x * v2.x + normal.y * v2.y;
    // velocity of point of impact in normal direction
    double v1norm = v1norm_cog - yawVel1 * t1;
    double v2norm = v2norm_cog - yawVel2 * t2;
    // relative velocity pre crash in normal direction
    double vrel_pre_norm = v1norm - v2norm;

    // COG-velocity in tangential direction
    double v1tang_cog = tang.x * v1.x + tang.y * v1.y;
    double v2tang_cog = tang.x * v2.x + tang.y * v2.y;
    // velocity of point of impact in tangential direction
    double v1tang = v1tang_cog + yawVel1 * n1;
    double v2tang = v2tang_cog + yawVel2 * n2;
    // relative velocity pre crash in tangential direction
    double vrel_pre_tang = v1tang - v2tang;

    // auxiliary parameters
    double c1 = 1 / mass1 + 1 / mass2 + n1 * n1 / momentIntertiaYaw1 + n2 * n2 / momentIntertiaYaw2;
    double c2 = 1 / mass1 + 1 / mass2 + t1 * t1 / momentIntertiaYaw1 + t2 * t2 / momentIntertiaYaw2;
    double c3 = t1 * n1 / momentIntertiaYaw1 + t2 * n2 / momentIntertiaYaw2;
    // compute pulse vector in compression phase
    double Tc = ( c3 * vrel_pre_norm + c2 * vrel_pre_tang ) / ( c3 * c3 - c1 * c2 );
    double Nc = ( c1 * vrel_pre_norm + c3 * vrel_pre_tang ) / ( c3 * c3 - c1 * c2 );

    bool out1_sliding = false;
    bool out2_sliding = false;
    if ( fabs(Tc) > fabs( interFriction * Nc ) ) {
        out1_sliding = true;
        out2_sliding = true;
        LOG(CbkLogLevel::Warning,
            "Sliding collision detected. Calculation of post-crash dynamics not valid for sliding collisions.");
    }
    // vector total pulse
    double T = Tc * ( 1 + coeffRest );
    double N = Nc * ( 1 + coeffRest );

    // relative post crash velocities: should be (very close to) ZERO! Just a check
    double vrel_post_tang = vrel_pre_tang + c1 * Tc - c3 * Nc;
    double vrel_post_norm = vrel_pre_norm - c3 * Tc + c2 * Nc;

    if ((fabs(vrel_post_norm) > 1E-3) || (fabs(vrel_post_tang) > 1E-3)) {
        std::stringstream msg;
        msg << "PostCrasDynamic Check: "
            << "Relative post crash velocities are too high: "
            << "tang: "
            << vrel_post_tang
            << "norm: "
            << vrel_post_norm;
        LOG(CbkLogLevel::Warning, msg.str());
    }

    // compute post crash velocities in COG with momentum balance equations
    double v1tang_post_cog =  T / mass1 + v1tang_cog;
    double v1norm_post_cog =  N / mass1 + v1norm_cog;
    double v2tang_post_cog = -T / mass2 + v2tang_cog;
    double v2norm_post_cog = -N / mass2 + v2norm_cog;

    // matrix for coordinate transformation between global and local
    // POI-coordinates
    // pulse vector in global system
    Common::Vector2d pulse = Common::Vector2d(T, N);
    pulse.Rotate(phi);
    // Pulse direction
    double out1_PulseDir = 0.0;
    if ( pulse.x >= 0 ) {
        out1_PulseDir = atan( pulse.y / pulse.x );
    } else if ( pulse.x < 0 && pulse.y >= 0 ) {
        out1_PulseDir = atan( pulse.y / pulse.x ) + M_PI;
    } else if ( pulse.x < 0 && pulse.y < 0 ) {
        out1_PulseDir = atan( pulse.y / pulse.x ) - M_PI;
    }
    double out2_PulseDir = out1_PulseDir;

    double pulseLength = pulse.Length();

    Common::Vector2d out1_PulseLocal = Common::Vector2d(  pulseLength * cos( out1_PulseDir - yaw1 ),
                                                          pulseLength * sin( out1_PulseDir - yaw1 ) );
    Common::Vector2d out2_PulseLocal = Common::Vector2d( -pulseLength * cos( out2_PulseDir - yaw2 ),
                                                         -pulseLength * sin( out2_PulseDir - yaw2 ) );

    // --- compute output for vehicle 1
    // post crash velocity vector
    Common::Vector2d v1_post = Common::Vector2d( v1tang_post_cog, v1norm_post_cog );
    v1_post.Rotate( phi );
    // absolute velocity
    double out1_Vel = v1_post.Length();
    // velocity change delta-v
    double out1_dV = (v1 - v1_post).Length();
    // velocity direction
    double out1_VelDir = v1_post.Angle( );
    // yaw velocity from angular momentum conservation equation
    double out1_YawVel = ( T * n1 - N * t1 ) / momentIntertiaYaw1 + yawVel1;
    Common::Vector2d  out1_Pulse = pulse;
    // rotation of poi from global to local
    Common::Vector2d out1_poiLocal = poi - cog1;
    out1_poiLocal.Rotate( -yaw1 );
    double out1_CollVel = vel1;


    // --- compute output for vehicle 2
    // post crash velocity vector
    Common::Vector2d v2_post = Common::Vector2d(v2tang_post_cog, v2norm_post_cog);
    v2_post.Rotate( phi );
    // absolute velocity
    double out2_Vel = v2_post.Length();
    // velocity change delta-v
    double out2_dV = (v2 - v2_post).Length();
    // velocity direction
    double out2_VelDir = v2_post.Angle();
    // yaw velocity from angular momentum conservation equation
    double out2_YawVel = ( -T * n2 + N * t2 ) / momentIntertiaYaw2 + yawVel2;
    Common::Vector2d out2_Pulse = pulse;
    // rotation of poi from global to local
    Common::Vector2d out2_poiLocal = poi - cog2;
    out2_poiLocal.Rotate( -yaw2 );
    double out2_CollVel = vel2;

    postCrashDynamic1 = new PostCrashDynamic(out1_Vel, out1_dV, out1_VelDir, out1_YawVel,
                                             out1_Pulse, out1_PulseDir, out1_PulseLocal,
                                             out1_poiLocal, out1_CollVel, out1_sliding );

    postCrashDynamic2 = new PostCrashDynamic(out2_Vel, out2_dV, out2_VelDir, out2_YawVel,
                                             out2_Pulse, out2_PulseDir, out2_PulseLocal,
                                             out2_poiLocal, out2_CollVel, out2_sliding );

    //for debug purpose
    LogPostCrashDynamic(postCrashDynamic1, agent1->GetAgentId());
    LogPostCrashDynamic(postCrashDynamic2, agent2->GetAgentId());

    return true; // Calculation successful
}

bool CollisionDetection_Impact_Implementation::GetIntersectionPoint(Common::Vector2d n1,
                                                                    Common::Vector2d n2, double d1, double d2, Common::Vector2d &intersectionPoint)
{
    double det = (n1.x * n2.y - n1.y * n2.x);

    if (fabs(det) < std::numeric_limits<double>::epsilon()) {
        return false;
    }

    intersectionPoint.x = (d1 * n2.y - d2 * n1.y) / det;

    intersectionPoint.y = (d2 * n1.x - d1 * n2.x) / det;

    return true;
}

bool CollisionDetection_Impact_Implementation::GetTimeOfFirstContact(const AgentInterface *agent1,
                                                                     const AgentInterface *agent2,
                                                                     int &timeFirstContact)
{
    std::vector<Common::Vector2d> agent1Corners = GetAgentCorners(agent1);
    std::vector<Common::Vector2d> agent2Corners = GetAgentCorners(agent2);

    Polygon agent1Polygon(agent1Corners);

    Polygon agent2Polygon(agent2Corners);

    Common::Vector2d agent1VelocityVector = GetAgentVelocityVector(agent1);
    Common::Vector2d agent2VelocityVector = GetAgentVelocityVector(agent2);

    int cycleTime = 100;       // assumption

    timeFirstContact = 0;      //start of range
    int lastTimeNoContact = 0; //end of range

    // Check if velocities are nearly same. If true, time of first contact will be very high
    // (infinity if velocities are exactly the same)
    if ((agent1VelocityVector - agent2VelocityVector).Length() < 1E-5) {
        return false;
    }

    //get last time with no contact --> calculate end of range
    bool intersected = true;
    while (intersected) {
        timeFirstContact = lastTimeNoContact;
        lastTimeNoContact -= cycleTime; // one cycleTime to the past --> negative
        intersected = ShiftPolygonsAndCheckIntersection(agent1Polygon, agent2Polygon,
                                                        agent1VelocityVector * (static_cast<double>(lastTimeNoContact) / 1000),
                                                        agent2VelocityVector * (static_cast<double>(lastTimeNoContact) / 1000));
    }

    //calculate first time of contact with range getting smaller
    while (std::abs(timeFirstContact - lastTimeNoContact) > 1) {
        int nextTime = lastTimeNoContact - (lastTimeNoContact - timeFirstContact) / 2;

        intersected = ShiftPolygonsAndCheckIntersection(agent1Polygon, agent2Polygon,
                                                        agent1VelocityVector * (static_cast<double>(nextTime) / 1000),
                                                        agent2VelocityVector * (static_cast<double>(nextTime) / 1000));

        if (intersected) {
            timeFirstContact = nextTime;

        } else {
            lastTimeNoContact = nextTime;
        }
    }

    return true;
}

std::vector<int> CollisionDetection_Impact_Implementation::GetVertexTypes(
    std::vector<Common::Vector2d> vertices1, std::vector<Common::Vector2d> vertices2,
    std::vector<Common::Vector2d> verticesIntersection)
{
    std::vector<int> vertexTypes;

    if ( verticesIntersection.size() == 0 ) {
        return vertexTypes;
    }

    int numberOfType2 = verticesIntersection.size();
    for (unsigned int iIntersection = 0; iIntersection < verticesIntersection.size(); ++iIntersection) {
        vertexTypes.push_back(2);

        for (unsigned int iPoly1 = 0; iPoly1 < vertices1.size(); ++iPoly1) {
            if ( (verticesIntersection[iIntersection] - vertices1[iPoly1]).Length() < 1E-12) {
                vertexTypes[iIntersection] = 1;
                numberOfType2--;
                continue;
            }
        }
        for (unsigned int iPoly2 = 0; iPoly2 < vertices1.size(); ++iPoly2) {
            if ( (verticesIntersection[iIntersection] - vertices2[iPoly2]).Length() < 1E-12) {
                vertexTypes[iIntersection] = 1;
                numberOfType2--;
                continue;
            }
        }
    }
    if ( numberOfType2 == 0 ) {
        // special case: perfectly straight impact
        for (unsigned int iIntersection = 0; iIntersection < verticesIntersection.size(); ++iIntersection) {
            for (unsigned int iPoly1 = 0; iPoly1 < vertices1.size(); ++iPoly1) {
                if ( (verticesIntersection[iIntersection] - vertices1[iPoly1]).Length() < 1E-12) {
                    vertexTypes[iIntersection] = 3;
                }
            }
        }
    }
    return vertexTypes;
}

Common::Vector2d CollisionDetection_Impact_Implementation::GetAgentVelocityVector(
    const AgentInterface *agent)
{
    Common::Vector2d agentVelocityX(agent->GetVelocityX(),
                                    0);   // x-velocity in the local coordinate system of the agent
    Common::Vector2d agentVelocityY(0,
                                    agent->GetVelocityY());   // y-velocity in the local coordinate system of the agent
    double agentYawAngle = agent->GetYawAngle();
    agentVelocityX.Rotate(agentYawAngle);
    agentVelocityY.Rotate(agentYawAngle);
    Common::Vector2d agentVelocity = agentVelocityX + agentVelocityY;

    return agentVelocity;
}

void CollisionDetection_Impact_Implementation::LogPostCrashDynamic(PostCrashDynamic
                                                                   *postCrashDynamic, int id)
{
    std::stringstream msg;
    msg     << "#PCD: id = " << id
            << " Velocity = " << postCrashDynamic->GetVelocity()
            << " VelocityChange = " << postCrashDynamic->GetVelocityChange()
            << " VelocityDirection = " << postCrashDynamic->GetVelocityDirection()
            << " YawVelocity = " << postCrashDynamic->GetYawVelocity()
            << " PulseX = " << postCrashDynamic->GetPulse().x
            << " PulseY = " << postCrashDynamic->GetPulse().y
            << " PulseDirection = " << postCrashDynamic->GetPulseDirection()
            << " PulseLocalX = " << postCrashDynamic->GetPulseLocal().x
            << " PulseLocalY = " << postCrashDynamic->GetPulseLocal().y
            << " PointOfContactLocalX = " << postCrashDynamic->GetPointOfContactLocal().x
            << " PointOfContactLocalY = " << postCrashDynamic->GetPointOfContactLocal().y
            << " CollisionVelocity = " << postCrashDynamic->GetCollisionVelocity()
            << " Sliding = " << postCrashDynamic->GetSliding();

    LOG(CbkLogLevel::Info, msg.str());
}

bool CollisionDetection_Impact_Implementation::ShiftPolygonsAndCheckIntersection(Polygon polygon1,
                                                                                 Polygon polygon2, Common::Vector2d shiftVector1, Common::Vector2d shiftVector2)
{
    polygon1.Translate(shiftVector1);
    polygon2.Translate(shiftVector2);

    std::vector<Common::Vector2d> intersectionPoints = CalculateAllIntersectionPoints(
                                                           polygon1.GetVertices(), polygon2.GetVertices());

    if (intersectionPoints.size() > 0) {
        return true;
    } else {
        return false;
    }
}

bool CollisionDetection_Impact_Implementation::GetCollisionPosition(const AgentInterface *agent1,
                                                                    const AgentInterface *agent2, Common::Vector2d &cog1, Common::Vector2d &cog2,
                                                                    Common::Vector2d &pointOfImpact, double &phi)
{
    std::vector<Common::Vector2d> agent1Corners;
    std::vector<Common::Vector2d> agent2Corners;

    agent1Corners = GetAgentCorners(agent1);
    agent2Corners = GetAgentCorners(agent2);

    Polygon agent1Polygon(agent1Corners);

    Polygon agent2Polygon(agent2Corners);

    agent1Polygon.CalculateCentroid(cog1);
    agent2Polygon.CalculateCentroid(cog2);

    std::vector<Common::Vector2d> intersectionPoints = CalculateAllIntersectionPoints(
                                                           agent1Polygon.GetVertices(), agent2Polygon.GetVertices());
    Polygon intersectionPolygon(intersectionPoints);

    std::vector<int> vertexTypes = GetVertexTypes(agent1Polygon.GetVertices(),
                                                  agent2Polygon.GetVertices(),
                                                  intersectionPoints);

    CalculatePlaneOfContact(intersectionPolygon, vertexTypes, pointOfImpact, phi);

    return true;
}

bool CollisionDetection_Impact_Implementation::CreateResult(const AgentInterface *agent1,
                                                            const AgentInterface *agent2,
                                                            PostCrashDynamic *&postCrashDynamic1,
                                                            PostCrashDynamic *&postCrashDynamic2,
                                                            RunResultInterface &runResult)
{
    //dummy, not calculated
    double resultAgent1DistOnBorder = -1;
    double resultAgent2DistOnBorder = -1;

    int timeOfFirstContact = 0;
    if (!GetTimeOfFirstContact(agent1, agent2, timeOfFirstContact)) {
        LOG(CbkLogLevel::Error, "Could not calculate time of first contact.");
        return false;
    }

    Common::Vector2d resultAgent1COG = Common::Vector2d(-1, -1);
    Common::Vector2d resultAgent2COG = Common::Vector2d(-1, -1);
    Common::Vector2d pointOfImpact;
    double phi;

    if (!GetCollisionPosition(agent1, agent2, resultAgent1COG, resultAgent2COG, pointOfImpact, phi)) {
        LOG(CbkLogLevel::Error, "Could not get collision position parameters.");
        return false;
    }

    if (!CalculatePostCrashDynamic(resultAgent1COG, agent1,
                                   resultAgent2COG, agent2,
                                   pointOfImpact, phi,
                                   postCrashDynamic1, postCrashDynamic2)) {
        LOG(CbkLogLevel::Error, "Could not calculate post crash dynamic");
        return false;
    }

    runResult.SetCollision();

    if (!runResult.AddCollision(agent1,
                                resultAgent1COG,
                                resultAgent1DistOnBorder,
                                agent2,
                                resultAgent2COG,
                                resultAgent2DistOnBorder)) {
        LOG(CbkLogLevel::Error, "Could not store collision result.");
        return false;
    }

    return true;
}

void CollisionDetection_Impact_Implementation::AddPostCrashData(int agentId,
                                                                PostCrashDynamic *postCrashDynamic)
{
    std::map<int, std::vector<PostCrashDynamic *>>::iterator postCrashIterator
                                                = postCrashData.find(agentId);
    if (postCrashIterator != postCrashData.end()) {
        std::vector<PostCrashDynamic *> postCrashDataVector = postCrashIterator->second;
        postCrashDataVector.push_back(postCrashDynamic);
    } else {
        std::vector<PostCrashDynamic *> postCrashDataVector;
        postCrashDataVector.push_back(postCrashDynamic);
        postCrashData.emplace(std::make_pair(agentId, postCrashDataVector));
    }
}

bool CollisionDetection_Impact_Implementation::HandleCollisionsInAgents(
    RunResultInterface &runResult,
    bool &isCollision)
{
    if (!agents) {
        LOG(CbkLogLevel::Warning, "collision detection not initialized");
        return false;
    }

    // reset collision flags
    isCollision = false;


    // accumulate collisions
    for (auto it = agents->cbegin(); it != agents->cend(); ++it) {
        const AgentInterface *agent1 = it->second;
        assert(agent1 != nullptr);

        for (auto otherIt = std::next(it); otherIt != agents->cend(); ++otherIt) {
            const AgentInterface *agent2 = otherIt->second;
            if (!agent2) {
                LOG(CbkLogLevel::Warning, "collision detection aborted");
                return false;
            }

            // quick check
            std::vector<Common::Vector2d> agent1Corners;
            std::vector<Common::Vector2d> agent2Corners;

            agent1Corners = GetAgentCorners(agent1);
            agent2Corners = GetAgentCorners(agent2);

            double quickDistance = agent1->GetLength() + agent1->GetWidth() + agent2->GetLength() +
                                   agent2->GetWidth();

            if (fabs(agent1Corners[UpperLeft].x - agent2Corners[UpperLeft].x) > quickDistance ||
                    fabs(agent1Corners[UpperLeft].y - agent2Corners[UpperLeft].y) > quickDistance) {
                continue;
            }

            // get collision intersection points
            std::vector<Common::Vector2d> intersectionPoints = CalculateAllIntersectionPoints(agent1Corners,
                                                                                              agent2Corners);

            // in case of collision
            if (intersectionPoints.size() > 0) {
                isCollision = true;

                PostCrashDynamic *postCrashDynamic1 = nullptr;
                PostCrashDynamic *postCrashDynamic2 = nullptr;
                if (!CreateResult(agent1, agent2, postCrashDynamic1,
                                  postCrashDynamic2, runResult)) {
                    LOG(CbkLogLevel::Error, "Could not create run result.");
                    return false;
                }

                AddPostCrashData(agent1->GetAgentId(), postCrashDynamic1);
                AddPostCrashData(agent2->GetAgentId(), postCrashDynamic2);

                const_cast<AgentInterface *>(agent1)->UpdateCollision(agent2->GetAgentId(),
                                                                      POSTCRASHDYNAMICID,
                                                                      postCrashDynamic1);
                const_cast<AgentInterface *>(agent2)->UpdateCollision(agent1->GetAgentId(),
                                                                      POSTCRASHDYNAMICID,
                                                                      postCrashDynamic2);
            }
        }
    }
    return true;
}
