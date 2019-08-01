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
#include <math.h>
#include "worldInterface.h"
#include "pcm_data.h"
#include "vector2d.h"

class World_PCM_Implementation;

/*!
 * \brief The AgentAdapter class
 * This class is a adapter for the communication between the framework and world.
 */
class AgentAdapter : public AgentInterface
{
public:
    AgentAdapter(WorldInterface *world, const CallbackInterface *callbacks);
    AgentAdapter(const AgentAdapter &) = delete;
    AgentAdapter(AgentAdapter &&) = delete;
    AgentAdapter &operator=(const AgentAdapter &) = delete;
    AgentAdapter &operator=(AgentAdapter &&) = delete;
    virtual ~AgentAdapter() = default;

    bool InitAgentParameter(int id,
                            int agentTypeId,
                            int spawnTime,
                            const AgentSpawnItem *agentSpawnItem,
                            const SpawnItemParameterInterface &spawnItemParameter);

    int GetAgentId() const
    {
        return id;
    }

    void ResetLocalDrivingView()
    {
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
    virtual int GetGear() const
    {
        return 0;    // dummy
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
                                     [collisionPartnerId](const int &idInList)
        {
            return collisionPartnerId == idInList;
        });
        if (findIter == idsCollisionPartners.end())
        {
            // not in list yet
            idsCollisionPartners.push_back(collisionPartnerId);
        }
    }

    void UpdateCollision(int collisionPartnerId, int collisionDataId,
                         void *collisionData);

    std::vector<int> GetCollisionPartners() const
    {
        return idsCollisionPartners;
    }

    std::vector<void *> GetCollisionData(int collisionPartnerId,
                                         int collisionDataId) const;

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
    virtual void SetGear(int gear)
    {
        Q_UNUSED(gear);    // dummy
    }

