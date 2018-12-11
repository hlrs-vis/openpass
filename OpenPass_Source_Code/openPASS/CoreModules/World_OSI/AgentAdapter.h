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
//! @file  AgentAdapter.h
//! @brief This file implements the wrapper for the agent so it can
//!        interact with the world.
//!        It permits a simple implementation of delegation concepts
//!        (composition vs. inheritance).
//-----------------------------------------------------------------------------

#pragma once

#include <QtGlobal>
#include <functional>

#include "worldInterface.h"
#include "Localization.h"
#include "WorldData.h"
#include "WorldObjectAdapter.h"

/*!
 * \brief The AgentAdapter class
 * This class is a adapter for the communication between the framework and world.
 */
class AgentAdapter final : public AgentInterface
{
public:
    const std::string MODULENAME = "AGENTADAPTER";

    AgentAdapter(WorldInterface* world, const CallbackInterface* callbacks);
    AgentAdapter(const AgentAdapter&) = delete;
    AgentAdapter(AgentAdapter&&) = delete;
    AgentAdapter& operator=(const AgentAdapter&) = delete;
    AgentAdapter& operator=(AgentAdapter&&) = delete;
    ~AgentAdapter() override;

    bool InitAgentParameter(int id,
                            int agentTypeId,
                            int spawnTime,
                            const AgentSpawnItem *agentSpawnItem,
                            const SpawnItemParameterInterface &spawnItemParameter) override;

    void InitDrivingView();

//////////////////////////////////////////////////////////

    const OWL::MovingObject* GetBaseTrafficObject() const;

    AreaOfInterest GetAgentAOI() const;

    int GetAgentId() const override
    {
        return id;
    }

    double GetHeight() const override
    {
        return baseTrafficObject->GetDimension().height;
    }

    double GetLength() const override
    {
        return baseTrafficObject->GetDimension().length;
    }

    double GetPositionX() const override
    {
        return baseTrafficObject->GetAbsPosition().x;
    }

    double GetPositionY() const override
    {
        return baseTrafficObject->GetAbsPosition().y;
    }

    double GetWidth() const override
    {
        return baseTrafficObject->GetDimension().width;
    }

    double GetYawAngle() const override
    {
        return baseTrafficObject->GetAbsOrientation().yaw;
    }

    double GetVelocityAbsolute() const override
    {
        return baseTrafficObject->GetAbsVelocityDouble();
    }

    double GetVelocityLateral() override
    {
        // TODO
        return 0.0;
    }

    double GetVelocityX() const override
    {
        OWL::Primitive::AbsOrientation orientation = baseTrafficObject->GetAbsOrientation();
        Common::Vector2d orientationVec{1.0, 0};
        orientationVec.Rotate(orientation.yaw);

        OWL::Primitive::AbsVelocity velocity = baseTrafficObject->GetAbsVelocity();
        Common::Vector2d velocityVec{velocity.vx, velocity.vy};

        return velocityVec.Dot(orientationVec);
    }

    double GetVelocityY() const override
    {
        return 0.0;
    }

    double GetAccelerationAbsolute() const override
    {
        return baseTrafficObject->GetAbsAccelerationDouble();
    }

    double GetAccelerationX() const override
    {
        OWL::Primitive::AbsOrientation orientation = baseTrafficObject->GetAbsOrientation();
        Common::Vector2d orientationVec{1.0, 0};
        orientationVec.Rotate(orientation.yaw);

        OWL::Primitive::AbsAcceleration acceleration = baseTrafficObject->GetAbsAcceleration();
        Common::Vector2d accelerationVec{acceleration.ax, acceleration.ay};

        return accelerationVec.Dot(orientationVec);
    }

    double GetAccelerationY() const override
    {
        return 0.0;
    }

    double GetYawVelocity() override
    {
        return yawVelocity;
    }

    double GetYawAcceleration() override
    {
        return yawAcceleration;
    }


    double GetDistanceCOGtoFrontAxle() const override
    {
        return distanceCOGToFrontAxle;
    }

    double GetDistanceCOGtoLeadingEdge() const override
    {
        return distanceCOGToLeadingEdge;
    }

    double GetWeight() const override
    {
        return weight;
    }

    int GetSpawnTime() const override
    {
        return spawnTime;
    }

    double GetHeightCOG() const override
    {
        return heightCOG;
    }

    double GetWheelbase() const override
    {
        return wheelbase;
    }

