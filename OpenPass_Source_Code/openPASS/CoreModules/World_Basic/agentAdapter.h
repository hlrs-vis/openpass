/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  agentAdapter.h
//! @brief This file implements the wrapper for the agent so it can
//!        interact with the world.
//!        It permits a simple implementation of delegation concepts
//!        (composition vs. inheritance).
//-----------------------------------------------------------------------------


#ifndef AGENTADAPTER_H
#define AGENTADAPTER_H

#include <QtGlobal>
#include "worldInterface.h"

/*!
 * \brief The AgentAdapter class
 * This class is a adapter for the communication between the framework and world.
 */
class AgentAdapter : public AgentInterface
{
public:
    AgentAdapter(WorldInterface *world, const CallbackInterface *callbacks);
    AgentAdapter(const AgentAdapter&) = delete;
    AgentAdapter(AgentAdapter&&) = delete;
    AgentAdapter& operator=(const AgentAdapter&) = delete;
    AgentAdapter& operator=(AgentAdapter&&) = delete;
    virtual ~AgentAdapter();

    bool InitAgentParameter(int id,
                            int agentTypeId,
                            int spawnTime,
                            const AgentSpawnItem *agentSpawnItem,
                            const SpawnItemParameterInterface &spawnItemParameter);

    int GetAgentId() const{
        return id;
    }

    void ResetLocalDrivingView(){
        return;
    }

    double GetPositionX() const
    {
        return positionX;
    }

    double GetPositionY() const
    {
        return positionY;
    }

    double GetWidth() const
    {
        return width;
    }

    double GetLength() const
    {
        return length;
    }

    double GetHeight() const
    {
        return height;
    }

    double GetVelocityX() const
    {
        return velocityX;
    }

    double GetVelocityY() const
    {
        return velocityY;
    }

    double GetDistanceCOGtoFrontAxle() const
    {
        return distanceCOGtoFrontAxle;
    }

    double GetWeight() const
    {
        return weight;
    }

    double GetHeightCOG() const
    {
        return heightCOG;
    }

    double GetWheelbase() const
    {
        return wheelbase;
    }

    double GetMomentInertiaRoll() const
    {
        return momentInertiaRoll;
    }

    double GetMomentInertiaPitch() const
    {
        return momentInertiaPitch;
    }

    double GetMomentInertiaYaw() const
    {
        return momentInertiaYaw;
    }

    double GetFrictionCoeff() const
    {
        return frictionCoeff;
    }

    double GetTrackWidth() const
    {
        return trackWidth;
    }

    double GetDistanceCOGtoLeadingEdge() const
    {
        return distanceCOGtoLeadingEdge;
    }

    double GetAccelerationX() const
    {
        return accelerationX;
    }

    double GetAccelerationY() const
    {
        return accelerationY;
    }

    double GetYawAngle() const
    {
        return yawAngle;
    }

    void UpdateCollision(int collisionPartnerId)
    {
        auto findIter = std::find_if(idsCollisionPartners.begin(), idsCollisionPartners.end(),
                                     [collisionPartnerId](const int &idInList) { return collisionPartnerId == idInList; });
        if (findIter == idsCollisionPartners.end())
        {
            // not in list yet
            idsCollisionPartners.push_back(collisionPartnerId);
        }
    }

    std::vector<int> GetCollisionPartners() const
    {
        return idsCollisionPartners;
    }

    AgentVehicleType GetVehicleType() const
    {
        return vehicleType;
    }