    virtual void SetEngineSpeed(double engineSpeed)
    {
        Q_UNUSED(engineSpeed);    // dummy
    }
    virtual void SetEffAccelPedal(double percent)
    {
        Q_UNUSED(percent);    // dummy
    }
    virtual void SetEffBrakePedal(double percent)
    {
        Q_UNUSED(percent);    // dummy
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
    virtual void SetMaxAcceleration(double maxAcceleration)
    {
        Q_UNUSED(maxAcceleration);    // dummy
    }
    virtual void SetMaxDeceleration(double maxDeceleration)
    {
        Q_UNUSED(maxDeceleration);    // dummy
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

    void UpdateYawVelocity(double yawVelocity)
    {
        this->yawVelocity = yawVelocity;
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

    void UpdateCollisionState(bool collisionState)
    {
        this->collisionState = collisionState;
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

    void SetBrakeLight(bool brakeLightStatus)
    {
        this->brakeLightStatus = brakeLightStatus;
    }

    bool GetBrakeLight() const
    {
        return brakeLightStatus;
    }

    int GetAgentTypeId() const;

    int GetSpawnTime() const
    {
        return 0;
    }

    void SetIndicatorState(IndicatorState indicatorState)
    {
        Q_UNUSED(indicatorState);   //dummy
    }
    IndicatorState GetIndicatorState()
    {
        return IndicatorState::IndicatorState_Off;   //dummy
    }
    int GetAgentLaneId() const
    {
        return 0;   //dummy
    }
    int GetAgentLaneIdLeft()
    {
        return 0;   //dummy
    }
    int GetAgentLaneIdRight()
    {
        return 0;   //dummy
    }
    int GetAgentLaneNumber()
    {
        return 0;   //dummy
    }
    bool IsAgentInWorld()
    {
        return true;   //dummy
    }
    void ReinitCarInQueue() {} //dummy
    bool IsAgentAtEndOfRoad()
    {
        return false;   //dummy
    }
    void ReenterAgentAtStart() {} //dummy
    void SetPosition(Position pos)
    {
        Q_UNUSED(pos);   //dummy
    }
    double GetDistanceToStartOfRoad() const
    {
        return 0;   //dummy
    }
    Position GetPositionByDistance(double distance) const
    {
        Q_UNUSED(distance);    //dummy
        return Position();
    }
    double GetLaneWidth()
    {
        return 0;   //dummy
    }
    double GetLaneWidthLeft()
    {
        return 0;   //dummy
    }
    double GetLaneWidthRight()
    {
        return 0;   //dummy
    }
    double GetCurvature()
    {
        return 0;   //dummy
    }
    double GetCurvatureInDistance(double distance)
    {
        Q_UNUSED(distance);    //dummy
        return 0;
    }
    bool IsSpecialAgent() const
    {
        return false;   //dummy
    }

    double GetDistanceToFrontAgent(int laneId);
    double GetDistanceToRearAgent(int laneId)
    {
        Q_UNUSED(laneId);    //dummy
        return 0;
    }
    const AgentInterface *GetAgentInFront(int laneId) const
    {
        Q_UNUSED(laneId);    //dummy
        return nullptr;
    }
    const AgentInterface *GetAgentBehind(int laneId) const
    {
        Q_UNUSED(laneId);    //dummy
        return nullptr;
    }
    double GetDistanceToAgent(AgentInterface *otherAgent)
    {
        Q_UNUSED(otherAgent);    //dummy
        return 0;
    }
    void RemoveSpecialAgentMarker() {} //dummy
    void SetSpecialAgentMarker() {} //dummy
    bool ExistLaneLeft()
    {
        return false;   //dummy
    }
    bool ExistLaneRight()
    {
        return false;   //dummy
    }
    void SetObstacleFlag() {} //dummy
    double GetVelocityLateral()
    {
        return 0;   //dummy
    }
    void GetAgentsDirectlyInFront(double PeripheralPreviewDistance,
                                  AgentInterface *&agentFront,
                                  AgentInterface *&agentFrontLeft,
                                  AgentInterface *&agentFrontRight)
    {
        Q_UNUSED(PeripheralPreviewDistance);
        Q_UNUSED(agentFront);
        Q_UNUSED(agentFrontLeft);
        Q_UNUSED(agentFrontRight);
    } //dummy
    double GetDistanceToSpecialAgent()
    {
        return 0;   //dummy
    }
    bool IsObstacle()
    {
        return false;   //dummy
    }
    double GetDistanceToEndOfLane(double sightDistance) const
    {
        Q_UNUSED(sightDistance);    //dummy
        return 0;
    }
    bool PerceiveMinimumSpeedOfPlatoonInLaneLeft(double MesoscopicPreviewDistance,
                                                 int &iLane,
                                                 double &laneSpeedDifferential) const
    {
        Q_UNUSED(MesoscopicPreviewDistance);
        Q_UNUSED(iLane);
        Q_UNUSED(laneSpeedDifferential);
        return false;
    } //dummy
    bool PerceiveMinimumSpeedOfPlatoonInLaneRight(double MesoscopicPreviewDistance,
                                                  int &iLane,
                                                  double &laneSpeedDifferential) const
    {
        Q_UNUSED(MesoscopicPreviewDistance);
        Q_UNUSED(iLane);
        Q_UNUSED(laneSpeedDifferential);
        return false;
    } //dummy
    void ObtainGroundTruthObjectLaneExistences(AgentInterface *&agent,
                                               bool &hasRightLane,
                                               bool &hasLeftLane,
                                               double PreviewDistance,
                                               double _carLengthEffective)
    {
        Q_UNUSED(agent);
        Q_UNUSED(hasRightLane);
        Q_UNUSED(hasLeftLane);
        Q_UNUSED(PreviewDistance);
        Q_UNUSED(_carLengthEffective);
    } //dummy
    double GetLaneDepartureFromLeftLaneBoundary()
    {
        return 0;   //dummy
    }
    double GetLaneDepartureFromRightLaneBoundary()
    {
        return 0;   //dummy
    }
    double GetVelocityAbsolute() const;
    void SetCarInfo(CarInfo *carInfo)
    {
        Q_UNUSED(carInfo);   //dummy
    }
    CarInfo *GetCarInfo() const
    {
        return nullptr;   //dummy
    }
    double GetDistanceToEndOfRamp(int laneId)
    {
        Q_UNUSED(laneId);    //dummy
        return 0;
    }
    double GetPositionLateral() const
    {
        return 0;   //dummy
    }
    void SetCarInfoExtra(void *extraInfo)
    {
        Q_UNUSED(extraInfo)
    } //dummy
    void *GetCarInfoExtra()
    {
        return nullptr;   //dummy
    }
    void AssignCarInfo(double accSensDist)
    {
        Q_UNUSED(accSensDist)
    } //dummy
    void AssignCarInfoExtra() {} //dummy
    CarInfo *GenerateCarInfo()
    {
        return nullptr;   //dummy
    }
    double GetDistanceFrontAgentToEgo()
    {
        return 0;   //dummy
    }
    bool HasTwoLeftLanes()
    {
        return false;   //dummy
    }
    bool HasTwoRightLanes()
    {
        return false;   //dummy
    }
    LaneChangeState EstimateLaneChangeState(double thresholdLooming)
    {
        Q_UNUSED(thresholdLooming);    //dummy
        return LaneChangeState::NoLaneChange;
    }
    std::list<AgentInterface *> GetAllAgentsInLane(int laneID,
                                                   double minDistance,
                                                   double maxDistance,
                                                   double AccSensDist)
    {
        Q_UNUSED(laneID);
        Q_UNUSED(minDistance);
        Q_UNUSED(maxDistance);
        Q_UNUSED(AccSensDist);
        std::list<AgentInterface *> dummy;
        return dummy;
    } //dummy
    bool IsBicycle() const
    {
        return false;   //dummy
    }
    double GetLaneDirection() const
    {
        return 0;   //dummy
    }
    void Unregister() const {} //dummy
    bool IsFirstCarInLane() const
    {
        return false;   //dummy
    }

    MarkType GetTypeOfNearestMark() const;
    std::string GetTypeOfNearestMarkString() const;
    double GetDistanceToNearestMark(MarkType markType) const;
    double GetOrientationOfNearestMark(MarkType markType) const;
    double GetViewDirectionToNearestMark(MarkType markType) const;
    AgentViewDirection GetAgentViewDirectionToNearestMark(MarkType markType) const;
    double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                   AgentViewDirection agentViewDirection) const;
    double GetDistanceToNearestMarkInViewDirection(MarkType markType, double mainViewDirection) const;
    double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                      AgentViewDirection agentViewDirection) const;
    double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                      double mainViewDirection) const;
    double GetDistanceToNearestMarkInViewRange(MarkType markType, AgentViewDirection agentViewDirection,
                                               double range) const;
    double GetDistanceToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                               double range) const;
    double GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                  AgentViewDirection agentViewDirection, double range) const;
    double GetOrientationOfNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                  double range) const;
    double GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                    AgentViewDirection agentViewDirection, double range) const;
    double GetViewDirectionToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                    double range) const;

    std::string GetTypeOfNearestObject(AgentViewDirection agentViewDirection,
                                       double range) const;
    std::string GetTypeOfNearestObject(double mainViewDirection,
                                       double range) const;
    double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                 AgentViewDirection agentViewDirection,
                                                 double range) const;
    double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                 double mainViewDirection,
                                                 double range) const;
    double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                      AgentViewDirection agentViewDirection,
                                                      double range) const;
    double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                      double mainViewDirection,
                                                      double range) const;

    int GetIdOfNearestAgent(AgentViewDirection agentViewDirection,
                            double range) const;
    int GetIdOfNearestAgent(double mainViewDirection,
                            double range) const;
    double GetDistanceToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                double range) const;
    double GetDistanceToNearestAgentInViewRange(double mainViewDirection,
                                                double range) const;
    double GetViewDirectionToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                     double range) const;
    double GetViewDirectionToNearestAgentInViewRange(double mainViewDirection,
                                                     double range) const;

    double GetVisibilityToNearestAgentInViewRange(double mainViewDirection, double range) const;

    double GetYawVelocity();
    void SetYawVelocity(double yawVelocity);
    double GetYawAcceleration();
    void SetYawAcceleration(double yawAcceleration);
    const std::vector<int> *GetTrajectoryTime() const;
    const std::vector<double> *GetTrajectoryXPos() const;
    const std::vector<double> *GetTrajectoryYPos() const;
    const std::vector<double> *GetTrajectoryVelocity() const;
    const std::vector<double> *GetTrajectoryAngle() const;

    void SetAccelerationIntention(double accelerationIntention);
    double GetAccelerationIntention() const;
    void SetDecelerationIntention(double decelerationIntention);
    double GetDecelerationIntention() const;
    void SetAngleIntention(double angleIntention);
    double GetAngleIntention() const;
    void SetCollisionState(bool collisionState);
    bool GetCollisionState() const;

    double GetAccelerationAbsolute() const;

