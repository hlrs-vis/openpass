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
//! @file  AgentInterface.h
//! @brief This file contains the interface for communicating between framework
//!        and world.
//-----------------------------------------------------------------------------

#ifndef AGENTINTERFACE_H
#define AGENTINTERFACE_H

#include <map>
#include <list>
#include <vector>

#include "globalDefinitions.h"
#include "spawnPointInterface.h"
#include "signalInterface.h"
#include "carInfo.h"

/**
* \brief Agent Interface within the openPASS framework.
* \details This interface provides access to agent parameters, properties, attributes and dynamic states.
*/
class AgentInterface
{
public:
    AgentInterface() = default;
    AgentInterface(const AgentInterface &) = delete;
    AgentInterface(AgentInterface &&) = delete;
    AgentInterface &operator=(const AgentInterface &) = delete;
    AgentInterface &operator=(AgentInterface &&) = delete;
    virtual ~AgentInterface() = default;

    //-----------------------------------------------------------------------------
    //! Retrieves id of agent
    //!
    // @return                Id of agent
    //-----------------------------------------------------------------------------
    virtual int GetAgentId() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves time of spawn event of this agent
    //!
    //! @return                Spawn time
    //-----------------------------------------------------------------------------
    virtual int GetSpawnTime() const = 0;
    //! Retrieves type of vehicle of agent
    //!
    // @return                Type of vehicle
    //-----------------------------------------------------------------------------
    virtual AgentVehicleType GetVehicleType() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves x-coordinate of agent.
    //!
    // @return                X-coordinate
    //-----------------------------------------------------------------------------
    virtual double GetPositionX() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves y-coordinate of agent
    //!
    // @return                Y-coordinate
    //-----------------------------------------------------------------------------
    virtual double GetPositionY() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves width of agent boundary box
    //!
    // @return                Width of boundary box
    //-----------------------------------------------------------------------------
    virtual double GetWidth() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves length of agent boundary box
    //!
    // @return                Length of boundary box
    //-----------------------------------------------------------------------------
    virtual double GetLength() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves height of agent boundary box
    //!
    // @return                Height of boundary box
    //-----------------------------------------------------------------------------
    virtual double GetHeight() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves forward velocity of agent
    //!
    // @return                Forward velocity
    //-----------------------------------------------------------------------------
    virtual double GetVelocityX() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves sideward velocity of agent
    //!
    // @return                Sideward velocity
    //-----------------------------------------------------------------------------
    virtual double GetVelocityY() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves distance from COG to front axle of agent
    //!
    // @return                Distance from COG to front axle
    //-----------------------------------------------------------------------------
    virtual double GetDistanceCOGtoFrontAxle() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves weight of agent
    //!
    // @return                Weight of agent
    //-----------------------------------------------------------------------------
    virtual double GetWeight() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves distance from ground to COG of agent
    //!
    // @return                Distance from ground to COG
    //-----------------------------------------------------------------------------
    virtual double GetHeightCOG() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves distance between the centers of the front and rear wheels
    //!
    // @return                Distance between front and rear wheels
    //-----------------------------------------------------------------------------
    virtual double GetWheelbase() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves moment of inertia (roll axis)
    //!
    // @return                Moment of inertia for roll axis
    //-----------------------------------------------------------------------------
    virtual double GetMomentInertiaRoll() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves moment of inertia (pitch axis)
    //!
    // @return                Moment of inertia for pitch axis
    //-----------------------------------------------------------------------------
    virtual double GetMomentInertiaPitch() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves moment of inertia (yaw axis)
    //!
    // @return                Moment of inertia for yaw axis
    //-----------------------------------------------------------------------------
    virtual double GetMomentInertiaYaw() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves friction coefficient
    //!
    // @return                Friction coefficient
    //-----------------------------------------------------------------------------
    virtual double GetFrictionCoeff() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves distance between wheels on the same axle
    //!
    // @return                Distance between both front wheels (or both rear wheels)
    //-----------------------------------------------------------------------------
    virtual double GetTrackWidth() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the current gear number
    //!
    // @return                Gear no.
    //-----------------------------------------------------------------------------
    virtual int GetGear() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves distance from COG to leading edge
    //!
    // @return                Distance from COG to leading edge
    //-----------------------------------------------------------------------------
    virtual double GetDistanceCOGtoLeadingEdge() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves forward acceleration of agent
    //!
    // @return                Forward acceleration
    //-----------------------------------------------------------------------------
    virtual double GetAccelerationX() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves sideward acceleration of agent
    //!
    // @return                Sideward acceleration
    //-----------------------------------------------------------------------------
    virtual double GetAccelerationY() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves yaw angle of agent.
    //!
    // @return                Yaw angle
    //-----------------------------------------------------------------------------
    virtual double GetYawAngle() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves list of collisions partners of agent.
    //!
    // @return                a value >0 if collided
    //-----------------------------------------------------------------------------
    virtual std::vector<int> GetCollisionPartners() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves vector of all collisionData with a collisionPartner.
    //!
    // @return                a value >0 if collided
    //-----------------------------------------------------------------------------
    virtual std::vector<void *> GetCollisionData(int collisionPartnerId,
                                                 int collisionDataId) const = 0;