    double GetMomentInertiaRoll() const override
    {
        return momentInertiaRoll;
    }

    double GetMomentInertiaPitch() const override
    {
        return momentInertiaPitch;
    }

    double GetMomentInertiaYaw() const override
    {
        return momentInertiaYaw;
    }

    double GetFrictionCoeff() const override
    {
        return frictionCoeff;
    }

    double GetTrackWidth() const override
    {
        return trackWidth;
    }

    double GetDistanceReferencePointToLeadingEdge() const override
    {
        return distanceReferencePointToLeadingEdge;
    }

    double GetEngineSpeed() const override
    {
        return engineSpeed;
    }

    int GetGear() const override
    {
        return currentGear;
    }

    double GetEffAccelPedal() const override
    {
        return accelPedal;
    }

    double GetEffBrakePedal() const override
    {
        return brakePedal;
    }

    double GetMaxAcceleration() const override
    {
        return maxAcceleration;
    }

    double GetMaxDeceleration() const override
    {
        return maxDeceleration;
    }

    RoadPosition GetRoadPosition() const override
    {
        return roadPos;
    }

    AgentVehicleType GetVehicleType() const override
    {
        return vehicleType;
    }

    void UpdateCollision(int collisionPartnerId) override;

    void UpdateCollision(int collisionPartnerId, int collisionDataId, void *collisionData) override;

    std::vector<void *> GetCollisionData(int collisionPartnerId, int collisionDataId) const override;

    std::vector<int> GetCollisionPartners() const override
    {
        return idsCollisionPartners;
    }