//----------------END AgentInterface functions-------------------------------------



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
        if (callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }
    virtual RoadPosition GetRoadPosition() const
    {
        RoadPosition rp;
        return rp;//dummy
    }
    virtual double GetDistanceToStartOfRoad(MeasurementPoint mp) const
    {
        Q_UNUSED(mp);
        return 0.0;//dummy
    }
    virtual double GetDistanceReferencePointToLeadingEdge() const
    {
        return 0.0;//dummy
    }
    virtual double GetEngineSpeed() const
    {
        return 0.0;//dummy
    }
    virtual double GetEffAccelPedal() const
    {
        return 0.0;//dummy
    }
    virtual double GetEffBrakePedal() const
    {
        return 0.0;//dummy
    }
    virtual double GetMaxAcceleration() const
    {
        return 0.0;//dummy
    }
    virtual double GetMaxDeceleration() const
    {
        return 0.0;//dummy
    }
    virtual double GetLaneRemainder(Side) const
    {
        return 0.0;//dummy
    }

private:
    double GetAbsoluteViewAngle(double mainViewDirection) const ;
    double GetViewDirectionToPoint(const PCM_Point &point) const;
    AgentDetection GetNearestAgent(double mainViewDirection,double range) const;
    bool GetObstacleViewRanges(double viewAngle, double range,
                               double distanceMax,
                               const std::map<int, PCM_Line *> *lineMap,
                               std::vector<std::pair<double, double>> &obstacleViewRanges) const;

    double positionX = 0.0;
    double positionY = 0.0;
    double width = 0.0;
    double length = 0.0;
    double height = 0.0;
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
    double yawVelocity = 0.0;
    double yawAcceleration = 0.0;
    double accelerationIntention = 0.0;
    double decelerationIntention = 0.0;
    double angleIntention = 0.0;
    bool collisionState = false;

    AgentVehicleType vehicleType = AgentVehicleType::Undefined;

    std::vector<int> idsCollisionPartners;
    std::map<int, std::map<int, std::vector<void *>>> collisionCrashData;
    bool isValid = true;
    bool brakeLightStatus = false;

    int id = -1;
    int agentTypeId = -1;

    WorldInterface *world = nullptr;
    World_PCM_Implementation *worldPCM = nullptr;
    const CallbackInterface *callbacks = nullptr;
};

#endif // AGENTADAPTER_H