    //-----------------------------------------------------------------------------
    //! Sets x-coordinate of agent
    //!
    //! @param[in]     positionX    X-coordinate
    //-----------------------------------------------------------------------------
    virtual void SetPositionX(double positionX) = 0;

    //-----------------------------------------------------------------------------
    //! Sets y-coordinate of agent
    //!
    //! @param[in]     positionY    Y-coordinate
    //-----------------------------------------------------------------------------
    virtual void SetPositionY(double positionY) = 0;

    //-----------------------------------------------------------------------------
    //! Sets width of agents boundary box
    //!
    //! @param[in]     width    Width of agent
    //-----------------------------------------------------------------------------
    virtual void SetWidth(double width) = 0;

    //-----------------------------------------------------------------------------
    //! Sets length of agents boundary box
    //!
    //! @param[in]     length    Length of agent
    //-----------------------------------------------------------------------------
    virtual void SetLength(double length) = 0;

    //-----------------------------------------------------------------------------
    //! Sets height of agents boundary box
    //!
    //! @param[in]     height    Height of agent
    //-----------------------------------------------------------------------------
    virtual void SetHeight(double height) = 0;

    //-----------------------------------------------------------------------------
    //! Sets forward velocity of agent
    //!
    //! @param[in]     velocityX    Forward velocity
    //-----------------------------------------------------------------------------
    virtual void SetVelocityX(double velocityX) = 0;

    //-----------------------------------------------------------------------------
    //! Sets sideward velocity of agent
    //!
    //! @param[in]     velocityY    Sideward velocity
    //-----------------------------------------------------------------------------
    virtual void SetVelocityY(double velocityY) = 0;