    void SetPositionX(double positionX) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdatePositionX(arg);
        },
        positionX);
    }

    void SetPositionY(double positionY) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdatePositionY(arg);
        },
        positionY);
    }

    void SetWidth(double width) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateWidth(arg);
        },
        width);
    }

    void SetLength(double length) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateLength(arg);
        },
        length);
    }

    void SetHeight(double height) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateHeight(arg);
        },
        height);
    }

    void SetVelocityX(double velocityX) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateVelocityX(arg);
        },
        velocityX);
    }

    void SetVelocityY(double velocityY) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateVelocityY(arg);
        },
        velocityY);
    }

    void SetDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateDistanceCOGtoFrontAxle(arg);
        },
        distanceCOGtoFrontAxle);
    }

    void SetWeight(double weight) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateWeight(arg);
        },
        weight);
    }

    void SetHeightCOG(double heightCOG) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateHeightCOG(arg);
        },
        heightCOG);
    }

    void SetWheelbase(double wheelbase) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateWheelbase(arg);
        },
        wheelbase);
    }

    void SetMomentInertiaRoll(double momentInertiaRoll) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMomentInertiaRoll(arg);
        },
        momentInertiaRoll);
    }

    void SetMomentInertiaPitch(double momentInertiaPitch) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMomentInertiaPitch(arg);
        },
        momentInertiaPitch);
    }

    void SetMomentInertiaYaw(double momentInertiaYaw) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMomentInertiaYaw(arg);
        },
        momentInertiaYaw);
    }

    void SetEngineSpeed(double engineSpeed) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateEngineSpeed(arg);
        },
        engineSpeed);
    }

    void SetMaxAcceleration(double maxAcceleration) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMaxAcceleration(arg);
        },
        maxAcceleration);
    }

    void SetMaxDeceleration(double maxDeceleration) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMaxDeceleration(arg);
        },
        maxDeceleration);
    }

    void SetFrictionCoeff(double frictionCoeff) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateFrictionCoeff(arg);
        },
        frictionCoeff);
    }

    void SetTrackWidth(double trackWidth) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateTrackWidth(arg);
        },
        trackWidth);
    }

    void SetDistanceCOGtoLeadingEdge(double distanceCOGtoLeadingEdge) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateDistanceCOGtoLeadingEdge(arg);
        },
        distanceCOGtoLeadingEdge);
    }

    void SetGear(int gear) override
    {
        world->QueueAgentUpdate([this](int arg)
        {
            UpdateGear(arg);
        },
        gear);
    }

    void SetEffAccelPedal(double percent) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateAccelPedal(arg);
        },
        percent);
    }

    void SetEffBrakePedal(double percent) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateBrakePedal(arg);
        },
        percent);
    }

    void SetAccelerationX(double accelerationX) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateAccelerationX(arg);
        },
        accelerationX);
    }

    void SetAccelerationY(double accelerationY) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateAccelerationY(arg);
        },
        accelerationY);
    }

    void SetYawAngle(double yawAngle) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateYawAngle(arg);
        },
        yawAngle);
    }

    void SetYawVelocity(double yawVelocity) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateYawVelocity(arg);
        },
        yawVelocity);
    }

    void SetYawAcceleration(double yawAcceleration) override
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateYawAcceleration(arg);
        },
        yawAcceleration);
    }

    void UpdateWidth(double width)
    {
        this->width = width;
        OWL::Primitive::Dimension dimension = baseTrafficObject->GetDimension();
        dimension.width = width;
        baseTrafficObject->SetDimension(dimension);
    }

    void UpdateLength(double length)
    {
        this->length = length;
        OWL::Primitive::Dimension dimension = baseTrafficObject->GetDimension();
        dimension.length = length;
        baseTrafficObject->SetDimension(dimension);
    }

    void UpdateHeight(double height)
    {
        this->height = height;
        OWL::Primitive::Dimension dimension = baseTrafficObject->GetDimension();
        dimension.height = height;
        baseTrafficObject->SetDimension(dimension);
    }

    void UpdateVelocityX(double velocityX)
    {
        this->velocityX = velocityX;
        baseTrafficObject->SetAbsVelocity(velocityX);
    }

    void UpdateAccelerationX(double accelerationX)
    {
        this->accelerationX = accelerationX;
        baseTrafficObject->SetAbsAcceleration(accelerationX);
    }

    void UpdatePositionX(double positionX)
    {
        this->positionX = positionX;
        baseTrafficObject->SetX(positionX);
    }

    void UpdatePositionY(double positionY)
    {
        this->positionY = positionY;
        baseTrafficObject->SetY(positionY);
    }

    void UpdateVelocityY(double velocityY)
    {
        this->velocityY = velocityY;
        /* NOTE: not used now
           TODO: map to OSI
        OWL::Primitive::AbsVelocity velocity = baseTrafficObject->GetAbsVelocity();
        velocity.vx = ...
        velocity.vy = velocityY;
        baseTrafficObject->SetAbsVelocity(velocity);
        */
    }

    void UpdateAccelerationY(double accelerationY)
    {
        this->accelerationY = accelerationY;
        /* NOTE: not used now
           TODO: map to OSI
        OWL::Primitive::AbsAcceleration acceleration = baseTrafficObject->GetAbsAcceleration();
        acceleration.ay = accelerationY;
        baseTrafficObject->SetAbsAcceleration(acceleration);
        */
    }

    void UpdateYawAngle(double yawAngle)
    {
        this->yawAngle = yawAngle;
        OWL::Primitive::AbsOrientation orientation = baseTrafficObject->GetAbsOrientation();
        orientation.yaw = yawAngle;
        baseTrafficObject->SetAbsOrientation(orientation);
    }

    void UpdateYawVelocity(double yawVelocity)
    {
        this->yawVelocity = yawVelocity;
        OWL::Primitive::AbsOrientationRate orientationRate = baseTrafficObject->GetAbsOrientationRate();
        orientationRate.yawRate = yawVelocity;
        baseTrafficObject->SetAbsOrientationRate(orientationRate);
    }

    void UpdateYawAcceleration(double yawAcceleration)
    {
        this->yawAcceleration = yawAcceleration;
    }

    void UpdateAccelerationIntention(double accelerationIntention)
    {
        this->accelerationIntention = accelerationIntention;
    }

    void UpdateDecelerationIntention(double decelerationIntention)
    {
        this->decelerationIntention = decelerationIntention;
    }

    void UpdateAngleIntention(double angleIntention)
    {
        this->angleIntention = angleIntention;
    }

    void UpdateGear(double gear)
    {
        this->currentGear = static_cast<int>(gear);
    }

    void UpdateAccelPedal(double percent)
    {
        this->accelPedal = percent;
    }

    void UpdateBrakePedal(double percent)
    {
        this->brakePedal = percent;
    }

    void UpdateHeadLight(bool headLight)
    {
        this->headLightSwitch = headLight;
    }

    void UpdateHighBeamLight(bool highBeam)
    {
        this->highBeamLightSwitch = highBeam;
    }

    void UpdateBrakeLight(bool brakeLightStatus)
    {
        this->brakeLight = brakeLightStatus;
    }

    void UpdateHorn(bool horn)
    {
        this->hornSwitch = horn;
    }

    void UpdateFlasher(bool flasher)
    {
        this->flasherSwitch = flasher;
    }

    void UpdateYaw(double yawAngle)
    {
        OWL::Primitive::AbsOrientation orientation = baseTrafficObject->GetAbsOrientation();
        orientation.yaw = yawAngle;
        baseTrafficObject->SetAbsOrientation(orientation);
    }

    void UpdateDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle)
    {
        this->distanceCOGToFrontAxle = distanceCOGtoFrontAxle;
    }

    void UpdateDistanceCOGtoLeadingEdge(double distanceCOGToLeadingEdge)
    {
        this->distanceCOGToLeadingEdge = distanceCOGToLeadingEdge;
    }

    void UpdateDistanceReferencePointToFrontAxle(double distanceReferencePointToFrontAxle)
    {
        this->distanceReferencePointToFrontAxle = distanceReferencePointToFrontAxle;
    }

    void UpdateWeight(double weight)
    {
        this->weight = weight;
    }

    void UpdateHeightCOG(double heightCOG)
    {
        this->heightCOG = heightCOG;
    }

    void UpdateWheelbase(double wheelbase)
    {
        this->wheelbase = wheelbase;
    }

    void UpdateMomentInertiaRoll(double momentInertiaRoll)
    {
        this->momentInertiaRoll = momentInertiaRoll;
    }

    void UpdateMomentInertiaPitch(double momentInertiaPitch)
    {
        this->momentInertiaPitch = momentInertiaPitch;
    }

    void UpdateMomentInertiaYaw(double momentInertiaYaw)
    {
        this->momentInertiaYaw = momentInertiaYaw;
    }

    void UpdateSteeringRatio(double steeringRatio)
    {
        this->steeringRatio = steeringRatio;
    }

    void UpdateMaxVelocity(double maxVelocity)
    {
       this->maxVelocity = maxVelocity;
    }

    void UpdateMaxCurvature(double maxCurvature)
    {
       this->maxCurvature = maxCurvature;
    }

    void UpdateMaxAcceleration(double maxAcceleration)
    {
       this->maxAcceleration = maxAcceleration;
    }

    void UpdateEngineSpeed(double engineSpeed)
    {
        this->engineSpeed = engineSpeed;
    }

    void UpdateMaxDeceleration(double maxDeceleration)
    {
       this->maxDeceleration = maxDeceleration;
    }

    void UpdateFrictionCoeff(double frictionCoeff)
    {
        this->frictionCoeff = frictionCoeff;
    }

    void UpdateTrackWidth(double trackWidth)
    {
        this->trackWidth = trackWidth;
    }

    void UpdateDistanceReferencePointToLeadingEdge(double distanceReferencePointToLeadingEdge)
    {
        this->distanceReferencePointToLeadingEdge = distanceReferencePointToLeadingEdge;
    }

    void UpdateCollisionState(bool collisionState)
    {
        this->collisionState = collisionState;
    }

    bool Locate() override;

    bool Unlocate() override;

    // callback from model
    void RemoveAgent() override
    {
        // disable in schedule
        isValid = false;
    }

    bool IsValid() const override
    {
        return isValid;
    }

    void SetBrakeLight(bool brakeLightStatus) override
    {
        world->QueueAgentUpdate([this](bool arg)
        {
            UpdateBrakeLight(arg);
        },
        brakeLightStatus);
    }

    bool GetBrakeLight() const override
    {
        return brakeLight;
    }

    int GetAgentTypeId() const override
    {
        return agentTypeId;
    }

    int GetAgentLaneId() const override
    {
        return frontMainLaneId;
    }

    int GetAgentLaneIdLeft() override
    {
        // TODO
        return 0;
    }

    int GetAgentLaneIdRight() override
    {
        // TODO
        return 0;
    }

    int GetAgentLaneNumber() override
    {
        // TODO
        return 0;
    }

    void ReinitCarInQueue() override {} //dummy

    bool IsAgentAtEndOfRoad() override
    {
        return false;
    }

    void ReenterAgentAtStart() override {} //dummy

    Position GetPositionByDistance(double distance) const override
    {
        Q_UNUSED(distance);    //dummy
        return Position();
    }

    void SetIndicatorState(IndicatorState indicatorState) override;

    IndicatorState GetIndicatorState() override;

    int GetNumberOfLaneAssignments() const;

    bool IsAgentInWorld() override;

    void SetPosition(Position pos) override;

    double GetDistanceToStartOfRoad() const override;

    double GetDistanceToStartOfRoad(MeasurementPoint mp) const override;

    double GetLaneWidth() override;

    double GetLaneWidthLeft() override;

    double GetLaneWidthRight() override;

    double GetCurvature() override;

    double GetCurvatureInDistance(double distance) override;

    bool IsSpecialAgent() const override;

    bool OnRoad(const OWL::Interfaces::Road& road) const;
    bool OnLane(const OWL::Interfaces::Lane& lane) const;

    const AgentInterface* GetAgentInFront(int laneId) const override;

    double GetDistanceToFrontAgent(int laneId) override;

    double GetDistanceToRearAgent(int laneId) override;

    double GetDistanceToFrontAgent();

    double GetDistanceFrontAgentToEgo() override
    {
        return 0.0; //dummy
    }

    bool HasTwoLeftLanes() override
    {
        return false;   //dummy
    }

    bool HasTwoRightLanes() override
    {
        return false;   //dummy
    }

    LaneChangeState EstimateLaneChangeState(double thresholdLooming) override
    {
        Q_UNUSED(thresholdLooming);    //dummy
        return LaneChangeState::NoLaneChange;
    }

    std::list<AgentInterface *> GetAllAgentsInLane(int laneID,
                                                   double minDistance,
                                                   double maxDistance,
                                                   double AccSensDist) override
    {
        Q_UNUSED(laneID);
        Q_UNUSED(minDistance);
        Q_UNUSED(maxDistance);
        Q_UNUSED(AccSensDist);
        std::list<AgentInterface *> dummy;
        return dummy;
    } //dummy

    bool IsBicycle() const override
    {
        return false;   //dummy
    }

    double GetDistanceToAgent(AgentInterface* otherAgent) override;

    double GetDistanceToSpecialAgent() override
    {
        return INFINITY;
    }

    bool IsObstacle() override
    {
        return false;
    }

    bool IsFirstCarInLane() const override
    {
        return false;   //dummy
    }

    bool PerceiveMinimumSpeedOfPlatoonInLaneLeft(double MesoscopicPreviewDistance,
                                                 int &iLane,
                                                 double &laneSpeedDifferential) const override
    {
        Q_UNUSED(MesoscopicPreviewDistance);
        Q_UNUSED(iLane);
        Q_UNUSED(laneSpeedDifferential);

        return false;
    }

    bool PerceiveMinimumSpeedOfPlatoonInLaneRight(double MesoscopicPreviewDistance,
                                                  int &iLane,
                                                  double &laneSpeedDifferential) const override
    {
        Q_UNUSED(MesoscopicPreviewDistance);
        Q_UNUSED(iLane);
        Q_UNUSED(laneSpeedDifferential);

        return false;
    }

    double GetLaneDepartureFromLeftLaneBoundary() override
    {
        return 0.0;   //dummy
    }

    double GetLaneDepartureFromRightLaneBoundary() override
    {
        return 0.0;   //dummy
    }

    std::vector<AgentInterface*> GetAllAgentsInFront() const;

    AgentInterface *GetAgentBehind(int laneId) const override;

    bool ExistLaneLeft() override;

    bool ExistLaneRight() override;

    void GetAgentsDirectlyInFront(double PeripheralPreviewDistance,
                                  AgentInterface *&agentFront,
                                  AgentInterface *&agentFrontLeft,
                                  AgentInterface *&agentFrontRight) override;

    double GetDistanceToEndOfLane(double sightDistance) const override;

    void ObtainGroundTruthObjectLaneExistences(AreaOfInterest aoi,
                                               AgentInterface *&agentAOI,
                                               bool &hasRightLane,
                                               bool &hasLeftLane,
                                               double PreviewDistance,
                                               double _carLengthEffective) override;

    void SetCarInfo(CarInfo *carInfo) override;

    CarInfo *GetCarInfo() const override;

    double GetDistanceToEndOfRamp(int laneId) override;

    double GetPositionLateral() const override;

    void SetCarInfoExtra(void *extraInfo) override;

    void *GetCarInfoExtra() override;

    void AssignCarInfo(double accSensDist) override;

    void AssignCarInfoExtra() override;

    CarInfo *GenerateCarInfo() override;

    double GetLaneDirection() const override;

    void Unregister() const override;

    double GetLaneRemainder(Side side) const override;

    void RemoveSpecialAgentMarker() override {} //dummy
    void SetSpecialAgentMarker() override {} //dummy
    void SetObstacleFlag() override {} //dummy

    MarkType GetTypeOfNearestMark() const override
    {
        return MarkType::NONE;  //dummy
    }

    std::string GetTypeOfNearestMarkString() const override
    {
        return "NONE";  //dummy
    }

    double GetDistanceToNearestMark(MarkType markType) const override
    {
        Q_UNUSED(markType);
        return 0.0;     //dummy
    }

    double GetOrientationOfNearestMark(MarkType markType) const override
    {
        Q_UNUSED(markType);
        return 0.0;    //dummy
    }

    double GetViewDirectionToNearestMark(MarkType markType) const override
    {
        Q_UNUSED(markType);
        return 0.0;    //dummy
    }

    AgentViewDirection GetAgentViewDirectionToNearestMark(MarkType markType) const override
    {
        Q_UNUSED(markType);
        return AgentViewDirection::none;
    }

    double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                   AgentViewDirection agentViewDirection) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        return 0.0;     //dummy
    }

    double GetDistanceToNearestMarkInViewDirection(MarkType markType, double mainViewDirection) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        return 0.0;     //dummy
    }

    double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                      AgentViewDirection agentViewDirection) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        return 0.0;     //dummy
    }

    double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                      double mainViewDirection) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        return 0.0;     //dummy
    }

    double GetDistanceToNearestMarkInViewRange(MarkType markType, AgentViewDirection agentViewDirection,
                                               double range) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetDistanceToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                               double range) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                  AgentViewDirection agentViewDirection, double range) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetOrientationOfNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                  double range) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                    AgentViewDirection agentViewDirection, double range) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetViewDirectionToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                    double range) const override
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    std::string GetTypeOfNearestObject(AgentViewDirection agentViewDirection,
                                       double range) const override
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return "NONE";     //dummy
    }

    std::string GetTypeOfNearestObject(double mainViewDirection,
                                       double range) const override
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return "NONE";     //dummy
    }

    double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                 AgentViewDirection agentViewDirection,
                                                 double range) const override
    {
        Q_UNUSED(objectType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                 double mainViewDirection,
                                                 double range) const override
    {
        Q_UNUSED(objectType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                      AgentViewDirection agentViewDirection,
                                                      double range) const override
    {
        Q_UNUSED(objectType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                      double mainViewDirection,
                                                      double range) const override
    {
        Q_UNUSED(objectType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }


    int GetIdOfNearestAgent(AgentViewDirection agentViewDirection,
                            double range) const override
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    int GetIdOfNearestAgent(double mainViewDirection,
                            double range) const override
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetDistanceToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                double range) const override
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetDistanceToNearestAgentInViewRange(double mainViewDirection,
                                                double range) const override
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetViewDirectionToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                     double range) const override
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    double GetViewDirectionToNearestAgentInViewRange(double mainViewDirection,
                                                     double range) const override
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return 0.0;     //dummy
    }

    //-----------------------------------------------------------------------------
    //! Retrieve the visibility to an agent in a specific range about a viewing direction
    //! angle in radiant.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual double GetVisibilityToNearestAgentInViewRange(double mainViewDirection,
        double range) const
    {
        return 0.0; //dummy
    }

    const std::vector<int>* GetTrajectoryTime() const override
    {
        return nullptr;     //dummy
    }

    const std::vector<double>* GetTrajectoryXPos() const override
    {
        return nullptr;     //dummy
    }

    const std::vector<double>* GetTrajectoryYPos() const override
    {
        return nullptr;     //dummy
    }

    const std::vector<double>* GetTrajectoryVelocity() const override
    {
        return nullptr;     //dummy
    }

    const std::vector<double>* GetTrajectoryAngle() const override
    {
        return nullptr;     //dummy
    }

    void SetAccelerationIntention(double accelerationIntention) override;
    double GetAccelerationIntention() const override;
    void SetDecelerationIntention(double decelerationIntention) override;
    double GetDecelerationIntention() const override;
    void SetAngleIntention(double angleIntention) override;
    double GetAngleIntention() const override;
    void SetCollisionState(bool collisionState) override;
    bool GetCollisionState() const override;

