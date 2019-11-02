/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*               2016, 2017, 2018, 2019 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  globalDefinitions.h
//! @brief This file contains several classes for global purposes
//-----------------------------------------------------------------------------

#ifndef GLOBALDEFINITIONS
#define GLOBALDEFINITIONS

#include <string>
#include <tuple>
#include <vector>
#include <map>

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
//! Agent category classification
//-----------------------------------------------------------------------------
enum class AgentCategory
{
    Ego = 0,
    Scenario,
    Common,
    Any
};

//-----------------------------------------------------------------------------
//! Agent type classification
//-----------------------------------------------------------------------------
enum class AgentVehicleType
{
    NONE = -2,
    Undefined = -1,
    Car = 0,
    Pedestrian,
    Motorbike,
    Bicycle,
    Truck
};

// convert a vehicle type name to VehicleType enum
inline AgentVehicleType GetAgentVehicleType(const std::string &strVehicleType)
{
    if (0 == strVehicleType.compare("Car"))
    {
        return AgentVehicleType::Car;
    }
    else if (0 == strVehicleType.compare("Pedestrian"))
    {
        return AgentVehicleType::Pedestrian;
    }
    else if (0 == strVehicleType.compare("Motorbike"))
    {
        return AgentVehicleType::Motorbike;
    }
    else if (0 == strVehicleType.compare("Bicycle"))
    {
        return AgentVehicleType::Bicycle;
    }
    else if (0 == strVehicleType.compare("Truck"))
    {
        return AgentVehicleType::Truck;
    }
    return AgentVehicleType::Undefined;
}

// convert a AgentVehicleType to VehicleType string
inline std::string GetAgentVehicleTypeStr(const AgentVehicleType &vehicleType)
{
    return (vehicleType == AgentVehicleType::Car) ? "Car" :
            (vehicleType == AgentVehicleType::Pedestrian) ? "Pedestrian" :
            (vehicleType == AgentVehicleType::Motorbike) ? "Motorbike" :
            (vehicleType == AgentVehicleType::Bicycle) ? "Bicycle" :
            (vehicleType == AgentVehicleType::Truck) ? "Truck" : "unknown type";
}

// convert a string of type code to VehicleType string
inline std::string GetAgentVehicleTypeStr(const std::string &vehicleTypeCode)
{
    try{
        AgentVehicleType vehicleType = static_cast<AgentVehicleType>(std::stoi(vehicleTypeCode));
        return GetAgentVehicleTypeStr(vehicleType);
    } catch(...) {
        return "unknown type";
    }
}

//! State of indicator lever
enum class IndicatorLever
{
    IndicatorLever_Off = 0,
    IndicatorLever_Left = 1,
    IndicatorLever_Right = -1
};

enum class IndicatorState
{
    IndicatorState_Off = 0,
    IndicatorState_Left = 1,
    IndicatorState_Right = 2,
    IndicatorState_Warn = 3
};

struct Position
{
    Position() {}
    Position(double x,
             double y,
             double yaw,
             double curvature):
        xPos(x),
        yPos(y),
        yawAngle(yaw),
        curvature(curvature) {}

    double xPos {0};
    double yPos {0};
    double yawAngle {0};
    double curvature {0};
};

struct RoadPosition
{
    RoadPosition() = default;
    RoadPosition(double s,
                 double t,
                 double hdg):
        s(s),
        t(t),
        hdg(hdg) {}

    double s {0};
    double t {0};
    double hdg {0};

    bool operator==(const RoadPosition& other) const
    {
        return s == other.s && t == other.t && hdg == other.hdg;
    }
};

struct GlobalRoadPosition
{
    GlobalRoadPosition() = default;
    GlobalRoadPosition(std::string roadId, int laneId, double s, double t, double hdg) :
        roadId{roadId},
        laneId{laneId},
        roadPosition(s, t, hdg)
    {}

    std::string roadId {};
    int laneId {-999};
    RoadPosition roadPosition {};
};

enum class MeasurementPoint
{
    Front,
    Reference,
    Rear
};

namespace CommonTrafficSign {
enum Type
{
    Undefined = 0,
    MaximumSpeedLimit = 274,
    MinimumSpeedLimit = 275,
    EndOfMaximumSpeedLimit = 278,
    EndOfMinimumSpeedLimit = 279,
    EndOffAllSpeedLimitsAndOvertakingRestrictions = 282,
    TownBegin = 310,
    TownEnd = 311,
    Zone30Begin = 2741, //274.1
    Zone30End = 2742, // 274.2
    TrafficCalmedDistrictBegin = 3251, // 325.1
    TrafficCalmedDistrictEnd = 3252, // 325.2
    EnvironmentalZoneBegin = 2701, // 270.1
    EnvironmentalZoneEnd = 2702, // 270.2
    OvertakingBanBegin = 276,
    OvertakingBanEnd = 280,
    OvertakingBanForTrucksBegin = 277,
    OvertakingBanForTrucksEnd = 281,
    RightOfWayBegin = 306,
    RightOfWayEnd = 307,
    RightOfWayNextIntersection = 301,
    Stop = 206,
    DoNotEnter = 267,
    HighWayBegin = 3301, // 330.1
    HighWayEnd = 3302, // 330.2
    HighWayExit = 333
};

struct Entity
{
    Type type;
    double distanceToStartOfRoad;
    double relativeDistance;
    double value;
};
}