    void SetPositionX(double positionX)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdatePositionX(arg);
        },
        positionX);
    }

    void SetPositionY(double positionY)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdatePositionY(arg);
        },
        positionY);
    }

    void SetWidth(double width)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateWidth(arg);
        },
        width);
    }

    void SetLength(double length)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateLength(arg);
        },
        length);
    }

    void SetHeight(double height)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateHeight(arg);
        },
        height);
    }

    void SetVelocityX(double velocityX)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateVelocityX(arg);
        },
        velocityX);
    }

    void SetVelocityY(double velocityY)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateVelocityY(arg);
        },
        velocityY);
    }

    void SetDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateDistanceCOGtoFrontAxle(arg);
        },
        distanceCOGtoFrontAxle);
    }

    void SetWeight(double weight)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateWeight(arg);
        },
        weight);
    }

    void SetHeightCOG(double heightCOG)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateHeightCOG(arg);
        },
        heightCOG);
    }

    void SetWheelbase(double wheelbase)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateWheelbase(arg);
        },
        wheelbase);
    }

    void SetMomentInertiaRoll(double momentInertiaRoll)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMomentInertiaRoll(arg);
        },
        momentInertiaRoll);
    }

    void SetMomentInertiaPitch(double momentInertiaPitch)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMomentInertiaPitch(arg);
        },
        momentInertiaPitch);
    }

    void SetMomentInertiaYaw(double momentInertiaYaw)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateMomentInertiaYaw(arg);
        },
        momentInertiaYaw);
    }

    void SetFrictionCoeff(double frictionCoeff)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateFrictionCoeff(arg);
        },
        frictionCoeff);
    }

    void SetTrackWidth(double trackWidth)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateTrackWidth(arg);
        },
        trackWidth);
    }

    void SetDistanceCOGtoLeadingEdge(double distanceCOGtoLeadingEdge)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateDistanceCOGtoLeadingEdge(arg);
        },
        distanceCOGtoLeadingEdge);
    }

    void SetAccelerationX(double accelerationX)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateAccelerationX(arg);
        },
        accelerationX);
    }

    void SetAccelerationY(double accelerationY)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateAccelerationY(arg);
        },
        accelerationY);
    }

    void SetYawAngle(double yawAngle)
    {
        world->QueueAgentUpdate([this](double arg)
        {
            UpdateYawAngle(arg);
        },
        yawAngle);
    }

    void UpdateWidth(double width)
    {
        this->width = width;
    }

    void UpdateLength(double length)
    {
        this->length = length;
    }

    void UpdateHeight(double height)
    {
        this->height = height;
    }

    void UpdatePositionX(double positionX)
    {
        this->positionX = positionX;
    }

    void UpdateVelocityX(double velocityX)
    {
        this->velocityX = velocityX;
    }

    void UpdateAccelerationX(double accelerationX)
    {
        this->accelerationX = accelerationX;
    }

    void UpdatePositionY(double positionY)
    {
        this->positionY = positionY;
    }

    void UpdateVelocityY(double velocityY)
    {
        this->velocityY = velocityY;
    }

    void UpdateAccelerationY(double accelerationY)
    {
        this->accelerationY = accelerationY;
    }

    void UpdateYawAngle(double yawAngle)
    {
        this->yawAngle = yawAngle;
    }

    void UpdateDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle)
    {
        this->distanceCOGtoFrontAxle = distanceCOGtoFrontAxle;
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

    void UpdateFrictionCoeff(double frictionCoeff)
    {
        this->frictionCoeff = frictionCoeff;
    }

    void UpdateTrackWidth(double trackWidth)
    {
        this->trackWidth = trackWidth;
    }

    void UpdateDistanceCOGtoLeadingEdge(double distanceCOGtoLeadingEdge)
    {
        this->distanceCOGtoLeadingEdge = distanceCOGtoLeadingEdge;
    }

    bool Unlocate()
    {
        return true;
    }

    bool Locate()
    {
        return true;
    }

    // callback from model
    void RemoveAgent()
    {
        // disable in schedule
        isValid = false;
    }

    bool IsValid() const
    {
        return isValid;
    }

    void SetBrakeLight(bool brakeLightStatus){
        this->brakeLightStatus = brakeLightStatus;
    }

    bool GetBrakeLight(){
        return brakeLightStatus;
    }

    int GetAgentTypeId() const;

    int GetSpawnTime() const {
        return 0;
    }

    virtual void SetIndicatorState(IndicatorState indicatorState) {Q_UNUSED(indicatorState);} //dummy
    virtual IndicatorState GetIndicatorState() {return IndicatorState::IndicatorState_Off;} //dummy
    virtual int GetAgentLaneId() const {return 0;} //dummy
    virtual int GetAgentLaneIdLeft() {return 0;} //dummy
    virtual int GetAgentLaneIdRight() {return 0;} //dummy
    virtual int GetAgentLaneNumber() {return 0;} //dummy
    virtual bool IsAgentInWorld() {return true;} //dummy
    virtual void ReinitCarInQueue(){} //dummy
    virtual bool IsAgentAtEndOfRoad() {return false;} //dummy
    virtual void ReenterAgentAtStart() {} //dummy
    virtual void SetPosition(Position pos) {Q_UNUSED(pos);} //dummy
    virtual double GetDistanceToStartOfRoad() const {return 0;} //dummy
    virtual Position GetPositionByDistance(double distance) const {Q_UNUSED(distance); return Position();} //dummy
    virtual double GetLaneWidth() {return 0;} //dummy
    virtual double GetLaneWidthLeft() {return 0;} //dummy
    virtual double GetLaneWidthRight() {return 0;} //dummy
    virtual double GetCurvature() {return 0;} //dummy
    virtual double GetCurvatureInDistance(double distance) {Q_UNUSED(distance); return 0;} //dummy
    virtual bool IsSpecialAgent() const {return false;} //dummy

    double GetDistanceToFrontAgent(int laneId);
    virtual double GetDistanceToRearAgent(int laneId) {Q_UNUSED(laneId); return 0;} //dummy
    virtual AgentInterface *GetAgentInFront(int laneId) const {Q_UNUSED(laneId); return nullptr;} //dummy
    virtual AgentInterface *GetAgentBehind(int laneId) const {Q_UNUSED(laneId); return nullptr;} //dummy
    virtual double GetDistanceToAgent(AgentInterface *otherAgent) {Q_UNUSED(otherAgent);return 0;} //dummy
    virtual void RemoveSpecialAgentMarker(){} //dummy
    virtual void SetSpecialAgentMarker(){} //dummy
    virtual bool ExistLaneLeft() {return false;} //dummy
    virtual bool ExistLaneRight() {return false;} //dummy
    virtual void SetObstacleFlag(){} //dummy
    virtual double GetVelocityLateral() {return 0;} //dummy
    virtual void GetAgentsDirectlyInFront(double PeripheralPreviewDistance,
                                          AgentInterface *&agentFront,
                                          AgentInterface *&agentFrontLeft,
                                          AgentInterface *&agentFrontRight){Q_UNUSED(PeripheralPreviewDistance);
                                                                            Q_UNUSED(agentFront);
                                                                            Q_UNUSED(agentFrontLeft);
                                                                            Q_UNUSED(agentFrontRight);} //dummy
    virtual double GetDistanceToSpecialAgent() {return 0;} //dummy
    virtual bool IsObstacle() {return false;} //dummy
    virtual double GetDistanceToEndOfLane(double sightDistance) const  {Q_UNUSED(sightDistance);return 0;} //dummy
    virtual bool PerceiveMinimumSpeedOfPlatoonInLaneLeft(double MesoscopicPreviewDistance,
                                                         int &iLane,
                                                         double &laneSpeedDifferential) const {Q_UNUSED(MesoscopicPreviewDistance);
                                                                                               Q_UNUSED(iLane);
                                                                                               Q_UNUSED(laneSpeedDifferential);
                                                                                               return false;} //dummy
    virtual bool PerceiveMinimumSpeedOfPlatoonInLaneRight(double MesoscopicPreviewDistance,
                                                         int &iLane,
                                                          double &laneSpeedDifferential) const {Q_UNUSED(MesoscopicPreviewDistance);
                                                                                                Q_UNUSED(iLane);
                                                                                                Q_UNUSED(laneSpeedDifferential);
                                                                                                return false;} //dummy
    virtual void ObtainGroundTruthObjectLaneExistences(AreaOfInterest aoi,
                                                       AgentInterface* &agentAOI,
                                                       bool& hasRightLane,
                                                       bool& hasLeftLane,
                                                       double PreviewDistance,
                                                       double _carLengthEffective) {Q_UNUSED(aoi);
                                                                                    Q_UNUSED(agentAOI);
                                                                                    Q_UNUSED(hasRightLane);
                                                                                    Q_UNUSED(hasLeftLane);
                                                                                    Q_UNUSED(PreviewDistance);
                                                                                    Q_UNUSED(_carLengthEffective);} //dummy
    virtual double GetLaneDepartureFromLeftLaneBoundary() {return 0;} //dummy
    virtual double GetLaneDepartureFromRightLaneBoundary() {return 0;} //dummy
    virtual double GetVelocityAbsolute() {return 0;} //dummy
    virtual void SetCarInfo(CarInfo *carInfo) {Q_UNUSED(carInfo);} //dummy
    virtual CarInfo *GetCarInfo() const {return nullptr;} //dummy
    virtual double GetDistanceToEndOfRamp(int laneId) {Q_UNUSED(laneId); return 0;} //dummy
    virtual double GetPositionLateral() const {return 0;} //dummy
    virtual void SetCarInfoExtra(void *extraInfo) {Q_UNUSED(extraInfo)} //dummy
    virtual void *GetCarInfoExtra() {return nullptr;} //dummy
    virtual void AssignCarInfo(double accSensDist) {Q_UNUSED(accSensDist)} //dummy
    virtual void AssignCarInfoExtra(){} //dummy
    virtual CarInfo *GenerateCarInfo() {return nullptr;} //dummy
    virtual double GetDistanceFrontAgentToEgo() {return 0;} //dummy
    virtual bool HasTwoLeftLanes() {return false;} //dummy
    virtual bool HasTwoRightLanes() {return false;} //dummy
    virtual LaneChangeState EstimateLaneChangeState(double thresholdLooming) {Q_UNUSED(thresholdLooming); return LaneChangeState::NoLaneChange;} //dummy
    virtual std::list<AgentInterface*> GetAllAgentsInLane(int laneID,
                                                          double minDistance,
                                                          double maxDistance,
                                                          double AccSensDist) {Q_UNUSED(laneID);
                                                                               Q_UNUSED(minDistance);
                                                                               Q_UNUSED(maxDistance);
                                                                               Q_UNUSED(AccSensDist);
                                                                               std::list<AgentInterface*> dummy;
                                                                               return dummy;} //dummy
    virtual bool IsBicycle() const {return false;} //dummy
    virtual double GetLaneDirection() const {return 0;} //dummy
    virtual void Unregister() const {} //dummy
    virtual bool IsFirstCarInLane() const {return false;} //dummy

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
             const std::string &message)
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

    double positionX = 0;
    double positionY = 0;
    double width = 0;
    double length = 0;
    double height = 0;
    double velocityX = 0.0;
    double velocityY = 0.0;
    double distanceCOGtoFrontAxle = 0.0;
    double weight = 0.0;
    double heightCOG = 0.0;
    double wheelbase = 0.0;
    double momentInertiaRoll = 0.0;
    double momentInertiaPitch = 0.0;
    double momentInertiaYaw = 0.0;
    double frictionCoeff = 0.0;
    double trackWidth = 0.0;
    double distanceCOGtoLeadingEdge = 0.0;
    double accelerationX = 0.0;
    double accelerationY = 0.0;
    double yawAngle = 0.0;
    AgentVehicleType vehicleType = AgentVehicleType::Undefined;

    std::vector<int> idsCollisionPartners;
    bool isValid = true;
    bool brakeLightStatus = false;

    int id;
    int agentTypeId;

    WorldInterface *world;
    const CallbackInterface *callbacks;
};

#endif // AGENTADAPTER_H
