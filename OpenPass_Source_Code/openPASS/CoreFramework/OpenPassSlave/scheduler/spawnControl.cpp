/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  SpawnControl.cpp */
//-----------------------------------------------------------------------------

#include "commonTools.h"
#include "spawnControl.h"
#include "spawnPointNetwork.h"

#include "Common/boostGeometryCommon.h"
#include <boost/geometry/geometries/adapted/c_array.hpp>

namespace SimulationSlave {

SpawnControl::SpawnControl(SpawnPointNetworkInterface* spawnPointNetwork,
                           WorldInterface* world,
                           const int cycleTime) :
    spawnPointNetwork(spawnPointNetwork),
    world(world),
    cycleTime(cycleTime)
{
    SpawnAgentParameters spawnPointParams = {spawnPointNetwork->GetSpawnPoint(), nullptr, 0, 0};
    spawnPoints.push_back(spawnPointParams);
}

SpawnControl::~SpawnControl()
{
    for (auto& spawnAgentParams : spawnPoints)
    {
        if (spawnAgentParams.agentBlueprint != nullptr)
        {
            delete spawnAgentParams.agentBlueprint;
            spawnAgentParams.agentBlueprint = nullptr;
        }
    }
}

std::list<const Agent*> SpawnControl::PullNewAgents()
{
    std::list<const Agent*> agents = newAgents;
    newAgents.clear();

    return agents;
}

SpawnControlError SpawnControl::GetError() const
{
    return spawnError;
}

bool SpawnControl::Execute(int timestamp)
{
    spawnError = SpawnControlError::NoError;

    for (auto& spawnAgentParams : spawnPoints)
    {
        while (timestamp == spawnAgentParams.nextSpawnTime)
        {
            if (!Spawn(&spawnAgentParams, timestamp))
            {
                return false;
            }
        }
    }

    return true;
}

bool SpawnControl::Spawn(SpawnAgentParameters* spawnAgentParams, int timestamp)
{

    assert(nullptr != spawnAgentParams->spawnPoint->GetImplementation());

    AgentBlueprintInterface* agentBlueprint = spawnAgentParams->agentBlueprint;

    if (agentBlueprint != nullptr)
    {
        const SpawnParameter& spawnParameter = agentBlueprint->GetSpawnParameter();
        if (!isScenarioMember(spawnAgentParams))
        {
            bool adoptVelocitySuccessfull = true;
            if (spawnAgentParams->holdbackTime == 0 && timestamp > 0)
            {
                const int holdbackTime = CalculateHoldbackTime(agentBlueprint);

                //Check if agent needs to be held back
                if (holdbackTime > 0)
                {
                    spawnAgentParams->holdbackTime = holdbackTime;
                    spawnAgentParams->nextSpawnTime = timestamp + holdbackTime;
                    return true;
                }

                //Holdback would make no sense => Maybe adjust velocity
                if (holdbackTime < 0)
                {
                    adoptVelocitySuccessfull =  AdaptVelocityForAgentBlueprint(agentBlueprint);
                }
            }
            else //Agent was already held back => Maybe adjust velocity
            {
                adoptVelocitySuccessfull =  AdaptVelocityForAgentBlueprint(agentBlueprint);
            }

            if (!adoptVelocitySuccessfull)
            {
                LOG_INTERN(LogLevel::Warning) << "spawn point " << spawnAgentParams->spawnPoint->GetId() <<
                                              ": Could not keep time to collision. Disposed blueprint.";
            }
            else
                if (!InstantiateAgent(timestamp, *spawnAgentParams))
                {
                    return false;
                }
        }
        else
        {
            if (!InstantiateAgent(timestamp, *spawnAgentParams))
            {
                return false;
            }
        }

        delete agentBlueprint;
        agentBlueprint = nullptr;
    }

    return GenerateNextBlueprint(*spawnAgentParams, timestamp);
}

bool SpawnControl::GenerateNextBlueprint(SpawnAgentParameters& spawnAgentParams, int timestamp)
{
    AgentBlueprint* agentBlueprint = new AgentBlueprint;

    if (!spawnAgentParams.spawnPoint->GetLibrary()->GenerateAgent(spawnAgentParams.spawnPoint->GetImplementation(),
            agentBlueprint))
    {
        return false;
    }

    spawnAgentParams.nextSpawnTime = agentBlueprint->GetSpawnParameter().nextTimeOffset + timestamp;
    spawnAgentParams.agentBlueprint = agentBlueprint;
    spawnAgentParams.holdbackTime = 0;
    return true;
}

bool SpawnControl::AdaptVelocityForAgentBlueprint(AgentBlueprintInterface* agentBlueprint)
{
    constexpr double ttb = 1.0;
    constexpr double minSpawningDistance = 1.0;
    constexpr int assumedBrakeAccelerationEgo = -6;
    constexpr int aFront = -10;

    SpawnParameter& spawnParameter = agentBlueprint->GetSpawnParameter();
    const VehicleModelParameters& vehicleModelParameters = agentBlueprint->GetVehicleModelParameters();

    auto spawningDistance = spawnParameter.distance +
                            vehicleModelParameters.distanceReferencePointToLeadingEdge;

    const auto spawningRoadId = spawnParameter.SpawningRoadId;
    const auto spawningLaneId = spawnParameter.SpawningLaneId;

    auto vEgo = spawnParameter.velocity;
    auto fullBrakingDistance = vEgo * ttb + vEgo * vEgo / (2 * std::abs(assumedBrakeAccelerationEgo));

    auto opponentSearchDistance = spawningDistance - vehicleModelParameters.length;
    auto maxSearchDistance = spawningDistance + fullBrakingDistance;

    while (opponentSearchDistance <= maxSearchDistance)
    {
        const WorldObjectInterface* opponent = world->GetNextObjectInLane(spawningRoadId, spawningLaneId,
                                               opponentSearchDistance);
        if (!opponent)
        {
            const auto freeSpace = world->GetDistanceToEndOfDrivingLane(spawningRoadId, spawningLaneId, spawningDistance,
                                   maxSearchDistance);

            // free space too small - no rescue
            if (freeSpace <= minSpawningDistance)
            {
                return false;
            }

            if (freeSpace < fullBrakingDistance)
            {
                // reduce speed of spawning agent to guarantee TTB to end of lane
                while (vEgo > 0 &&
                        TrafficHelperFunctions::WillCrash(freeSpace, vEgo, assumedBrakeAccelerationEgo, 0, 0, ttb))
                {
                    vEgo -= 10 / 3.6;
                }
            }

            spawnParameter.velocity = std::max(0.0, vEgo);
            return true;
        }

        auto vFront = opponent->GetVelocity();
        auto freeSpace = opponent->GetDistanceToStartOfRoad(MeasurementPoint::Rear) - spawningDistance;

        // free space too small - no rescue
        if (freeSpace <= minSpawningDistance)
        {
            return false;
        }

        if (!drivingCorridorDoesNotOverlap(ttb, vFront, aFront, fullBrakingDistance, agentBlueprint, opponent)
                && TrafficHelperFunctions::WillCrash(freeSpace, vEgo, assumedBrakeAccelerationEgo, vFront, aFront, ttb))
        {
            while (vEgo > 0 &&
                    TrafficHelperFunctions::WillCrash(freeSpace, vEgo, assumedBrakeAccelerationEgo, vFront, aFront, ttb))
            {
                vEgo -= 10 / 3.6;
            }
            spawnParameter.velocity = std::max(0.0, vEgo);
        }

        opponentSearchDistance = opponent->GetDistanceToStartOfRoad(MeasurementPoint::Front);
    }

    return true;
}

int SpawnControl::CalculateHoldbackTime(AgentBlueprintInterface* agentBlueprint)
{
    constexpr double ttb = 1.0;
    constexpr double minSpawningDistance = 1.0;
    constexpr int assumedBrakeAccelerationEgo = -6;
    constexpr int aFront = -10;
    constexpr int maxHoldbackTime = 5000;

    const SpawnParameter& spawnParameter = agentBlueprint->GetSpawnParameter();
    const VehicleModelParameters& vehicleModelParameters = agentBlueprint->GetVehicleModelParameters();

    const auto spawningDistance = spawnParameter.distance +
                                  vehicleModelParameters.distanceReferencePointToLeadingEdge;

    const auto spawningRoadId = spawnParameter.SpawningRoadId;
    const auto spawningLaneId = spawnParameter.SpawningLaneId;

    const auto vEgo = spawnParameter.velocity;
    const auto fullBrakingDistance = vEgo * ttb + vEgo * vEgo / (2 * std::abs(assumedBrakeAccelerationEgo));

    auto opponentSearchDistance = spawningDistance - vehicleModelParameters.length;
    const auto maxSearchDistance = spawningDistance + fullBrakingDistance;

    int holdbackTime = 0;

    while (opponentSearchDistance <= maxSearchDistance)
    {
        const WorldObjectInterface* opponent = world->GetNextObjectInLane(spawningRoadId, spawningLaneId,
                                               opponentSearchDistance);

        if (!opponent)
        {
            if (fullBrakingDistance > world->GetDistanceToEndOfDrivingLane(spawningRoadId, spawningLaneId, spawningDistance,
                    maxSearchDistance))
            {
                return -1;
            }
            else
            {
                return holdbackTime;
            }
        }

        const auto freeSpace = opponent->GetDistanceToStartOfRoad(MeasurementPoint::Rear) - spawningDistance;

        if (freeSpace > fullBrakingDistance)
        {
            return holdbackTime;
        }

        const auto vFront = opponent->GetVelocity();

        if (vFront == 0)
        {
            return -1;
        }

        // free space too small - no rescue
        if (freeSpace <= minSpawningDistance)
        {
            return -1;
        }

        if (!drivingCorridorDoesNotOverlap(ttb, vFront, aFront, fullBrakingDistance, agentBlueprint, opponent)
                && TrafficHelperFunctions::WillCrash(freeSpace, vEgo, assumedBrakeAccelerationEgo, vFront, aFront, ttb))
        {

            double predictedDistanceToFrontVehicle = freeSpace + holdbackTime / 1000.0 * vFront;

            while (TrafficHelperFunctions::WillCrash(predictedDistanceToFrontVehicle, vEgo, assumedBrakeAccelerationEgo, vFront,
                    aFront, ttb)
                    && holdbackTime < maxHoldbackTime)
            {
                holdbackTime += cycleTime;
                predictedDistanceToFrontVehicle = freeSpace + holdbackTime / 1000.0 * vFront;
            }
        }

        opponentSearchDistance = opponent->GetDistanceToStartOfRoad(MeasurementPoint::Front);
    }

    return holdbackTime;
}

bool SpawnControl::InstantiateAgent(int timestamp, SpawnAgentParameters& spawnAgentParams)
{
    Agent* agent = spawnAgentParams.spawnPoint->GetAgentFactory()->AddAgent(spawnAgentParams.agentBlueprint, timestamp);

    if (!agent)
    {
        spawnError = SpawnControlError::AgentGenerationError;
        LOG_INTERN(LogLevel::Error) << "spawn point" << spawnAgentParams.spawnPoint->GetId() << ": Could not add agent";
        return false;
    }

    if (!agent->GetAgentAdapter()->IsAgentInWorld())
    {
        if (isScenarioMember(&spawnAgentParams))
        {
            LOG_INTERN(LogLevel::Error) << "spawn point " << spawnAgentParams.spawnPoint->GetId() <<
                                        ": Ego or scenario car outside world.";
            spawnError = SpawnControlError::IncompleteScenario;
            return false;
        }
        LOG_INTERN(LogLevel::Warning) << "spawn point" << spawnAgentParams.spawnPoint->GetId() << ": New agent outside world";
        return true;
    }

    newAgents.push_back(agent);
    return true;
}


bool drivingCorridorDoesNotOverlap(const double assumedTtb,
                                   const double vFront,
                                   const double aFront,
                                   const double fullBrakingDistanceEgo,
                                   AgentBlueprintInterface* egoAgentBlueprint,
                                   const WorldObjectInterface* opponent)
{
    const VehicleModelParameters& vehicleModelParameters = egoAgentBlueprint->GetVehicleModelParameters();
    const SpawnParameter& spawnParameter = egoAgentBlueprint->GetSpawnParameter();

    double egoFront = vehicleModelParameters.distanceReferencePointToLeadingEdge;
    double egoDrivingCorridorLength = fullBrakingDistanceEgo;
    double egoHalfWidth = vehicleModelParameters.width / 2;
    double egoRotation  = spawnParameter.yawAngle;
    double egoRear = vehicleModelParameters.distanceReferencePointToLeadingEdge
                     - vehicleModelParameters.length;

    Common::Vector2d egoPosition(spawnParameter.positionX,
                                 spawnParameter.positionY);

    double egoBoundaryPoints[][2]
    {
        {egoRear, -egoHalfWidth},
        {egoRear,  egoHalfWidth},
        {egoFront + egoDrivingCorridorLength, egoHalfWidth},
        {egoFront + egoDrivingCorridorLength, -egoHalfWidth},
        {egoRear, -egoHalfWidth}
    };

    polygon_t drivingCorridorTemp;

    polygon_t egoDrivingCorridor;
    bg::append(egoDrivingCorridor, egoBoundaryPoints);

    bt::rotate_transformer<bg::radian, double, 2, 2> egoRotate(-egoRotation);
    bg::transform(egoDrivingCorridor, drivingCorridorTemp, egoRotate);

    bt::translate_transformer<double, 2, 2> egoTranslate(egoPosition.x, egoPosition.y);
    bg::transform(drivingCorridorTemp, egoDrivingCorridor, egoTranslate);

    // estimate values for front vehicle at t=ttb
    auto stopTimeOpponent = aFront < 0 ? -vFront / aFront : std::numeric_limits<double>::max();
    auto tOpponent = std::min(stopTimeOpponent, assumedTtb);
    auto sOpponentAtTtb = std::max(0.0, vFront * tOpponent + aFront * tOpponent * tOpponent / 2);

    double opponentFront = opponent->GetDistanceReferencePointToLeadingEdge();
    double opponentDrivingCorridorLength =  sOpponentAtTtb;
    double opponentHalfWidth = opponent->GetWidth() / 2;
    double opponentRotation  = opponent->GetYaw();
    double opponentRear = opponent->GetDistanceReferencePointToLeadingEdge()
                          - opponent->GetLength();

    Common::Vector2d frontPosition(opponent->GetPositionX(),
                                   opponent->GetPositionY());

    double frontBoundaryPoints[][2]
    {
        {opponentRear, -opponentHalfWidth},
        {opponentRear,  opponentHalfWidth},
        {opponentFront + opponentDrivingCorridorLength, opponentHalfWidth},
        {opponentFront + opponentDrivingCorridorLength, -opponentHalfWidth},
        {opponentRear, -opponentHalfWidth}
    };

    polygon_t frontDrivingCorridor;
    bg::append(frontDrivingCorridor, frontBoundaryPoints);

    bt::rotate_transformer<bg::radian, double, 2, 2> frontRotate(-opponentRotation);
    bg::transform(frontDrivingCorridor, drivingCorridorTemp, frontRotate);

    bt::translate_transformer<double, 2, 2> frontTranslate(frontPosition.x, frontPosition.y);
    bg::transform(drivingCorridorTemp, frontDrivingCorridor, frontTranslate);

    return !bg::intersects(egoDrivingCorridor, frontDrivingCorridor);
}

bool isScenarioMember(const SpawnAgentParameters* spawnAgentParams)
{
    const AgentCategory& agentCategory = spawnAgentParams->agentBlueprint->GetAgentCategory();

    return agentCategory == AgentCategory::Ego ||
           agentCategory == AgentCategory::Scenario;
}

} //namespace SimulationSlave