//! Enum of potential types of marks.
enum class MarkType
{
    NONE,
    CONTINUOUS,
    INTERRUPTED_LONG,
    INTERRUPTED_SHORT,
    ROADSIDE,
    NumberOfMarkTypes
};

//! Enum of potential types of objects.
enum class ObjectType
{
    NONE,
    OBJECT,
    VIEWOBJECT,
    NumberOfObjectTypes
};

enum ObjectTypeOSI
{
    None    = 0x00, // default at initialization
    Vehicle = 0x01,
    Object  = 0x02,
    Any     = Vehicle | Object
};

enum class LightState
{
    Off = 0,
    LowBeam,
    FogLight,
    HighBeam,
    Flash,
    NumberOfLightStates
};

//! Possibile direction of view angle with agent in center.
enum class AgentViewDirection
{
    none,
    front,
    left,
    back,
    right,
    NumberOfCarViewDirections
};

enum class Side
{
    Left,
    Right
};

struct VehicleModelParameters
{
    // -----------------------------------------
    // full vehicle related parameters
    // -----------------------------------------

    // The type of the vehicle
    AgentVehicleType vehicleType = AgentVehicleType::Undefined;
    // The maximum width of the vehicle in m
    double width = -999.0;
    // The maximum length of the vehicle in m
    double length = -999.0;
    // The maximum height of the vehicle in m
    double height = -999.0;
    // The wheelbase of the vehicle in m
    double wheelbase = -999.0;
    // The trackwidth of the vehicle in m
    double trackwidth = -999.0;
    // The distance between the vehicle coordinate system's reference point (rear axle) and the front bumper in m
    double distanceReferencePointToLeadingEdge = -999.0;
    // The distance between the vehicle coordinate system's reference point (rear axle) and the front axle in m
    double distanceReferencePointToFrontAxle = -999.0;
    // The maximum velocity of the vehicle in m/s
    double maxVelocity = -999.0;
    // The overall mass of the vehicle in kg
    double weight = -999.0;
    // The height of the center of gravity above ground in m
    double heightCOG = -999.0;
    // The moment of inertia along the vehicle's longtudinal axes in kgm2
    double momentInertiaRoll = -999.0;
    // The moment of inertia along the vehicle's lateral axes in kgm2
    double momentInertiaPitch = -999.0;
    // The moment of inertia along the vehicle's vertical axes in kgm2
    double momentInertiaYaw = -999.0;
    // The projected front surface of the vehicle in m2
    double frontSurface = -999.0;
    // The air drag coefficient of the vehicle
    double airDragCoefficient = -999.0;

    // -----------------------------------------
    // power train related parameters
    // -----------------------------------------

    // The idle speed of the engine in 1/min
    double minimumEngineSpeed = -999.0;
    // The maximum engine speed in 1/min
    double maximumEngineSpeed = -999.0;
    // The drag torque of the engine in Nm
    double minimumEngineTorque = -999.0;
    // The maximum torque of the engine in Nm
    double maximumEngineTorque = -999.0;
    // The number of gears in the gearbox (no reverse gear)
    int numberOfGears = -999;
    // The ratios of all gears in the gearbox (no reverse gear)
    std::vector<double> gearRatios;
    // The ratio of the axle gear
    double axleRatio = -999.0;
    // The deceleration caused by the overall powertrain drag torque in m/s2
    double decelerationFromPowertrainDrag = -999.0;

    // -----------------------------------------
    // steering related parameters
    // -----------------------------------------

    // The ratio of the steering gear
    double steeringRatio = -999.0;
    // The maximum amplitude of the steering wheel angle in degree
    double maximumSteeringWheelAngleAmplitude = -999.0;
    // The maximum curavture the vehicle is able to drive in 1/m
    double maxCurvature = -999.0;

    // -----------------------------------------
    // wheel related parameters
    // -----------------------------------------

    // The static wheel radius in m
    double staticWheelRadius = -999.0;
    // The friction coefficient between road and the vehicles tires
    double frictionCoeff = -999.0;
};

enum class AdasType
{
    Safety = 0,
    Comfort,
    Undefined
};

const std::map<AdasType, std::string> adasTypeToString = { { AdasType::Safety,    "Safety" },
    { AdasType::Comfort,   "Comfort" },
    { AdasType::Undefined, "Undefined" }
};

enum class ComponentType
{
    Driver = 0,
    TrajectoryFollower,
    VehicleComponent,
    Undefined
};

enum class LaneCategory
{
    Undefined = 0,
    RegularLane,
    RightMostLane
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
    WorldParameter(const WorldParameter &) = delete;
    WorldParameter(WorldParameter &&) = delete;
    WorldParameter &operator=(const WorldParameter &) = delete;
    WorldParameter &operator=(WorldParameter &&) = delete;
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
    AgentSpawnItem(const AgentSpawnItem &) = delete;
    AgentSpawnItem(AgentSpawnItem &&) = delete;
    AgentSpawnItem &operator=(const AgentSpawnItem &) = delete;
    AgentSpawnItem &operator=(AgentSpawnItem &&) = delete;
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