    //-----------------------------------------------------------------------------
    //! Sets distance from COG to front axle of agent
    //!
    //! @param[in]     distanceCOGtoFrontAxle    distance from COG to front axle
    //-----------------------------------------------------------------------------
    virtual void SetDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle) = 0;

    //-----------------------------------------------------------------------------
    //! Sets gear of vehicle
    //!
    //! @param[in]     gear    current gear
    //-----------------------------------------------------------------------------
    virtual void SetGear(int gear) = 0;

    virtual void SetEngineSpeed(double engineSpeed) = 0;

    //-----------------------------------------------------------------------------
    //! Sets current position of acceleration pedal in percent
    //!
    //! @param[in]     percent    current percentage
    //-----------------------------------------------------------------------------
    virtual void SetEffAccelPedal(double percent) = 0;

    //-----------------------------------------------------------------------------
    //! Sets current position of brake pedal in percent
    //!
    //! @param[in]     percent    current percentage
    //-----------------------------------------------------------------------------
    virtual void SetEffBrakePedal(double percent) = 0;

    //-----------------------------------------------------------------------------
    //! Sets weight of agent
    //!
    //! @param[in]     weight    agents weight
    //-----------------------------------------------------------------------------
    virtual void SetWeight(double weight) = 0;

    //-----------------------------------------------------------------------------
    //! Sets distance from ground to COG of agent
    //!
    //! @param[in]     heightCOG    distance from ground to COG
    //-----------------------------------------------------------------------------
    virtual void SetHeightCOG(double heightCOG) = 0;

    //-----------------------------------------------------------------------------
    //! Sets distance between centers of front and rear wheels
    //!
    //! @param[in]     wheelbase    Distance between front and rear wheels.
    //-----------------------------------------------------------------------------
    virtual void SetWheelbase(double wheelbase) = 0;

    //-----------------------------------------------------------------------------
    //! Sets moment of inertia for roll axis
    //!
    //! @param[in]     momentInertiaRoll    moment of inertia for roll axis
    //-----------------------------------------------------------------------------
    virtual void SetMomentInertiaRoll(double momentInertiaRoll) = 0;

    //-----------------------------------------------------------------------------
    //! Sets moment of inertia for pitch axis
    //!
    //! @param[in]     momentInertiaPitch    moment of inertia for pitch axis
    //-----------------------------------------------------------------------------
    virtual void SetMomentInertiaPitch(double momentInertiaPitch) = 0;

    //-----------------------------------------------------------------------------
    //! Sets moment of inertia for yaw axis
    //!
    //! @param[in]     momentInertiaYaw    moment of inertia for yaw axis
    //-----------------------------------------------------------------------------
    virtual void SetMomentInertiaYaw(double momentInertiaYaw) = 0;

    //-----------------------------------------------------------------------------
    //! Sets maximum acceleration of the vehicle
    //!
    //! @param[in]     maxAcceleration   maximum acceleration
    //-----------------------------------------------------------------------------
    virtual void SetMaxAcceleration(double maxAcceleration) = 0;

    //-----------------------------------------------------------------------------
    //! Sets maximum deceleration of the vehicle
    //!
    //! @param[in]     maxDeceleration   maximum deceleration
    //-----------------------------------------------------------------------------
    virtual void SetMaxDeceleration(double maxDeceleration) = 0;

    //-----------------------------------------------------------------------------
    //! Sets friction coefficient
    //!
    //! @param[in]     frictionCoeff    friction coefficient
    //-----------------------------------------------------------------------------
    virtual void SetFrictionCoeff(double frictionCoeff) = 0;

    //-----------------------------------------------------------------------------
    //! Sets distance between wheels on same axle
    //!
    //! @param[in]     trackWidth    distance between both front wheels (or both
    //!                              rear wheels)
    //-----------------------------------------------------------------------------
    virtual void SetTrackWidth(double trackWidth) = 0;

    //-----------------------------------------------------------------------------
    //! Sets distance from COG to leading edge
    //!
    //! @param[in]     distanceCOGtoLeadingEdge    distance from COG to leading
    //!                                            edge
    //-----------------------------------------------------------------------------
    virtual void SetDistanceCOGtoLeadingEdge(double distanceCOGtoLeadingEdge) = 0;

    //-----------------------------------------------------------------------------
    //! Sets forward acceleration of agent
    //!
    //! @param[in]     accelerationX    forward acceleration
    //-----------------------------------------------------------------------------
    virtual void SetAccelerationX(double accelerationX) = 0;

    //-----------------------------------------------------------------------------
    //! Sets sideward acceleration of agent
    //!
    //! @param[in]     accelerationY    sideward acceleration
    //-----------------------------------------------------------------------------
    virtual void SetAccelerationY(double accelerationY) = 0;

    //-----------------------------------------------------------------------------
    //! Sets yaw angle of agent
    //!
    //! @param[in]     yawAngle    agent orientation
    //-----------------------------------------------------------------------------
    virtual void SetYawAngle(double yawAngle) = 0;

    //-----------------------------------------------------------------------------
    //! Requests removal of agents at next time step.
    //-----------------------------------------------------------------------------
    virtual void RemoveAgent() = 0;

    //-----------------------------------------------------------------------------
    //! update list with collision partners
    //!
    // @return
    //-----------------------------------------------------------------------------
    virtual void UpdateCollision(int collisionPartnerId) = 0;

    //-----------------------------------------------------------------------------
    //! update list with collision partners and corresponding collisionData
    //!
    // @return
    //-----------------------------------------------------------------------------
    virtual void UpdateCollision(int collisionPartnerId, int collisionDataId,
                                 void *collisionData) = 0;

    //-----------------------------------------------------------------------------
    //! Unlocate agent in world.
    //!
    // @return
    //-----------------------------------------------------------------------------
    virtual bool Unlocate() = 0;

    //-----------------------------------------------------------------------------
    //! Locate agent in world.
    //!
    // @return
    //-----------------------------------------------------------------------------
    virtual bool Locate() = 0;

    //-----------------------------------------------------------------------------
    //! Set the brake light on or off.
    //!
    // @param[in]     brakeLightStatus    status of brake light
    // @return
    //-----------------------------------------------------------------------------
    virtual void SetBrakeLight(bool brakeLightStatus) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the status of the brake light.
    //!
    // @return         true if brake light is on
    //-----------------------------------------------------------------------------
    virtual bool GetBrakeLight() const = 0;

    //-----------------------------------------------------------------------------
    //! Inits all physical and world specific parameters of an agent.
    //!
    // @return     true for success
    //-----------------------------------------------------------------------------
    virtual bool InitAgentParameter(int id,
                                    int agentTypeId,
                                    int spawnTime,
                                    const AgentSpawnItem *agentSpawnItem,
                                    const SpawnItemParameterInterface &spawnItemParameter) = 0;

    //-----------------------------------------------------------------------------
    //! Checks if an agent is still valid or if its marked for remove.
    //!
    // @return     true if still valid, false if marked for remove
    //-----------------------------------------------------------------------------
    virtual bool IsValid() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves type of agent
    //!
    //! @return                Id of agent type
    //-----------------------------------------------------------------------------
    virtual int GetAgentTypeId() const = 0;

    //! Sets to Inidcator in a specific state
    virtual void SetIndicatorState(IndicatorState indicatorState) = 0;

    //! Retrieve the state of the indicator
    virtual IndicatorState GetIndicatorState() = 0;

    //! Retrieve the lane ID of the agent.
    virtual int GetAgentLaneId() const = 0;

    //! Retrieve the lane ID left of the agent. Return INFINITY if there is no lane.
    virtual int GetAgentLaneIdLeft() = 0;

    //! Retrieve the lane ID right of the agent. Return INFINITY if there is no lane.
    virtual int GetAgentLaneIdRight() = 0;

    //! Retrieves the lane number of the agent.
    virtual int GetAgentLaneNumber() = 0;

    //-----------------------------------------------------------------------------
    //! Returns true if agent is still in World located.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool IsAgentInWorld() = 0;

    //! The Vehicle will be reset in queue.
    virtual void ReinitCarInQueue() = 0;

    //-----------------------------------------------------------------------------
    //! Returns true if agent is agent is at end of road or near the end.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool IsAgentAtEndOfRoad() = 0;

    //-----------------------------------------------------------------------------
    //! Sets the position of an agent to the start of the road.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void ReenterAgentAtStart() = 0;

    //-----------------------------------------------------------------------------
    //! Set the position of an agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetPosition(Position pos) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the start of the road.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToStartOfRoad() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns a Position of an agent calculated by the distance from start.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual Position GetPositionByDistance(double distance) const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the width of a lane an agent is on.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetLaneWidth() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the width of a lane left of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetLaneWidthLeft() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the width of a lane right of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetLaneWidthRight() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the curvature of a lane an agent is on.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetCurvature() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the curvature of a lane an agent is on in a distance.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetCurvatureInDistance(double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Returns true if Agent is marked as the special vehicle.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool IsSpecialAgent() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the distance to the next agent in front in a specific lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToFrontAgent(int laneId) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the distance to the next agent behind in a specific lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToRearAgent(int laneId) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the AgentInterface of the next agent in front in a specific lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetAgentInFront(int laneId) const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the AgentInterface of the next agent behind in a specific lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetAgentBehind(int laneId) const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the distance to another agent. (negative if other agent is behind)
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToAgent(AgentInterface *otherAgent) = 0;

    //-----------------------------------------------------------------------------
    //! Removes the marker of the agent which marks it special.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void RemoveSpecialAgentMarker() = 0;

    //-----------------------------------------------------------------------------
    //! Sets the marker of the agent which marks it special.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetSpecialAgentMarker() = 0;

    //-----------------------------------------------------------------------------
    //! Returns true if a left lane exists.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool ExistLaneLeft() = 0;

    //-----------------------------------------------------------------------------
    //! Returns true if a right lane exists.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool ExistLaneRight() = 0;

    //-----------------------------------------------------------------------------
    //! Sets the flag to mark the agent as obstacle.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetObstacleFlag() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the part-velocity acting perpendicular to the road direction
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetVelocityLateral() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve a pointer to an agentInterface of the next agents in front in sight.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void GetAgentsDirectlyInFront(double PeripheralPreviewDistance,
                                          AgentInterface *&agentFront,
                                          AgentInterface *&agentFrontLeft,
                                          AgentInterface *&agentFrontRight) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the special vehicle.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToSpecialAgent() = 0;

    //-----------------------------------------------------------------------------
    //! Return true if obstacle flag is set in agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool IsObstacle() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the distance to the end of the lane or the sightDistance, if end of
    //! lane is far away.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToEndOfLane(double sightDistance) const  = 0;

    //-----------------------------------------------------------------------------
    //! Perceive the miniumum speed of the platoon on the the left lane.
    //! Returns true if the speed is successfully perceived.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool PerceiveMinimumSpeedOfPlatoonInLaneLeft(double MesoscopicPreviewDistance,
                                                         int &iLane,
                                                         double &laneSpeedDifferential) const = 0;
    //-----------------------------------------------------------------------------
    //! Perceive the miniumum speed of the platoon on the the right lane.
    //! Returns true if the speed is successfully perceived.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool PerceiveMinimumSpeedOfPlatoonInLaneRight(double MesoscopicPreviewDistance,
                                                          int &iLane,
                                                          double &laneSpeedDifferential) const = 0;

    //-----------------------------------------------------------------------------
    //! Obtain basic information about surroundings.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void ObtainGroundTruthObjectLaneExistences(AreaOfInterest aoi,
                                                       AgentInterface *&agentAOI,
                                                       bool &hasRightLane,
                                                       bool &hasLeftLane,
                                                       double PreviewDistance,
                                                       double _carLengthEffective) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the lane departure from the left lane boundary.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetLaneDepartureFromLeftLaneBoundary() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the lane departure from the right lane boundary.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetLaneDepartureFromRightLaneBoundary() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the absolute velocity.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetVelocityAbsolute() const = 0;

    //-----------------------------------------------------------------------------
    //! Sets the internal CarInfo object.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetCarInfo(CarInfo *carInfo) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the internal CarInfo object.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual CarInfo *GetCarInfo() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the distance to the end of the ramp
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToEndOfRamp(int laneId) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the lateral position.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetPositionLateral() const = 0 ;

    //-----------------------------------------------------------------------------
    //! Sets internal extra information for car.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetCarInfoExtra(void *extraInfo) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the extra information of the car.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void *GetCarInfoExtra() = 0;

    //-----------------------------------------------------------------------------
    //! Sets a carInfo of the current car state with the acceleration sense distance.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void AssignCarInfo(double accSensDist) = 0;

    //-----------------------------------------------------------------------------
    //! Sets a carInfoExtra of the current car state.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void AssignCarInfoExtra() = 0;

    //-----------------------------------------------------------------------------
    //! Generates a CarInfo of current state of the car.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual CarInfo *GenerateCarInfo() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the distance of the front agent to ego.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceFrontAgentToEgo() = 0;

    //-----------------------------------------------------------------------------
    //! Checks whether the agent has two lanes on the left.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool HasTwoLeftLanes() = 0;

    //-----------------------------------------------------------------------------
    //! Checks whether the agent has two lanes on the right.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool HasTwoRightLanes() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve an estimated lane change state.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual LaneChangeState EstimateLaneChangeState(double thresholdLooming) = 0;

    //-----------------------------------------------------------------------------
    //! Get a list of all agents in a lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual std::list<AgentInterface *> GetAllAgentsInLane(int laneID,
                                                           double minDistance,
                                                           double maxDistance,
                                                           double AccSensDist) = 0;

    //-----------------------------------------------------------------------------
    //! Returns true if agent is a bicycle.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool IsBicycle() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the current direction angle of the lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetLaneDirection() const = 0;

    //-----------------------------------------------------------------------------
    //! Unregisters the agent from the world.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void Unregister() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns true if agent is the first car in lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool IsFirstCarInLane() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the type of the nearest mark.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual MarkType GetTypeOfNearestMark() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the type of the nearest mark.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual std::string GetTypeOfNearestMarkString() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest mark of specific markType (NONE for no
    //! specific or any markType).
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestMark(MarkType markType) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the relative angle of the nearest mark of specific markType (NONE for no
    //! specific or any markType)
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetOrientationOfNearestMark(MarkType markType) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the view direction to the nearest mark of specific markType (NONE for no
    //! specific or any markType). The direction is seen from the agents
    //! perspective in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetViewDirectionToNearestMark(MarkType markType) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the AgentViewDirection to the nearest mark of specific markType
    //!(NONE for no specific or any markType). The direction is seen from the
    //! agents perspective.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual AgentViewDirection GetAgentViewDirectionToNearestMark(MarkType markType) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific direction angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                           AgentViewDirection agentViewDirection) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific direction angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                           double mainViewDirection) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the relative angle of the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific direction angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                              AgentViewDirection agentViewDirection) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the relative angle of the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific direction angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                              double mainViewDirection) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestMarkInViewRange(MarkType markType,
                                                       AgentViewDirection agentViewDirection, double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                       double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the relative angle of the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                          AgentViewDirection agentViewDirection, double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the relative angle of the nearest mark of specific markType (NONE for no
    //! specific or any markType) in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetOrientationOfNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                          double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the concrete viewing direction to the nearest mark in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                            AgentViewDirection agentViewDirection, double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the concrete viewing direction to the nearest mark in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetViewDirectionToNearestMarkInViewRange(MarkType markType, double mainViewDirection,

                                                            double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the type of the nearest object in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual std::string GetTypeOfNearestObject(AgentViewDirection agentViewDirection,
                                               double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the type of the nearest object in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual std::string GetTypeOfNearestObject(double mainViewDirection,
                                               double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest object of specific objectType (NONE for no
    //! specific or any objectType) in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                         AgentViewDirection agentViewDirection,
                                                         double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest object of specific objectType (NONE for no
    //! specific or any objectType) in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                         double mainViewDirection,
                                                         double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the concrete viewing direction to the nearest object in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                              AgentViewDirection agentViewDirection,
                                                              double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the concrete viewing direction to the nearest object in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                              double mainViewDirection,
                                                              double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the Id of the nearest Agent in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual int GetIdOfNearestAgent(AgentViewDirection agentViewDirection,
                                    double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the Id of the nearest Agent in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual int GetIdOfNearestAgent(double mainViewDirection,
                                    double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest agent in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                        double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the distance to the nearest agent in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToNearestAgentInViewRange(double mainViewDirection,
                                                        double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the concrete viewing direction to the nearest agent in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetViewDirectionToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                             double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the concrete viewing direction to the nearest agent in a viewing range.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetViewDirectionToNearestAgentInViewRange(double mainViewDirection,
                                                             double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the visibility to an agent in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetVisibilityToNearestAgentInViewRange(double mainViewDirection,
                                                        double range) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the yaw velocity of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetYawVelocity() = 0;

    //-----------------------------------------------------------------------------
    //! Set the yaw velocity of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetYawVelocity(double yawVelocity) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the yaw acceleration of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetYawAcceleration() = 0;

    //-----------------------------------------------------------------------------
    //! Set the yaw acceleration of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetYawAcceleration(double yawAcceleration) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the trajectory of time of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const std::vector<int> *GetTrajectoryTime() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the trajectory of x position of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const std::vector<double> *GetTrajectoryXPos() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the trajectory of y position of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const std::vector<double> *GetTrajectoryYPos() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the trajectory of the velocity of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const std::vector<double> *GetTrajectoryVelocity() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the trajectory of the angle of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const std::vector<double> *GetTrajectoryAngle() const = 0;

    //-----------------------------------------------------------------------------
    //! Set the acceleration intention of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetAccelerationIntention(double accelerationIntention) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the acceleration intention of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetAccelerationIntention() const = 0;

    //-----------------------------------------------------------------------------
    //! Set the deceleration intention of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetDecelerationIntention(double decelerationIntention) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the deceleration intention of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetDecelerationIntention() const = 0;

    //-----------------------------------------------------------------------------
    //! Set the angle intention of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetAngleIntention(double angleIntention) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the angle intention of the agent.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetAngleIntention() const = 0;

    //-----------------------------------------------------------------------------
    //! Set the collision state of the agent. (collision occured or not)
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetCollisionState(bool collisionState) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the collision state of the agent. (collision occured or not)
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool GetCollisionState() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves absolute acceleration of agent
    //!
    // @return                Sideward acceleration
    //-----------------------------------------------------------------------------
    virtual double GetAccelerationAbsolute() const = 0;

    virtual RoadPosition GetRoadPosition() const = 0;

    /// \brief  Get distance w.r.t. the specified measurement point
    /// \return distance
    virtual double GetDistanceToStartOfRoad(MeasurementPoint mp) const = 0;

    virtual double GetDistanceReferencePointToLeadingEdge() const = 0;

    virtual double GetEngineSpeed() const = 0;

    //-----------------------------------------------------------------------------
    //! Gets current position of acceleration pedal in percent
    //!
    //! @return     accelPedal    current percentage
    //-----------------------------------------------------------------------------
    virtual double GetEffAccelPedal() const = 0;

    //-----------------------------------------------------------------------------
    //! Gets current position of brake pedal in percent
    //!
    //! @return     brakePedal    current percentage
    //-----------------------------------------------------------------------------
    virtual double GetEffBrakePedal() const = 0;

    virtual double GetMaxAcceleration() const = 0;
    virtual double GetMaxDeceleration() const = 0;

    virtual double GetLaneRemainder(Side) const = 0;
};

#endif // AGENTINTERFACE_H
