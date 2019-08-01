/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*               2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

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

#include "Interfaces/worldInterface.h"
#include "Interfaces/trafficObjectInterface.h"
#include "Localization/Localization.h"
#include "Localization/LocalizationCache.h"
#include "WorldData.h"
#include "WorldObjectAdapter.h"

constexpr double zeroBaseline = 1e-9;

/*!
 * \brief The AgentAdapter class
 * This class is a adapter for the communication between the framework and world.
 */
class AgentAdapter final : public WorldObjectAdapter, public AgentInterface
{
public:
    const std::string MODULENAME = "AGENTADAPTER";

    AgentAdapter(WorldInterface* world, const CallbackInterface* callbacks, World::Localization::Cache& localizationCache);
    AgentAdapter(const AgentAdapter&) = delete;
    AgentAdapter(AgentAdapter&&) = delete;
    AgentAdapter& operator=(const AgentAdapter&) = delete;
    AgentAdapter& operator=(AgentAdapter&&) = delete;
    ~AgentAdapter();

    ObjectTypeOSI GetType() const override
    {
        return ObjectTypeOSI::Vehicle;
    }

    bool InitAgentParameter(int id,
                            int spawnTime,
                            AgentBlueprintInterface* agentBlueprint) override;

    //////////////////////////////////////////////////////////

    int GetId() const override
    {
        return id;
    }

    std::string GetVehicleModelType() const override
    {
        return vehicleModelType;
    }

    std::string GetScenarioName() const override
    {
        return objectName;
    }

    std::string GetDriverProfileName() const override
    {
        return driverProfileName;
    }


    std::vector<CommonTrafficSign::Entity> GetTrafficSignsInRange(double searchDistance, int relativeLane = 0) const;

    double GetSpeedGoalMin() const override
    {
        return speedGoalMin;
    }

    int GetSpawnTime() const
    {
        return spawnTime;
    }

    double GetDistanceReferencePointToFrontAxle() const override
    {
        return distanceReferencePointToFrontAxle;
    }

