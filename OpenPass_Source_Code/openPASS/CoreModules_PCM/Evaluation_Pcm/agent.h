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
//! @file  Evaluation_Pcm/agent.h
//! @brief This file contains a simplified representation of an agent necessary
//!        for the collision detection and the run result.
//-----------------------------------------------------------------------------

#ifndef AGENT_H
#define AGENT_H

#include <qglobal.h>
#include "worldInterface.h"

//-----------------------------------------------------------------------------
//! Class for the collision detection (CD) representation of an agent
//-----------------------------------------------------------------------------
class CD_Agent : public AgentInterface
{
public:
    CD_Agent() = default;
    CD_Agent(const CD_Agent &) = delete;
    CD_Agent(CD_Agent &&) = delete;
    CD_Agent &operator=(const CD_Agent &) = delete;
    CD_Agent &operator=(CD_Agent &&) = delete;
    virtual ~CD_Agent() = default;

    virtual void ResetLocalDrivingView() {} // dummy
    virtual int GetAgentTypeId() const
    {
        return agentTypeId;
    }
    virtual int GetAgentId() const
    {
        return id;
    }
    virtual int GetSpawnTime() const
    {
        return 0;    // dummy
    }

    virtual AgentVehicleType GetVehicleType() const
    {
        return AgentVehicleType::Undefined;    // dummy
    }
    virtual double GetPositionX() const
    {
        return positionX;
    }
    virtual double GetPositionY() const
    {
        return positionY;
    }
    virtual double GetWidth() const
    {
        return width;
    }
    virtual double GetLength() const
    {
        return length;
    }
    virtual double GetHeight() const
    {
        return height;
    }
    virtual double GetVelocityX() const
    {
        return velocityX;
    }
    virtual double GetVelocityY() const
    {
        return velocityY;
    }
    virtual double GetDistanceCOGtoFrontAxle() const
    {
        return 0.0;    // dummy
    }
    virtual double GetWeight() const
    {
        return 0.0;    // dummy
    }
    virtual double GetHeightCOG() const
    {
        return 0.0;    // dummy
    }
    virtual double GetWheelbase() const
    {
        return 0.0;    // dummy
    }
    virtual double GetMomentInertiaRoll() const
    {
        return 0.0;    // dummy
    }
    virtual double GetMomentInertiaPitch() const
    {
        return 0.0;    // dummy
    }
    virtual double GetMomentInertiaYaw() const
    {
        return 0.0;    // dummy
    }
    virtual double GetFrictionCoeff() const
    {
        return 0.0;    // dummy
    }
    virtual double GetTrackWidth() const
    {
        return 0.0;    // dummy
    }
    virtual int GetGear() const
    {
        return 0;    // dummy
    }
    virtual double GetDistanceCOGtoLeadingEdge() const
    {
        return distanceCOGtoLeadingEdge;
    }
    virtual double GetAccelerationX() const
    {
        return 0.0;    // dummy
    }
    virtual double GetAccelerationY() const
    {
        return 0.0;    // dummy
    }
    virtual double GetYawAngle() const
    {
        return yawAngle;
    }

    virtual std::vector<int> GetCollisionPartners() const
    {
        return {};   //dummy
    }
    virtual std::vector<void *> GetCollisionData(int collisionPartnerId,
                                                 int collisionDataId) const
    {
        Q_UNUSED(collisionPartnerId);
        Q_UNUSED(collisionDataId);
        return {};
    } //dummy