protected:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message) const
    {
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    WorldInterface* world;
    OWL::WorldData* worldData;
    const CallbackInterface *callbacks;
    World::Localization::BaseTrafficObjectLocator locator;

    //-----------------------------------------------------------------------------
    //! Initialize the ego vehicle object inside the drivingView.
    //-----------------------------------------------------------------------------
    void InitEgoVehicle();

    //-----------------------------------------------------------------------------
    //! Initialize all dynamic views inside the drivingView.
    //-----------------------------------------------------------------------------
    void InitDynamicViews();

    //-----------------------------------------------------------------------------
    //! Initialize the route inside the drivingView.
    //-----------------------------------------------------------------------------
    void InitRouteView();

    //-----------------------------------------------------------------------------
    //! Initialize the section views inside the drivingView.
    //-----------------------------------------------------------------------------
    void InitSectionViews();

    //-----------------------------------------------------------------------------
    //! Update the ego vehicle object inside the drivingView.
    //-----------------------------------------------------------------------------
    void UpdateEgoVehicle();

    //-----------------------------------------------------------------------------
    //! Update all dynamic views inside the drivingView.
    //-----------------------------------------------------------------------------
    void UpdateDynamicViews();

    //-----------------------------------------------------------------------------
    //! Update the route inside the drivingView.
    //-----------------------------------------------------------------------------
    void UpdateRouteView();

    //-----------------------------------------------------------------------------
    //! Update the section views inside the drivingView.
    //-----------------------------------------------------------------------------
    void UpdateSectionViews();

    int frontMainLaneId = 0;
    int rearMainLaneId = 0;

    std::map<int, Remainder> remainder;
    std::map<int, double> frontLaneCoverages;
    std::map<int, double> rearLaneCoverages;

    struct LaneObjParameters
    {
        double distance;
        double relAngle;
        double latPosition;
        Common::Vector2d upperLeftCoord;
        Common::Vector2d upperRightCoord;
        Common::Vector2d lowerLeftCoord;
        Common::Vector2d lowerRightCoord;
    };

    std::vector<const AgentInterface*> GetAgentsInRange(int laneId, double minDistance, double maxDistance) const;

    double distanceReferencePointToFrontAxle = 0.0;
    double weight = 0.0;
    double heightCOG = 0.0;
    double wheelbase = 0.0;
    double momentInertiaRoll = 0.0;
    double momentInertiaPitch = 0.0;
    double momentInertiaYaw = 0.0;
    double steeringRatio = 0.0;
    double maxAcceleration = 0.0;
    double maxDeceleration = 0.0;
    double maxVelocity = 0.0;
    double maxCurvature = 0.0;
    double frictionCoeff = 0.0;
    double trackWidth = 0.0;
    double distanceReferencePointToLeadingEdge = 0.0;

    double distanceCOGToFrontAxle = 0.0;
    double distanceCOGToLeadingEdge = 0.0;

    double positionX = 0.0;
    double positionY = 0.0;
    double width = 0.0;
    double length = 0.0;
    double height = 0.0;
    double velocityX = 0.0;
    double velocityY = 0.0;
    double accelerationX = 0.0;
    double accelerationY = 0.0;

    double yawAngle = 0.0;
    double yawVelocity = 0.0;
    double yawAcceleration = 0.0;
    int currentGear = 0;
    double accelPedal = 0.;
    double brakePedal = 0.;
    double engineSpeed = 0.;
    bool hornSwitch = false;
    bool headLightSwitch = false;
    bool highBeamLightSwitch = false;
    bool flasherSwitch = false;

    bool brakeLight = false;

    double accelerationIntention = 0.0;
    double decelerationIntention = 0.0;
    double angleIntention = 0.0;

    bool collisionState = false;
    std::vector<int> idsCollisionPartners;

    RoadPosition roadPos; //localized position of agent reference point in road coordinate system

    bool isValid = true;

    int id{0};
    int agentTypeId{-1};

    std::string agentTypeName = "";
    std::string vehicleModelType = "";
    std::string driverProfileName = "";
    std::string objectName = "";
    AgentVehicleType vehicleType = AgentVehicleType::Undefined;

    double speedGoalMin;
    int spawnTime;

    CarInfo *carInfo = nullptr;

    bool completlyInWorld = false;
    AreaOfInterest aoi = AreaOfInterest::NumberOfAreaOfInterests;

protected:
    OWL::MovingObject* baseTrafficObject{nullptr};

    double s{0.0};
};