    double GetEngineSpeed() const override
    {
        return engineSpeed;
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
        return GetBaseTrafficObject().GetDistanceReferencePointToLeadingEdge();
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

    double GetSteeringWheelAngle() const override
    {
        return steeringWheelAngle;
    }

    double GetMaxAcceleration() const override
    {
        return maxAcceleration;
    }

    double GetMaxDeceleration() const override
    {
        return maxDeceleration;
    }

    bool GetHeadLight() const override;

    bool GetHighBeamLight() const override;

    bool GetHorn() const override
    {
        return hornSwitch;
    }

    bool GetFlasher() const override
    {
        return flasherSwitch;
    }

    LightState GetLightState() const override;

    RoadPosition GetRoadPosition() const override
    {
        auto coor = GetBaseTrafficObject().GetRoadCoordinate();
        RoadPosition pos;
        pos.s = coor.s;
        pos.t = coor.t;
        pos.hdg = coor.hdg;
        return pos;
    }

    void UpdateCollision(std::pair<ObjectTypeOSI, int> collisionPartner) override;

    std::vector<std::pair<ObjectTypeOSI, int>> GetCollisionPartners() const override
    {
        return collisionPartners;
    }

    AgentVehicleType GetVehicleType() const override
    {
        return vehicleType;
    }

    VehicleModelParameters GetVehicleModelParameters() const override
    {
        return vehicleModelParameters;
    }

    void SetPositionX(double value) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdatePositionX, this, value));
    }

    void SetPositionY(double value) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdatePositionY, this, value));
    }


    void SetVelocity(double value) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateVelocity, this, value));
    }

    void SetAcceleration(double value) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateAcceleration, this, value));
    }

    void SetYaw(double value) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateYaw, this, value));
    }

    void SetYawRate(double value) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateYawRate, this, value));
    }

    void SetDistanceTraveled(double distanceTraveled) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateDistanceTraveled, this, distanceTraveled));
    }

    void SetWidth(double width) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateWidth, this, width));
    }

    void SetLength(double length) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateLength, this, length));
    }

    void SetHeight(double height) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateHeight, this, height));
    }

    void SetDistanceReferencePointToFrontAxle(double distanceReferencePointToFrontAxle) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateDistanceReferencePointToFrontAxle, this,
                                          distanceReferencePointToFrontAxle));
    }

    double GetWeight() const override
    {
        return weight;
    }

    void SetWeight(double weight) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateWeight, this, weight));
    }

    void SetHeightCOG(double heightCOG) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateHeightCOG, this, heightCOG));
    }

    void SetWheelbase(double wheelbase) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateWheelbase, this, wheelbase));
    }

    void SetMomentInertiaRoll(double momentInertiaRoll) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateMomentInertiaRoll, this, momentInertiaRoll));
    }

    void SetMomentInertiaPitch(double momentInertiaPitch) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateMomentInertiaPitch, this, momentInertiaPitch));
    }

    void SetMomentInertiaYaw(double momentInertiaYaw) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateMomentInertiaYaw, this, momentInertiaYaw));
    }

    void SetSteeringRatio(double steeringRatio) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateSteeringRatio, this, steeringRatio));
    }

    void SetMaxAcceleration(double maxAcceleration) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateMaxAcceleration, this, maxAcceleration));
    }

    void SetEngineSpeed(double engineSpeed) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateEngineSpeed, this, engineSpeed));
    }

    void SetMaxDeceleration(double maxDeceleration) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateMaxDeceleration, this, maxDeceleration));
    }

    void SetFrictionCoeff(double frictionCoeff) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateFrictionCoeff, this, frictionCoeff));
    }

    void SetTrackWidth(double trackWidth) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateTrackWidth, this, trackWidth));
    }

    void SetDistanceReferencePointToLeadingEdge(double distanceReferencePointToLeadingEdge) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateDistanceReferencePointToLeadingEdge, this,
                                          distanceReferencePointToLeadingEdge));
    }

    void SetGear(int gear) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateGear, this, gear));
    }

    void SetEffAccelPedal(double percent) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateAccelPedal, this, percent));
    }

    void SetEffBrakePedal(double percent) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateBrakePedal, this, percent));
    }

    void SetSteeringWheelAngle(double steeringWheelAngle) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateSteeringWheelAngle, this, steeringWheelAngle));
    }

    void SetHeadLight(bool headLight) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateHeadLight, this, headLight));
    }

    void SetHighBeamLight(bool highBeam) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateHighBeamLight, this, highBeam));
    }

    void SetHorn(bool horn) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateHorn, this, horn));
    }

    void SetFlasher(bool flasher) override
    {
        world->QueueAgentUpdate(std::bind(&AgentAdapter::UpdateFlasher, this, flasher));
    }

    double GetYawRate() const override
    {
        return GetBaseTrafficObject().GetAbsOrientationRate().yawRate;
    }

    void UpdateWidth(double width)
    {
        OWL::Primitive::Dimension dimension = baseTrafficObject.GetDimension();
        dimension.width = width;
        GetBaseTrafficObject().SetDimension(dimension);
    }

    void UpdateLength(double length)
    {
        OWL::Primitive::Dimension dimension = baseTrafficObject.GetDimension();
        dimension.length = length;
        GetBaseTrafficObject().SetDimension(dimension);
    }

    void UpdateHeight(double height)
    {
        OWL::Primitive::Dimension dimension = baseTrafficObject.GetDimension();
        dimension.height = height;
        GetBaseTrafficObject().SetDimension(dimension);
    }

    void UpdateVelocity(double velocity)
    {
        if (std::abs(velocity) < zeroBaseline)
        {
            velocity = 0.0;
        }
        GetBaseTrafficObject().SetAbsVelocity(velocity);
    }

    void UpdateAcceleration(double acceleration)
    {
        if (std::abs(acceleration) < zeroBaseline)
        {
            acceleration = 0.0;
        }
        GetBaseTrafficObject().SetAbsAcceleration(acceleration);
    }

    void UpdatePositionX(double positionX)
    {
        GetBaseTrafficObject().SetX(positionX);
    }

    void UpdatePositionY(double positionY)
    {
        GetBaseTrafficObject().SetY(positionY);
    }

    void UpdateDistanceTraveled(double distanceTraveled)
    {
        this->distanceTraveled = distanceTraveled;
    }

    void UpdateGear(double gear)
    {
        this->currentGear = static_cast<int>(gear);
    }

    void UpdateAccelPedal(double percent)
    {
        if (std::abs(percent) < zeroBaseline)
        {
            percent = 0.0;
        }
        this->accelPedal = percent;
    }

    void UpdateBrakePedal(double percent)
    {
        if (std::abs(percent) < zeroBaseline)
        {
            percent = 0.0;
        }
        this->brakePedal = percent;
    }

    void UpdateSteeringWheelAngle(double steeringWheelAngle)
    {
        if (std::abs(steeringWheelAngle) < zeroBaseline)
        {
            steeringWheelAngle = 0.0;
        }
        this->steeringWheelAngle = steeringWheelAngle;
    }

    void UpdateHeadLight(bool headLight)
    {
        GetBaseTrafficObject().SetHeadLight(headLight);
    }

    void UpdateHighBeamLight(bool highBeam)
    {
        GetBaseTrafficObject().SetHighBeamLight(highBeam);
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
        if (std::abs(yawAngle) < zeroBaseline)
        {
            yawAngle = 0.0;
        }
        OWL::Primitive::AbsOrientation orientation = baseTrafficObject.GetAbsOrientation();
        orientation.yaw = yawAngle;
        GetBaseTrafficObject().SetAbsOrientation(orientation);
    }

    void UpdateYawRate(double yawRate)
    {
        if (std::abs(yawRate) < zeroBaseline)
        {
            yawRate = 0.0;
        }
        OWL::Primitive::AbsOrientationRate orientationRate = GetBaseTrafficObject().GetAbsOrientationRate();
        orientationRate.yawRate = yawRate;
        GetBaseTrafficObject().SetAbsOrientationRate(orientationRate);
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
        if (std::abs(momentInertiaRoll) < zeroBaseline)
        {
            momentInertiaRoll = 0.0;
        }
        this->momentInertiaRoll = momentInertiaRoll;
    }

    void UpdateMomentInertiaPitch(double momentInertiaPitch)
    {
        if (std::abs(momentInertiaPitch) < zeroBaseline)
        {
            momentInertiaPitch = 0.0;
        }
        this->momentInertiaPitch = momentInertiaPitch;
    }

    void UpdateMomentInertiaYaw(double momentInertiaYaw)
    {
        if (std::abs(momentInertiaYaw) < zeroBaseline)
        {
            momentInertiaYaw = 0.0;
        }
        this->momentInertiaYaw = momentInertiaYaw;
    }

    void UpdateSteeringRatio(double steeringRatio)
    {
        if (std::abs(steeringRatio) < zeroBaseline)
        {
            steeringRatio = 0.0;
        }
        this->steeringRatio = steeringRatio;
    }

    void UpdateMaxVelocity(double maxVelocity)
    {
        if (std::abs(maxVelocity) < zeroBaseline)
        {
            maxVelocity = 0.0;
        }
        this->maxVelocity = maxVelocity;
    }

    void UpdateMaxCurvature(double maxCurvature)
    {
        if (std::abs(maxCurvature) < zeroBaseline)
        {
            maxCurvature = 0.0;
        }
        this->maxCurvature = maxCurvature;
    }

    void UpdateMaxAcceleration(double maxAcceleration)
    {
        if (std::abs(maxAcceleration) < zeroBaseline)
        {
            maxAcceleration = 0.0;
        }
        this->maxAcceleration = maxAcceleration;
    }

    void UpdateEngineSpeed(double engineSpeed)
    {
        if (std::abs(engineSpeed) < zeroBaseline)
        {
            engineSpeed = 0.0;
        }
        this->engineSpeed = engineSpeed;
    }

    void UpdateMaxDeceleration(double maxDeceleration)
    {
        if (std::abs(maxDeceleration) < zeroBaseline)
        {
            maxDeceleration = 0.0;
        }
        this->maxDeceleration = maxDeceleration;
    }

    void UpdateFrictionCoeff(double frictionCoeff)
    {
        if (std::abs(frictionCoeff) < zeroBaseline)
        {
            frictionCoeff = 0.0;
        }
        this->frictionCoeff = frictionCoeff;
    }

    void UpdateTrackWidth(double trackWidth)
    {
        this->trackWidth = trackWidth;
    }

    void UpdateDistanceReferencePointToLeadingEdge(double distanceReferencePointToLeadingEdge)
    {
        GetBaseTrafficObject().SetDistanceReferencPointToLeadingEdge(distanceReferencePointToLeadingEdge);
    }

    bool Locate() override;

    void Unlocate() override;

    bool Update() override;

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

    void SetBrakeLight(bool brakeLightStatus) override;

    bool GetBrakeLight() const override;

    AgentCategory GetAgentCategory() const override;

    std::string GetAgentTypeName() const override;

    void SetIndicatorState(IndicatorState indicatorState) override;

    IndicatorState GetIndicatorState() const override;

    std::string GetRoadId(MeasurementPoint mp = MeasurementPoint::Front) const override;

    int GetMainLaneId(MeasurementPoint mp = MeasurementPoint::Front) const override;

    virtual int GetLaneIdLeft() const override;

    virtual int GetLaneIdRight() const override;

    int GetLaneIdFromRelative(int relativeLaneId) const;

    bool IsLeavingWorld() const override;

    bool IsCrossingLanes() const override;

    int GetNumberOfLanes() override;

    bool IsAgentInWorld() const override;

    void SetPosition(Position pos) override;

    double GetDistanceToStartOfRoad() const override
    {
        return WorldObjectInterface::GetDistanceToStartOfRoad();
    }

    double GetDistanceToStartOfRoad(MeasurementPoint mp) const override;

    double GetLaneWidth(int relativeLane = 0, double distance = 0.0) const override;

    double GetLaneWidthRightDrivingAndStopLane() override;

    double GetLaneCurvature(int relativeLane = 0, double distance = 0.0) override;

    bool IsEgoAgent() const override;

    double GetDistanceToFrontTrafficObject() const;

    std::vector<AgentInterface*> GetAllAgentsInFront() const override;

    std::vector<const TrafficObjectInterface*> GetAllTrafficObjectsInFront() const;

    std::vector<const WorldObjectInterface*> GetAllWorldObjectsInFront() const override;

    bool OnRoad(const OWL::Interfaces::Road& road) const;
    bool OnLane(const OWL::Interfaces::Lane& lane) const;

    AgentInterface* GetAgentInFront(int laneId) const override;

    TrafficObjectInterface* GetTrafficObjectInFront(int laneId) const;

    WorldObjectInterface* GetObjectInFront(double previewDistance, int relativeLaneId = 0) const override;

    WorldObjectInterface* GetObjectBehind(double previewDistance, int relativeLaneId = 0) const override;

    const AgentInterface* GetAgentBehind(int laneId) const override;

    double GetDistanceToObject(const WorldObjectInterface* otherObject) const override;

    bool ExistsLaneLeft() const override;

    bool ExistsLaneRight() const override;

    bool IsLaneStopLane(int laneId, double distance = 0.0) override;

    bool IsLaneDrivingLane(int laneId, double distance = 0.0) override;

    bool IsLaneExitLane(int laneId, double distance = 0.0) override;

    bool IsLaneRamp(int laneId, double distance = 0.0) override;

    double GetDistanceToEndOfLane(double sightDistance, int relativeLane = 0) const override;

    double GetDistanceToEndOfExit(int laneID, double sightDistance) const override;

    double GetDistanceToEndOfRamp(int laneId, double sightDistance) const override;

    double GetVelocity(VelocityScope velocityScope = VelocityScope::Absolute) const override;

    double  GetRelativeYaw() const override
    {
        return GetRoadPosition().hdg;
    }

    double  GetDistanceTraveled() const override
    {
        return distanceTraveled;
    }

    const Obstruction GetObstruction(const WorldObjectInterface& worldObject) const override;

    double GetPositionLateral() const override;

    double GetLaneDirection() const override;

    void Unregister() const override;

    double GetLaneRemainder(Side side) const override;

    GlobalRoadPosition GetBoundaryPoint(Side side) const override;

    virtual const std::list<SensorParameter>& GetSensorParameters() const

    {
        return sensorParameters;
    }

    virtual void SetSensorParameters(std::list<SensorParameter> sensorParameters)
    {
        this->sensorParameters = sensorParameters;
    }

    virtual std::vector<const WorldObjectInterface*> GetObjectsInRange(int relativeLane, double backwardsRange,
            double forwardRange, MeasurementPoint mp) const override;

    virtual std::vector<const AgentInterface*> GetAgentsInRange(int relativeLane, double backwardsRange,
            double forwardRange, MeasurementPoint mp) const override;

    virtual std::vector<const AgentInterface*> GetAgentsInRangeAbsolute(int laneId, double minDistance,
            double maxDistance) const override;

    std::list<int> GetSecondaryCoveredLanes() override;

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
             const char* file,
             int line,
             const std::string& message) const
    {
        if (callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

public:
    virtual int GetAgentId() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetVelocityX() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetVelocityY() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceCOGtoLeadingEdge() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceCOGtoFrontAxle() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetAccelerationX() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetAccelerationY() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual std::vector<void *> GetCollisionData(int collisionPartnerId,
                                                 int collisionDataId) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetVelocityX(double velocityX) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetVelocityY(double velocityY) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetDistanceCOGtoLeadingEdge(double distanceCOGtoLeadingEdge) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetAccelerationX(double accelerationX) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetAccelerationY(double accelerationY) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool InitAgentParameter(int id,
                                    int agentTypeId,
                                    int spawnTime,
                                    const AgentSpawnItem *agentSpawnItem,
                                    const SpawnItemParameterInterface &spawnItemParameter) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual int GetAgentTypeId() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool IsAgentAtEndOfRoad() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToFrontAgent(int laneId) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToRearAgent(int laneId) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetSpecialAgentMarker() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetObstacleFlag() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void RemoveSpecialAgentMarker() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToSpecialAgent() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool IsObstacle() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceFrontAgentToEgo() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool HasTwoLeftLanes() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool HasTwoRightLanes() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual LaneChangeState EstimateLaneChangeState(double thresholdLooming) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual std::list<AgentInterface *> GetAllAgentsInLane(int laneID,
                                                           double minDistance,
                                                           double maxDistance,
                                                           double AccSensDist) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool IsBicycle() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool IsFirstCarInLane() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual MarkType GetTypeOfNearestMark() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual std::string GetTypeOfNearestMarkString() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestMark(MarkType markType) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetOrientationOfNearestMark(MarkType markType) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetViewDirectionToNearestMark(MarkType markType) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                           AgentViewDirection agentViewDirection) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                           double mainViewDirection) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                              AgentViewDirection agentViewDirection)const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                              double mainViewDirection) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestMarkInViewRange(MarkType markType,
                                                       AgentViewDirection agentViewDirection, double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                       double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                          AgentViewDirection agentViewDirection, double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetOrientationOfNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                          double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                            AgentViewDirection agentViewDirection, double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetViewDirectionToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                            double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual std::string GetTypeOfNearestObject(AgentViewDirection agentViewDirection,
                                               double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual std::string GetTypeOfNearestObject(double mainViewDirection,
                                               double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                         AgentViewDirection agentViewDirection,
                                                         double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                         double mainViewDirection,
                                                         double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                              AgentViewDirection agentViewDirection,
                                                              double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                              double mainViewDirection,
                                                              double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual int GetIdOfNearestAgent(AgentViewDirection agentViewDirection,
                                    double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual int GetIdOfNearestAgent(double mainViewDirection,
                                    double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                        double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDistanceToNearestAgentInViewRange(double mainViewDirection,
                                                        double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetViewDirectionToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                             double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetViewDirectionToNearestAgentInViewRange(double mainViewDirection,
                                                             double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetVisibilityToNearestAgentInViewRange(double mainViewDirection,
                                                        double range) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual AgentViewDirection GetAgentViewDirectionToNearestMark(MarkType markType) const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetYawAcceleration() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetYawAcceleration(double yawAcceleration) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const std::vector<int> *GetTrajectoryTime() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const std::vector<double> *GetTrajectoryXPos() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const std::vector<double> *GetTrajectoryYPos() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const std::vector<double> *GetTrajectoryVelocity() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const std::vector<double> *GetTrajectoryAngle() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetAccelerationIntention(double accelerationIntention) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetAccelerationIntention() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetDecelerationIntention(double decelerationIntention) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetDecelerationIntention() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetAngleIntention(double angleIntention) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetAngleIntention() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetCollisionState(bool collisionState) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool GetCollisionState() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual double GetAccelerationAbsolute() const override
    {
        throw std::runtime_error("not implemented");
    }


private:
    WorldInterface* world;
    const CallbackInterface* callbacks;
    OWL::Interfaces::WorldData* worldData;
    World::Localization::BaseTrafficObjectLocator locator;

    OWL::Interfaces::MovingObject& GetBaseTrafficObject()
    {
        return *(static_cast<OWL::Interfaces::MovingObject*>(&baseTrafficObject));
    }

    OWL::Interfaces::MovingObject& GetBaseTrafficObject() const
    {
        return *(static_cast<OWL::Interfaces::MovingObject*>(&baseTrafficObject));
    }


    //-----------------------------------------------------------------------------
    //! Initialize the ego vehicle object inside the drivingView.
    //-----------------------------------------------------------------------------
    void InitEgoVehicle();

    //-----------------------------------------------------------------------------
    //! Update the ego vehicle object inside the drivingView.
    //-----------------------------------------------------------------------------
    void UpdateEgoVehicle();


    std::map<int, Remainder> remainder;

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

    const WorldObjectInterface* GetNearestObjectInVector(std::vector<const WorldObjectInterface*> agents);

    double distanceReferencePointToFrontAxle = 0.0;
    double weight = 0.0;
    double heightCOG = 0.0;
    double wheelbase = 0.0;
    double momentInertiaRoll = 0.0;
    double momentInertiaPitch = 0.0;
    double momentInertiaYaw = 0.0;
    double steeringRatio = 0.0; //no getter
    double maxAcceleration = 0.0;
    double maxDeceleration = 0.0;
    double maxVelocity = 0.0; //no getter
    double maxCurvature = 0.0; //no getter
    double frictionCoeff = 0.0;
    double trackWidth = 0.0;
    int currentGear = 0;
    double accelPedal = 0.;
    double brakePedal = 0.;
    double steeringWheelAngle = 0.0;
    double engineSpeed = 0.;
    bool hornSwitch = false;
    bool flasherSwitch = false;
    double distanceTraveled = 0.0;

    polygon_t boundingBox2D;
    World::Localization::Result locateResult;
    mutable std::vector<GlobalRoadPosition> boundaryPoints;
    mutable World::Localization::Remainders remainders;

    std::vector<std::pair<ObjectTypeOSI, int>> collisionPartners;
    bool isValid = true;

    int id{0};
    AgentCategory agentCategory;
    std::string agentTypeName = "";
    std::string vehicleModelType = "";
    std::string driverProfileName = "";
    std::string objectName = "";
    AgentVehicleType vehicleType = AgentVehicleType::Undefined;
    VehicleModelParameters vehicleModelParameters;

    double speedGoalMin;
    int spawnTime;

    bool completlyInWorld = false;

    std::list<SensorParameter> sensorParameters;
};