    virtual void SetPositionX(double positionX)
    {
        this->positionX = positionX;
    }
    virtual void SetPositionY(double positionY)
    {
        this->positionY = positionY;
    }
    virtual void SetWidth(double width)
    {
        this->width = width;
    }
    virtual void SetLength(double length)
    {
        this->length = length;
    }
    virtual void SetHeight(double height)
    {
        this->height = height;
    }
    virtual void SetVelocityX(double velocityX)
    {
        this->velocityX = velocityX;
    }
    virtual void SetVelocityY(double velocityY)
    {
        this->velocityY = velocityY;
    }
    virtual void SetDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle)
    {
        Q_UNUSED(distanceCOGtoFrontAxle);    // dummy
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
    virtual void SetWeight(double weight)
    {
        Q_UNUSED(weight);    // dummy
    }
    virtual void SetHeightCOG(double heightCOG)
    {
        Q_UNUSED(heightCOG);    // dummy
    }
    virtual void SetWheelbase(double wheelbase)
    {
        Q_UNUSED(wheelbase);    // dummy
    }
    virtual void SetMomentInertiaRoll(double momentInertiaRoll)
    {
        Q_UNUSED(momentInertiaRoll);    // dummy
    }
    virtual void SetMomentInertiaPitch(double momentInertiaPitch)
    {
        Q_UNUSED(momentInertiaPitch);    // dummy
    }
    virtual void SetMomentInertiaYaw(double momentInertiaYaw)
    {
        Q_UNUSED(momentInertiaYaw);    // dummy
    }
    virtual void SetMaxAcceleration(double maxAcceleration)
    {
        Q_UNUSED(maxAcceleration);    // dummy
    }
    virtual void SetMaxDeceleration(double maxDeceleration)
    {
        Q_UNUSED(maxDeceleration);    // dummy
    }
    virtual void SetFrictionCoeff(double frictionCoeff)
    {
        Q_UNUSED(frictionCoeff);    // dummy
    }
    virtual void SetTrackWidth(double trackWidth)
    {
        Q_UNUSED(trackWidth);    // dummy
    }
    virtual void SetDistanceCOGtoLeadingEdge(double distanceCOGtoLeadingEdge)
    {
        this->distanceCOGtoLeadingEdge = distanceCOGtoLeadingEdge;
    }
    virtual void SetAccelerationX(double accelerationX)
    {
        Q_UNUSED(accelerationX);    // dummy
    }
    virtual void SetAccelerationY(double accelerationY)
    {
        Q_UNUSED(accelerationY);    // dummy
    }
    virtual void SetYawAngle(double yawAngle)
    {
        this->yawAngle = yawAngle;
    }

    virtual void RemoveAgent()
    {
        isValid = false;
    }
    virtual void UpdateCollision(int collisionPartnerId)
    {
        Q_UNUSED(collisionPartnerId);   //dummy
    }
    virtual void UpdateCollision(int collisionPartnerId, int collisionDataId,
                                 void *collisionData)
    {
        Q_UNUSED(collisionPartnerId);
        Q_UNUSED(collisionDataId);
        Q_UNUSED(collisionData);
    } //dummy

    virtual bool Unlocate()
    {
        return false;   //dummy
    }
    virtual bool Locate()
    {
        return false;   //dummy
    }
    virtual AgentInterface *GetAgentAdapter() const
    {
        return nullptr;   //dummy
    }
    virtual void SetAgentAdapter(AgentInterface *agentAdapter)
    {
        Q_UNUSED(agentAdapter);   //dummy
    }
    virtual void SetBrakeLight(bool brakeLightStatus)
    {
        Q_UNUSED(brakeLightStatus);   //dummy
    }
    virtual bool GetBrakeLight() const
    {
        return false;   //dummy
    }
    virtual bool InitAgentParameter(int id,
                                    int agentTypeId,
                                    const AgentSpawnItem *agentSpawnItem,
                                    const SpawnItemParameterInterface &spawnItemParameter)  //dummy
    {
        this->id = id;
        this->agentTypeId = agentTypeId;
        Q_UNUSED(agentSpawnItem);
        Q_UNUSED(spawnItemParameter);
        return true;
    }
    virtual bool IsValid() const
    {
        return isValid;
    }

    virtual bool InitAgentParameter(int id,
                                    int agentTypeId,
                                    int spawnTime,
                                    const AgentSpawnItem *agentSpawnItem,
                                    const SpawnItemParameterInterface &spawnItemParameter)
    {
        Q_UNUSED(id);
        Q_UNUSED(agentTypeId);
        Q_UNUSED(spawnTime);
        Q_UNUSED(agentSpawnItem);
        Q_UNUSED(spawnItemParameter);
        return true;
    } //dummy

    virtual void SetIndicatorState(IndicatorState indicatorState)
    {
        Q_UNUSED(indicatorState);   //dummy
    }
    virtual IndicatorState GetIndicatorState()
    {
        return IndicatorState::IndicatorState_Off;   //dummy
    }
    virtual int GetAgentLaneId() const
    {
        return 0;   //dummy
    }
    virtual int GetAgentLaneIdLeft()
    {
        return 0;   //dummy
    }
    virtual int GetAgentLaneIdRight()
    {
        return 0;   //dummy
    }
    virtual int GetAgentLaneNumber()
    {
        return 0;   //dummy
    }
    virtual bool IsAgentInWorld()
    {
        return false;   //dummy
    }
    virtual void ReinitCarInQueue() {} //dummy
    virtual bool IsAgentAtEndOfRoad()
    {
        return false;   //dummy
    }
    virtual void ReenterAgentAtStart() {} //dummy
    virtual void SetPosition(Position pos)
    {
        Q_UNUSED(pos);   //dummy
    }
    virtual double GetDistanceToStartOfRoad() const
    {
        return 0;   //dummy
    }
    virtual Position GetPositionByDistance(double distance) const
    {
        Q_UNUSED(distance);    //dummy
        return Position();
    }
    virtual double GetLaneWidth()
    {
        return 0;   //dummy
    }
    virtual double GetLaneWidthLeft()
    {
        return 0;   //dummy
    }
    virtual double GetLaneWidthRight()
    {
        return 0;   //dummy
    }
    virtual double GetCurvature()
    {
        return 0;   //dummy
    }
    virtual double GetCurvatureInDistance(double distance)
    {
        Q_UNUSED(distance);    //dummy
        return 0;
    }
    virtual bool IsSpecialAgent() const
    {
        return false;   //dummy
    }

    virtual double GetDistanceToFrontAgent(int laneId)
    {
        Q_UNUSED(laneId);    //dummy
        return 0;
    }
    virtual double GetDistanceToRearAgent(int laneId)
    {
        Q_UNUSED(laneId);    //dummy
        return 0;
    }
    virtual const AgentInterface *GetAgentInFront(int laneId) const
    {
        Q_UNUSED(laneId);    //dummy
        return nullptr;
    }
    virtual const AgentInterface *GetAgentBehind(int laneId) const
    {
        Q_UNUSED(laneId);    //dummy
        return nullptr;
    }
    virtual double GetDistanceToAgent(AgentInterface *otherAgent)
    {
        Q_UNUSED(otherAgent);    //dummy
        return 0;
    }
    virtual void RemoveSpecialAgentMarker() {} //dummy
    virtual void SetSpecialAgentMarker() {} //dummy
    virtual bool ExistLaneLeft()
    {
        return false;   //dummy
    }
    virtual bool ExistLaneRight()
    {
        return false;   //dummy
    }
    virtual void SetObstacleFlag() {} //dummy
    virtual double GetVelocityLateral()
    {
        return 0;   //dummy
    }
    virtual void GetAgentsDirectlyInFront(double PeripheralPreviewDistance,
                                          AgentInterface *&agentFront,
                                          AgentInterface *&agentFrontLeft,
                                          AgentInterface *&agentFrontRight)
    {
        Q_UNUSED(PeripheralPreviewDistance);
        Q_UNUSED(agentFront);
        Q_UNUSED(agentFrontLeft);
        Q_UNUSED(agentFrontRight);
    } //dummy
    virtual double GetDistanceToSpecialAgent()
    {
        return 0;   //dummy
    }
    virtual bool IsObstacle()
    {
        return false;   //dummy
    }
    virtual double GetDistanceToEndOfLane(double sightDistance) const
    {
        Q_UNUSED(sightDistance);    //dummy
        return 0;
    }
    virtual bool PerceiveMinimumSpeedOfPlatoonInLaneLeft(double MesoscopicPreviewDistance,
                                                         int &iLane,
                                                         double &laneSpeedDifferential) const
    {
        Q_UNUSED(MesoscopicPreviewDistance);
        Q_UNUSED(iLane);
        Q_UNUSED(laneSpeedDifferential);
        return false;
    } //dummy
    virtual bool PerceiveMinimumSpeedOfPlatoonInLaneRight(double MesoscopicPreviewDistance,
                                                          int &iLane,
                                                          double &laneSpeedDifferential) const
    {
        Q_UNUSED(MesoscopicPreviewDistance);
        Q_UNUSED(iLane);
        Q_UNUSED(laneSpeedDifferential);
        return false;
    } //dummy
    virtual void ObtainGroundTruthObjectLaneExistences(AgentInterface *&agent,
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
    virtual double GetLaneDepartureFromLeftLaneBoundary()
    {
        return 0;   //dummy
    }
    virtual double GetLaneDepartureFromRightLaneBoundary()
    {
        return 0;   //dummy
    }
    virtual double GetVelocityAbsolute() const
    {
        return 0;   //dummy
    }
    virtual void SetCarInfo(CarInfo *carInfo)
    {
        Q_UNUSED(carInfo);   //dummy
    }
    virtual CarInfo *GetCarInfo() const
    {
        return nullptr;   //dummy
    }
    virtual double GetDistanceToEndOfRamp(int laneId)
    {
        Q_UNUSED(laneId);    //dummy
        return 0;
    }
    virtual double GetPositionLateral() const
    {
        return 0;   //dummy
    }
    virtual void SetCarInfoExtra(void *extraInfo)
    {
        Q_UNUSED(extraInfo)
    } //dummy
    virtual void *GetCarInfoExtra()
    {
        return nullptr;   //dummy
    }
    virtual void AssignCarInfo(double accSensDist)
    {
        Q_UNUSED(accSensDist)
    } //dummy
    virtual void AssignCarInfoExtra() {} //dummy
    virtual CarInfo *GenerateCarInfo()
    {
        return nullptr;   //dummy
    }
    virtual double GetDistanceFrontAgentToEgo()
    {
        return 0;   //dummy
    }
    virtual bool HasTwoLeftLanes()
    {
        return false;   //dummy
    }
    virtual bool HasTwoRightLanes()
    {
        return false;   //dummy
    }
    virtual LaneChangeState EstimateLaneChangeState(double thresholdLooming)
    {
        Q_UNUSED(thresholdLooming);    //dummy
        return LaneChangeState::NoLaneChange;
    }
    virtual std::list<AgentInterface *> GetAllAgentsInLane(int laneID,
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
    virtual bool IsBicycle() const
    {
        return false;   //dummy
    }
    virtual double GetLaneDirection() const
    {
        return 0;   //dummy
    }
    virtual void Unregister() const {} //dummy
    virtual bool IsFirstCarInLane() const
    {
        return false;   //dummy
    }

    MarkType GetTypeOfNearestMark() const
    {
        return MarkType::NONE;
    }//dummy
    virtual std::string GetTypeOfNearestMarkString() const
    {
        return "";
    }//dummy
    virtual double GetDistanceToNearestMark(MarkType markType) const
    {
        Q_UNUSED(markType);
        return INFINITY;
    }//dummy
    double GetOrientationOfNearestMark(MarkType markType) const
    {
        Q_UNUSED(markType);
        return INFINITY;
    }//dummy
    virtual double GetViewDirectionToNearestMark(MarkType markType) const
    {
        Q_UNUSED(markType);
        return INFINITY;
    }//dummy
    virtual AgentViewDirection GetAgentViewDirectionToNearestMark(MarkType markType) const
    {
        Q_UNUSED(markType);
        return AgentViewDirection::none;
    }//dummy
    virtual double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                           AgentViewDirection agentViewDirection) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        return INFINITY;
    }//dummy
    virtual double GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                           double mainViewDirection) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        return INFINITY;
    }//dummy
    virtual double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                              AgentViewDirection agentViewDirection) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        return INFINITY;
    }//dummy
    virtual double GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                              double mainViewDirection) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        return INFINITY;
    }//dummy
    double GetDistanceToNearestMarkInViewRange(MarkType markType, AgentViewDirection agentViewDirection,
                                               double range) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return INFINITY;
    }//dummy
    double GetDistanceToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                               double range) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return INFINITY;
    }//dummy
    double GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                  AgentViewDirection agentViewDirection, double range) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return INFINITY;
    }//dummy
    double GetOrientationOfNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                  double range) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return INFINITY;
    }//dummy
    double GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                    AgentViewDirection agentViewDirection, double range) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);
        return INFINITY;
    }//dummy
    double GetViewDirectionToNearestMarkInViewRange(MarkType markType, double mainViewDirection,
                                                    double range) const
    {
        Q_UNUSED(markType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);
        return INFINITY;
    }//dummy
    std::string GetTypeOfNearestObject(AgentViewDirection agentViewDirection,
                                       double range) const
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);

        return "";
    }//dummy
    std::string GetTypeOfNearestObject(double mainViewDirection,
                                       double range) const
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);

        return "";
    }//dummy
    double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                 AgentViewDirection agentViewDirection,
                                                 double range) const
    {
        Q_UNUSED(objectType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                 double mainViewDirection,
                                                 double range) const
    {
        Q_UNUSED(objectType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                      AgentViewDirection agentViewDirection,
                                                      double range) const
    {
        Q_UNUSED(objectType);
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                      double mainViewDirection,
                                                      double range) const
    {
        Q_UNUSED(objectType);
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    int GetIdOfNearestAgent(AgentViewDirection agentViewDirection,
                            double range) const
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);

        return -1;
    }//dummy
    int GetIdOfNearestAgent(double mainViewDirection,
                            double range) const
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);

        return -1;
    }//dummy
    double GetDistanceToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                double range) const
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetDistanceToNearestAgentInViewRange(double mainViewDirection,
                                                double range) const
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetViewDirectionToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                     double range) const
    {
        Q_UNUSED(agentViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetViewDirectionToNearestAgentInViewRange(double mainViewDirection,
                                                     double range) const
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetVisibilityToNearestAgentInViewRange(double mainViewDirection,
                                                  double range) const
    {
        Q_UNUSED(mainViewDirection);
        Q_UNUSED(range);

        return INFINITY;
    }//dummy
    double GetYawVelocity()
    {
        return 0;   //dummy
    }
    void SetYawVelocity(double yawVelocity)
    {
        Q_UNUSED(yawVelocity);   //dummy
    }
    double GetYawAcceleration()
    {
        return 0;   //dummy
    }
    void SetYawAcceleration(double yawAcceleration)
    {
        Q_UNUSED(yawAcceleration);   //dummy
    }
    const std::vector<int> *GetTrajectoryTime() const
    {
        return nullptr;    //dummy
    }
    const std::vector<double> *GetTrajectoryXPos() const
    {
        return nullptr;    //dummy
    }
    const std::vector<double> *GetTrajectoryYPos() const
    {
        return nullptr;    //dummy
    }
    const std::vector<double> *GetTrajectoryVelocity() const
    {
        return nullptr;    //dummy
    }
    const std::vector<double> *GetTrajectoryAngle() const
    {
        return nullptr;    //dummy
    }

    void SetAccelerationIntention(double accelerationIntention)
    {
        Q_UNUSED(accelerationIntention);   //dummy
    }

    double GetAccelerationIntention() const
    {
        return 0;   //dummy
    }
    void SetDecelerationIntention(double decelerationIntention)
    {
        Q_UNUSED(decelerationIntention);   //dummy
    }

    double GetDecelerationIntention() const
    {
        return 0;   //dummy
    }

    void SetAngleIntention(double angleIntention)
    {
        Q_UNUSED(angleIntention);   //dummy
    }

    double GetAngleIntention() const
    {
        return 0;   //dummy
    }

    void SetCollisionState(bool collisionState)
    {
        Q_UNUSED(collisionState);   //dummy
    }
    bool GetCollisionState() const
    {
        return false;   //dummy
    }
    double GetAccelerationAbsolute() const
    {
        return 0;//dummy
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
    double positionX = 0.0;         //!< x-coordinate of the agent
    double positionY = 0.0;         //!< y-coordinate of the agent
    double width = 0.0;             //!< width of the agent
    double length = 0.0;            //!< length of the agent
    double height = 0.0;            //!< height of the agent
    double velocityX = 0.0;         //!< lateral velocity of the agent
    double velocityY = 0.0;         //!< vertical velocity of the agent
    double distanceCOGtoLeadingEdge =
            0.0;      //!< distance center of gravity to leading edge of the agent
    double accelerationX = 0.0;     //!< lateral acceleration of the agent
    double accelerationY = 0.0;     //!< vertical acceleration of the agent
    double yawAngle = 0.0;          //!< yaw angle of the agent

    int id = -1;
    int agentTypeId = -1;
    bool isValid = true;
};

#endif // AGENT_H
