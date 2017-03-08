/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  globalDefinitions.h
//! @brief This file contains several classes for global purposes
//-----------------------------------------------------------------------------

#ifndef GLOBALDEFINITIONS
#define GLOBALDEFINITIONS

#include <string>

//-----------------------------------------------------------------------------
//! @brief Containing the three possible states regarding lane change
//-----------------------------------------------------------------------------
enum class LaneChangeState
{
    NoLaneChange = 0,
    LaneChangeLeft,
    LaneChangeRight
};

//-----------------------------------------------------------------------------
//! weekday type
//-----------------------------------------------------------------------------
enum class Weekday
{
    Undefined = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

//-----------------------------------------------------------------------------
//! Agent type classification
//-----------------------------------------------------------------------------
enum class AgentVehicleType
{
    Undefined = 0,
    Car,
    Pedestrian,
    Motorbike,
    Bicycle,
    Truck
};

//! State of indicator lever
enum class IndicatorLever {
    IndicatorLever_Off = 0,
    IndicatorLever_Left = 1,
    IndicatorLever_Right = -1
};

enum class IndicatorState {
    IndicatorState_Off = 0,
    IndicatorState_Left = 1,
    IndicatorState_Right = 2,
    IndicatorState_Warn = 3
};

struct Position{
    double xPos;
    double yPos;
    double yawAngle;
    double curvature;
};

//! @brief Areas of interest for surrounding data.
//! \details "Downstream" is a gaze state, but not an area of interest for surrounding data.
//! The downstream gaze state is used to obtain data for anticipation.
enum class AreaOfInterest
{
    LEFT_FRONT = 0,
    RIGHT_FRONT,
    LEFT_REAR,
    RIGHT_REAR,
    EGO_FRONT,
    EGO_REAR,
    LEFT_SIDE,
    RIGHT_SIDE,
    //	DOWNSTREAM
    NumberOfAreaOfInterests
};

class WorldParameter
{
public:
    WorldParameter(Weekday weekday,
                   int timeOfDay,
                   const std::string &libraryName) :
        weekday(weekday),
        timeOfDay(timeOfDay),
        libraryName(libraryName)
    {}
    WorldParameter(const WorldParameter&) = delete;
    WorldParameter(WorldParameter&&) = delete;
    WorldParameter& operator=(const WorldParameter&) = delete;
    WorldParameter& operator=(WorldParameter&&) = delete;
    virtual ~WorldParameter() = default;

    Weekday GetWeekday()
    {
        return weekday;
    }

    int GetTimeOfDay()
    {
        return timeOfDay;
    }

    const std::string &GetLibraryName() const
    {
        return libraryName;
    }

private:
    Weekday weekday;
    int timeOfDay;
    const std::string libraryName;
};

//-----------------------------------------------------------------------------
//! Representation of an agent as defined in the run configuration.
//-----------------------------------------------------------------------------
class AgentSpawnItem
{
public:
    AgentSpawnItem(int id, int reference) :
        id(id),
        reference(reference)
    {}
    AgentSpawnItem(const AgentSpawnItem&) = delete;
    AgentSpawnItem(AgentSpawnItem&&) = delete;
    AgentSpawnItem& operator=(const AgentSpawnItem&) = delete;
    AgentSpawnItem& operator=(AgentSpawnItem&&) = delete;
    virtual ~AgentSpawnItem() = default;

    int GetId() const
    {
        return id;
    }

    int GetReference() const
    {
        return reference;
    }

    AgentVehicleType GetVehicleType() const
    {
        return vehicleType;
    }

    double GetWidth() const
    {
        return width;
    }

    double GetLength() const
    {
        return length;
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

    void SetVehicleType(AgentVehicleType vehicleType)
    {
        this->vehicleType = vehicleType;
    }

    void SetWidth(double width)
    {
        this->width = width;
    }

    void SetLength(double length)
    {
        this->length = length;
    }

    void SetDistanceCOGtoFrontAxle(double distanceCOGtoFrontAxle)
    {
        this->distanceCOGtoFrontAxle = distanceCOGtoFrontAxle;
    }

    void SetWeight(double weight)
    {
        this->weight = weight;
    }

    void SetHeightCOG(double heightCOG)
    {
        this->heightCOG = heightCOG;
    }

    void SetWheelbase(double wheelbase)
    {
        this->wheelbase = wheelbase;
    }

    void SetMomentInertiaRoll(double momentInertiaRoll)
    {
        this->momentInertiaRoll = momentInertiaRoll;
    }

    void SetMomentInertiaPitch(double momentInertiaPitch)
    {
        this->momentInertiaPitch = momentInertiaPitch;
    }

    void SetMomentInertiaYaw(double momentInertiaYaw)
    {
        this->momentInertiaYaw = momentInertiaYaw;
    }

    void SetFrictionCoeff(double frictionCoeff)
    {
        this->frictionCoeff = frictionCoeff;
    }

    void SetTrackWidth(double trackWidth)
    {
        this->trackWidth = trackWidth;
    }

    void SetDistanceCOGtoLeadingEdge(double distanceCOGtoLeadingEdge)
    {
        this->distanceCOGtoLeadingEdge = distanceCOGtoLeadingEdge;
    }

private:
    int id;
    int reference;
    AgentVehicleType vehicleType;
    double positionX;
    double positionY;
    double width;
    double length;
    double velocityX;
    double velocityY;
    double distanceCOGtoFrontAxle;
    double weight;
    double heightCOG;
    double wheelbase;
    double momentInertiaRoll;
    double momentInertiaPitch;
    double momentInertiaYaw;
    double frictionCoeff;
    double trackWidth;
    double distanceCOGtoLeadingEdge;
    double accelerationX;
    double accelerationY;
    double yawAngle;
};

#endif // GLOBALDEFINITIONS

