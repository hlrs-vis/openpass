/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  SpawnPoint.cpp */
//-----------------------------------------------------------------------------

#include "SpawnPoint.h"

//Constructor for testing only
SpawnPoint::SpawnPoint(WorldInterface* world,
                       const ParameterInterface* parameters,
                       AgentBlueprintProviderInterface* agentBlueprintProvider,
                       SamplerInterface* sampler,
                       ScenarioInterface* scenario,
                       SpawnPointState spawnPointState) :
    SpawnPointInterface(world, parameters, nullptr),
    scenario(scenario),
    sceneryParser(*world, scenario->GetEgoEntity().spawnInfo.roadId),
    agentBlueprintProvider(agentBlueprintProvider),
    sampler(sampler),
    spawnPointState(spawnPointState)
{
}

SpawnPoint::SpawnPoint(WorldInterface* world,
                       const ParameterInterface* parameters,
                       const CallbackInterface*  callbacks,
                       AgentBlueprintProviderInterface *agentBlueprintProvider,
                       SamplerInterface* sampler,
                       ScenarioInterface* scenario):
    SpawnPointInterface(world, parameters, callbacks),
    scenario(scenario),
    sceneryParser(*world, scenario->GetEgoEntity().spawnInfo.roadId),
    agentBlueprintProvider(agentBlueprintProvider),
    sampler(sampler)
{
    try
    {
        ExtractSpawnPointParameters(parameters);
        sceneryParser.Parse(egoSpawnParameters);
        //CalculateDerivedParameters();
    }
    catch (...)
    {
        const std::string msg = COMPONENTNAME + " could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}

inline double SpawnPoint::ConvertPerHourToPerSecond(double perHourValue) const
{
    return perHourValue / 3600.0;
}

void SpawnPoint::ExtractSpawnPointParameters(const ParameterInterface* parameters)
{
    //ExtractEgoParameter
    egoSpawnParameters.spawnRadius = spawnPointConstants.egoSpawnRadius;

    SpawnInfo egoSpawnInfo = scenario->GetEgoEntity().spawnInfo;
    egoSpawnParameters.egoRoadId    = egoSpawnInfo.roadId;
    egoSpawnParameters.egoLaneId    = egoSpawnInfo.ILane;
    egoSpawnParameters.offset       = egoSpawnInfo.offset;
    egoSpawnParameters.acceleration = egoSpawnInfo.acceleration;
    egoSpawnParameters.s            = egoSpawnInfo.s;
    if (egoSpawnParameters.s.value < 0)
    {
        egoSpawnParameters.s.value = 0;
    }

    //Parse traffic parameter
    auto doubleParameter = parameters->GetParametersDouble();

    spawnPointParameters.carsPerSecond           = ConvertPerHourToPerSecond(doubleParameter.at("TrafficVolume"));
    spawnPointParameters.platoonRate             = doubleParameter.at("PlatoonRate");
    spawnPointParameters.trafficVelocity         = doubleParameter.at("Velocity");
    spawnPointParameters.homogenity              = doubleParameter.at("Homogenity");
}

void SpawnPoint::CalculateDerivedParameters()
{

}

double SpawnPoint::GetCarRate()
{
    if (spawnPointState == SpawnPointState::CommonSpawning)
    {
        return spawnPointParameters.carsPerSecond * static_cast<double>
               (sceneryParser.GetEntryLanes().size());
    }

    return spawnPointParameters.carsPerSecond;
}

double SpawnPoint::CalculateGap()
{
    double carRatePerSecond = GetCarRate();
    double gapInSeconds = sampler->RollGapBetweenCars(carRatePerSecond);

    if (!sampler->RollFor(spawnPointParameters.platoonRate))
    {
        gapInSeconds += sampler->RollGapExtension(spawnPointConstants.nonPlatoonGapExtension);
    }

    return gapInSeconds;
}

double SpawnPoint::GetValueForLane(int laneId, const std::vector<double>& values)
{
    size_t laneIdx = static_cast<size_t>(std::abs(laneId) - 1);
    return laneIdx > (values.size() - 1) ? values.back() : values[laneIdx];
}

double SpawnPoint::GetSeparation(double gapInSeconds, double egoVelocity, double consideredCarLengths)
{
    double buffer = 2.0 + consideredCarLengths + spawnPointConstants.minimumGapInSeconds * egoVelocity;

    double separation = std::max(buffer, (gapInSeconds * egoVelocity) + consideredCarLengths);

    return separation;
}

SpawnInfo SpawnPoint::GetEgoSpawnCarInfo(int laneId, double egoVelocity, double agentRearLength)
{
    if (egoSpawnParameters.acceleration.value == -999)
    {
        egoSpawnParameters.acceleration.value = 0.0;
    }
    else
    {
        CalculateAttributeValue(egoSpawnParameters.acceleration);
    }

    if (AreSpawningCoordinatesValid(egoSpawnParameters.egoRoadId, laneId, egoSpawnParameters.s.value + agentRearLength, egoSpawnParameters.offset.value))
    {
        return SpawnInfo(0, egoSpawnParameters.s.value + agentRearLength, egoVelocity, egoSpawnParameters.egoRoadId, laneId, egoSpawnParameters.offset.value,
                         egoSpawnParameters.acceleration.value);
    }
    else
    {
        LogError("Could not place ego at distance: " + std::to_string(egoSpawnParameters.s.value + agentRearLength) +
                 " on lane: " + std::to_string(laneId));
        if (AreSpawningCoordinatesValid(egoSpawnParameters.egoRoadId, laneId, agentRearLength, egoSpawnParameters.offset.value))
        {
            LogError("Placed ego instead at distance: 0");
            egoSpawnParameters.s.value = 0;
            return SpawnInfo(0, agentRearLength, egoVelocity, egoSpawnParameters.egoRoadId, laneId, egoSpawnParameters.offset.value,
                             egoSpawnParameters.acceleration.value);
        }

        throw std::runtime_error("Could not place Ego");
    }
}

SpawnInfo SpawnPoint::GetScenerySpawnCarInfo(int laneId, double egoVelocity, double rearCarLength)
{
    SpawnInfo scenerySpawnInfo = scenario->GetScenarioEntities().at(scenarioAgentIterator)->spawnInfo;

    double spawnVelocity = scenerySpawnInfo.velocity.value;    
    if (spawnVelocity == -999)
    {
        spawnVelocity = egoVelocity;
    }

    if (scenerySpawnInfo.acceleration.value == -999)
    {
        scenerySpawnInfo.acceleration.value = 0.0;
    }
    else
    {
        CalculateAttributeValue(scenerySpawnInfo.acceleration);
    }

    double start = sceneryCarCurrentSPosition;
    double offset = sceneryCarCurrentOffset;

    if (start == -1)
    {
        start = scenerySpawnInfo.s.value;
        offset = scenerySpawnInfo.offset.value;
    }

    SpawnInfo spawnInfo(scenerySpawnInfo.TStart,
                        start + rearCarLength,
                        spawnVelocity,
                        scenerySpawnInfo.roadId,
                        laneId,
                        offset,
                        scenerySpawnInfo.acceleration.value
                       );
    scenarioAgentIterator++;

    return spawnInfo;
}

SpawnInfo SpawnPoint::GetNextSpawnCarInfoFront(LaneParameters laneParameters, double gapInSeconds, double egoVelocity,
        double rearLength)
{
    const std::string roadId = egoSpawnParameters.egoRoadId;
    const auto& referenceObject = GetWorld()->GetLastObjectInLane(roadId, laneParameters.laneId,
                                                                  egoSpawnParameters.s.value - egoSpawnParameters.spawnRadius);

    if (referenceObject == nullptr)
    {
        const AgentInterface* egoAgent = GetWorld()->GetEgoAgent();
        if (egoAgent == nullptr)
        {
            throw std::runtime_error("No ego car was placed!");
        }
        double position = egoAgent->GetDistanceToStartOfRoad() + egoSpawnParameters.spawnRadius;
        return SpawnInfo(GetPreRunTimeTillSpawn(), position, egoVelocity, roadId, laneParameters.laneId, laneParameters.offset, 0);
    }

    //CalculateSpawnDistance
    double rearAgentDistance = referenceObject->GetDistanceToStartOfRoad();
    double rearAgentDistanceReferencePointToLeadingEdge = referenceObject->GetDistanceReferencePointToLeadingEdge();

    double seperation = GetSeparation(gapInSeconds,
                                      referenceObject->GetVelocity(),
                                      rearLength + rearAgentDistanceReferencePointToLeadingEdge);

    // minimum distance to a slow or standing obstacle = delta velocity in meter
    seperation = std::max(seperation, std::abs(egoVelocity - referenceObject->GetVelocity()) * 3.6);

    double minSpawnDistance = egoSpawnParameters.s.value - egoSpawnParameters.spawnRadius;
    double spawnDistance = std::max(rearAgentDistance + seperation, minSpawnDistance);

    return SpawnInfo(GetPreRunTimeTillSpawn(), spawnDistance, egoVelocity, roadId, laneParameters.laneId, laneParameters.offset, 0);
}

SpawnInfo SpawnPoint::GetNextSpawnCarInfoRear(LaneParameters laneParameters, double gapInSeconds, double egoVelocity,
        double distanceReferencePointToLeadingEdge)
{
    const std::string roadId = egoSpawnParameters.egoRoadId;
    const int laneId = laneParameters.laneId;

    const auto& referenceObject = GetWorld()->GetFarthestObjectInUpstream(roadId, laneParameters.laneId,
                                                                   egoSpawnParameters.s.value + egoSpawnParameters.spawnRadius);

    if (referenceObject == nullptr)
    {
        const AgentInterface* egoAgent = GetWorld()->GetEgoAgent();
        if (egoAgent == nullptr)
        {
            throw std::runtime_error("No ego car was placed!");
        }
        double position = std::max(egoAgent->GetDistanceToStartOfRoad() - egoSpawnParameters.spawnRadius, 0.0);
        return SpawnInfo(GetPreRunTimeTillSpawn(), position, egoVelocity, roadId, laneId, 0, 0);
    }
    double frontAgentDistance = referenceObject->GetDistanceToStartOfRoad();
    double frontAgentRearLength = referenceObject->GetLength() - referenceObject->GetDistanceReferencePointToLeadingEdge();
    double seperation = GetSeparation(gapInSeconds,
                                      egoVelocity,
                                      frontAgentRearLength + distanceReferencePointToLeadingEdge);

    double maxSpawnDistance = egoSpawnParameters.s.value + egoSpawnParameters.spawnRadius;
    double spawnDistance = std::min(frontAgentDistance - seperation, maxSpawnDistance);

    return SpawnInfo(GetPreRunTimeTillSpawn(), spawnDistance, egoVelocity, roadId, laneId, laneParameters.offset, 0);
}

SpawnInfo SpawnPoint::GetNextSpawnCarInfoPreRunRegular(LaneParameters laneParameters, double gapInSeconds,
        double egoVelocity, double distanceReferencePointToLeadingEdge)
{
    const std::string roadId = egoSpawnParameters.egoRoadId;
    double spawnDistance = 0;

    const auto firstObjectDownstream = GetWorld()->GetFirstObjectDownstream(laneParameters.streamId);

    if (firstObjectDownstream == nullptr ||
            firstObjectDownstream->GetDistanceToStartOfRoad(MeasurementPoint::Rear) > egoSpawnParameters.s.value +
            egoSpawnParameters.spawnRadius)
    {
        spawnDistance = egoSpawnParameters.s.value + egoSpawnParameters.spawnRadius;
        while (spawnDistance > egoSpawnParameters.s.value - egoSpawnParameters.spawnRadius)
        {
            if (GetWorld()->GetDistanceToEndOfDrivingLane(roadId, laneParameters.laneId, spawnDistance, egoSpawnParameters.spawnRadius) > spawnPointConstants.minimumDistanceToEndOfLane)
            {
                break;
            }
            spawnDistance -= 50.0;
        }
    }
    else
    {
        double frontCarDistance = firstObjectDownstream->GetDistanceToStartOfRoad();
        double frontCarRearLength = firstObjectDownstream->GetLength() -
                                    firstObjectDownstream->GetDistanceReferencePointToLeadingEdge();
        double seperation = GetSeparation(gapInSeconds,
                                          egoVelocity,
                                          frontCarRearLength + distanceReferencePointToLeadingEdge);

        // this might be a wrong distance
        spawnDistance = frontCarDistance - seperation;
    }

    int laneId = -999;
    if (spawnDistance >= 0)
    {
        laneId = GetWorld()->GetLaneId(laneParameters.streamId, spawnDistance);
        assert(laneId != -999); // must be valid if SpawnDistance calculation is right
    }

    return SpawnInfo(GetPreRunTimeTillSpawn(), spawnDistance, egoVelocity, roadId, laneId, laneParameters.offset, 0);
}

SpawnInfo SpawnPoint::GetNextSpawnCarInfoRegular(std::string roadId, int laneId, double gapInSeconds, double egoVelocity,
        double rearCarLength, double distanceReferencePointToLeadingEdge)
{
    double timeTillSpawn;

    const auto& referenceObject = GetWorld()->GetNextObjectInLane(roadId, laneId, 0.0); //get first in downstream
    if (referenceObject)
    {
        double frontCarDistance = referenceObject->GetDistanceToStartOfRoad();
        double frontCarRearLength = referenceObject->GetLength() - referenceObject->GetDistanceReferencePointToLeadingEdge();
        double seperation = GetSeparation(gapInSeconds,
                                          egoVelocity,
                                          frontCarRearLength + distanceReferencePointToLeadingEdge);
        double spawnDistance = frontCarDistance - seperation;

        timeTillSpawn = (spawnPointConstants.minimumDistanceToStart - spawnDistance) / egoVelocity;
        if (timeTillSpawn < 0)
        {
            timeTillSpawn = 0.1;
        }
    }
    else
    {
        timeTillSpawn = gapInSeconds;
    }

    return SpawnInfo(timeTillSpawn,
                     spawnPointConstants.minimumDistanceToStart + rearCarLength + 1.0,
                     egoVelocity,
                     roadId,
                     laneId, 0, 0);
}

SpawnInfo SpawnPoint::GetNextSpawnCarInfoRespawn(LaneParameters laneParameters, double gapInSeconds, double egoVelocity,
        double frontCarRearLength, double distanceReferencePointToLeadingEdge)
{
    int laneId = laneParameters.laneId;
    AgentInterface* egoAgent = GetWorld()->GetEgoAgent();

    if (egoAgent)
    {
        double velocitySpecialagent = egoAgent->GetVelocity();

        if (egoVelocity < velocitySpecialagent)
        {
            return GetNextSpawnCarInfoRear(laneParameters, gapInSeconds, egoVelocity, distanceReferencePointToLeadingEdge);
        }
        else
        {
            return GetNextSpawnCarInfoFront(laneParameters, gapInSeconds, egoVelocity, frontCarRearLength);
        }
    }

    //Respawn is handled like common spawning and therefore based on the initial ego RoadId
    std::string roadId = egoSpawnParameters.egoRoadId;
    return GetNextSpawnCarInfoRegular(roadId, laneId, gapInSeconds, egoVelocity, frontCarRearLength,
                                      distanceReferencePointToLeadingEdge);

}

bool SpawnPoint::GetNextSpawnCarInfo(LaneParameters& laneParameters, AgentBlueprintInterface* agentBlueprint)
{
    const std::string roadId = egoSpawnParameters.egoRoadId;
    const int laneId = laneParameters.laneId;

    const double agentLength = agentBlueprint->GetVehicleModelParameters().length;
    const double agentFrontLength = agentBlueprint->GetVehicleModelParameters().distanceReferencePointToLeadingEdge;
    const double agentRearLength = agentLength - agentFrontLength;

    const double gapInSeconds  = CalculateGap();
    const double egoVelocity   = CalculateEgoVelocity(laneId, laneParameters.distanceToLaneStart);

    currentBluePrint = agentBlueprint;

    switch (spawnPointState)
    {
        case SpawnPointState::EgoPlacement:
            sceneryParser.PushEgoParameters(laneParameters);

            spawnInfo = GetEgoSpawnCarInfo(laneId, egoVelocity, agentRearLength);
            UpdateSpawnPointState();
            break;

        case SpawnPointState::SceneryPlacement:
            spawnInfo = GetScenerySpawnCarInfo(laneId, egoVelocity, agentRearLength);
            if (!AreSpawningCoordinatesValid(spawnInfo.roadId, spawnInfo.ILane, spawnInfo.s.value, spawnInfo.offset.value))
            {
                throw (std::logic_error("Coordinates of scenery agent are invalid."));
            }

            sceneryParser.PushSceneryParameters(laneParameters);

            if (IsSceneryCompleted())
            {
                UpdateSpawnPointState();
            }
            break;

        case SpawnPointState::CommonSpecificFrontPlacement:
            spawnInfo = GetNextSpawnCarInfoFront(laneParameters, gapInSeconds, egoVelocity, agentRearLength);
            if (!AreSpawningCoordinatesValid(spawnInfo.roadId, spawnInfo.ILane, spawnInfo.s.value, spawnInfo.offset.value) ||
                    !IsWithinEgoRadius(spawnInfo.s.value))
            {
                LogCurrentDebugStatus("Next spawncar info front invalid.");

                egoAndSceneryLanesIndex++;

                if (egoAndSceneryLanesIndex >= sceneryParser.GetEgoAndSceneryParameters().size())
                {
                    UpdateSpawnPointState();
                    egoAndSceneryLanesIndex = 0;
                }
                return false;
            }
            break;

        case SpawnPointState::CommonSpecificRearPlacement:
            spawnInfo = GetNextSpawnCarInfoRear(laneParameters, gapInSeconds, egoVelocity, agentFrontLength);
            if (!AreSpawningCoordinatesValid(spawnInfo.roadId, spawnInfo.ILane, spawnInfo.s.value, spawnInfo.offset.value) ||
                    !IsWithinEgoRadius(spawnInfo.s.value))
            {
                LogCurrentDebugStatus("Next spawncar info rear invalid.");

                egoAndSceneryLanesIndex++;
                if (egoAndSceneryLanesIndex >= sceneryParser.GetEgoAndSceneryParameters().size())
                {
                    UpdateSpawnPointState();
                    egoAndSceneryLanesIndex = 0;
                }

                return false;
            }
            break;

        case SpawnPointState::CommonPreRunPlacement:
            spawnInfo = GetNextSpawnCarInfoPreRunRegular(laneParameters, gapInSeconds, egoVelocity, agentFrontLength);

            if (!AreSpawningCoordinatesValid(spawnInfo.roadId, spawnInfo.ILane, spawnInfo.s.value, spawnInfo.offset.value) ||
                    spawnInfo.s.value < laneParameters.distanceToLaneStart)
            {
                LogCurrentDebugStatus("Next spawncar info pre run regular invalid.");

                RemoveLastPlacementLane();
                if (sceneryParser.GetCommonLaneStreamParameters().empty())
                {
                    UpdateSpawnPointState();
                }

                return false;
            }
            break;

        case SpawnPointState::CommonSpawning:
            spawnInfo = GetNextSpawnCarInfoRegular(roadId, laneId, gapInSeconds, egoVelocity, agentRearLength, agentFrontLength);
            break;

        case SpawnPointState::Respawning:
            spawnInfo = GetNextSpawnCarInfoRespawn(laneParameters, gapInSeconds, egoVelocity, agentRearLength, agentFrontLength);

            if (isFirstRespawning)
            {
                isFirstRespawning = false;
                spawnInfo.TStart = -1;
            }
            break;
    }

    return true;
}

void SpawnPoint::SetPredefinedParameterHelper(AgentBlueprintInterface* agentBlueprint, AgentCategory agentCategory, const ScenarioEntity& scenarioEntity)
{
    const SpawnInfo& spawnInfo = scenarioEntity.spawnInfo;
    SpawnParameter spawnParameter;

    spawnParameter.velocity = spawnInfo.velocity.value;
    spawnParameter.SpawningLaneId = spawnInfo.ILane;
    spawnParameter.SpawningRoadId = spawnInfo.roadId;
    spawnParameter.positionX = spawnInfo.s.value;
    spawnParameter.heading = spawnInfo.heading;

    agentBlueprint->SetAgentCategory(agentCategory);
    agentBlueprint->SetObjectName(scenarioEntity.name);
    agentBlueprint->SetSpawnParameter(spawnParameter);
}

void SpawnPoint::SetPredefinedParameter(AgentBlueprintInterface* agentBluePrint)
{
    ScenarioEntity scenarioEntity;

    switch (spawnPointState)
    {
        case SpawnPointState::EgoPlacement:
            scenarioEntity = scenario->GetEgoEntity();
            SetPredefinedParameterHelper(agentBluePrint, AgentCategory::Ego, scenarioEntity);
            LogCurrentDebugStatus(" spawn ego vehicle");
            break;

        case SpawnPointState::SceneryPlacement:
            if (scenario->GetScenarioEntities().size() > 0)
            {
                scenarioEntity = *(scenario->GetScenarioEntities().at(scenarioAgentIterator));
                SetPredefinedParameterHelper(agentBluePrint, AgentCategory::Scenario, scenarioEntity);
                LogCurrentDebugStatus(" spawn scenario vehicle");
            }
            break;

        default:
            agentBluePrint->SetAgentCategory(AgentCategory::Common);
            LogCurrentDebugStatus(" spawn common vehicle");
            break;
    }
}

double SpawnPoint::CalculateEgoVelocity(int laneId, double distance)
{
    switch (spawnPointState)
    {
        case SpawnPointState::EgoPlacement:
        {           
            auto egoVelocity = scenario->GetEgoEntity().spawnInfo.velocity;
            CalculateAttributeValue(egoVelocity);
            if (egoVelocity.value != -999)
            {
                return egoVelocity.value;
            }
            break;
        }

        case SpawnPointState::SceneryPlacement:
        {
            SpawnAttribute scenarioVelocity = scenario->GetScenarioEntities().at(scenarioAgentIterator)->spawnInfo.velocity;
            CalculateAttributeValue(scenarioVelocity);
            if (scenarioVelocity.value != -999)
            {
                return scenarioVelocity.value;
            }
            break;
        }

        default:
            break;
    }

    const auto meanVelocities = CalculateMeanVelocities(scenario->GetEgoEntity().spawnInfo.roadId, distance);
    const auto meanVelocity = GetValueForLane(laneId, meanVelocities);
    const auto stdDeviationVelocity = spawnPointConstants.stdDeviationVelocity;
    double egoVelocity = sampler->RollForVelocity(meanVelocity, stdDeviationVelocity);

    egoVelocity = ReviseLowerVelocityLimit(egoVelocity, meanVelocity, stdDeviationVelocity);
    egoVelocity = ReviseUpperVelocityLimit(egoVelocity, laneId);

    return egoVelocity;
}

std::vector<double> SpawnPoint::CalculateMeanVelocities(std::string roadId, double distance)
{
    std::vector<double> meanVelocities;
    int numberOfLanes = GetWorld()->GetNumberOfLanes(roadId, distance);
    std::vector<int> drivingLanes = GetWorld()->GetDrivingLanesAtDistance(roadId, distance);
    double velocity = spawnPointParameters.trafficVelocity;
    for (int curLaneId = -numberOfLanes; curLaneId < 0; curLaneId++)
    {
        if (std::find(drivingLanes.begin(), drivingLanes.end(), curLaneId) != drivingLanes.end())
        {
            meanVelocities.insert(meanVelocities.begin(), velocity);
            velocity *= 2 - spawnPointParameters.homogenity;
        }
        else
        {
            if (std::find_if(drivingLanes.begin(), drivingLanes.end(),
                             [curLaneId](double laneId) -> bool {return laneId > curLaneId;})
                    != drivingLanes.end())
            {
                // use mean velocity of next leftside driving lane
                meanVelocities.insert(meanVelocities.begin(), velocity);
            }
            else
            {
                // use mean velocity of next rightside driving lane
                meanVelocities.insert(meanVelocities.begin(), velocity / (2 - spawnPointParameters.homogenity));
            }
        }
    }
    return meanVelocities;
}

double SpawnPoint::ReviseLowerVelocityLimit(double velocity, double mean, double stdDeviation)
{
    auto lowerLimit =  mean - 3 * stdDeviation;
    return std::max({0.0, velocity, lowerLimit});
}

double SpawnPoint::ReviseUpperVelocityLimit(double velocity, int laneId)
{
    auto maximumVelocity = GetValueForLane(laneId, spawnPointConstants.maximumVelocityPerLane);
    return std::min(velocity, maximumVelocity);
}

bool SpawnPoint::CalculateSpawnParameter(AgentBlueprintInterface* agentBlueprint, LaneParameters laneParameters)
{
    SpawnParameter& spawnParameter = agentBlueprint->GetSpawnParameter();
    spawnParameter.SpawningRoadId = spawnInfo.roadId;
    spawnParameter.SpawningLaneId = spawnInfo.ILane;

    double distance = spawnInfo.s.value - laneParameters.distanceToLaneStart;
    Position pos = GetWorld()->GetPositionByDistanceAndLane(distance, laneParameters.offset, spawnInfo.roadId, laneParameters.laneId);
    LogCurrentDebugStatus(" spawn item at x=" + std::to_string(pos.xPos) + ", y=" + std::to_string(pos.yPos));

    double spawnV = spawnInfo.velocity.value;

    if(agentBlueprint->GetVehicleModelParameters().vehicleType == AgentVehicleType::Truck)
    {
        spawnV = std::min(90 / 3.6, spawnV);
    }

    //considers adjusted velocity in curvatures
    double kappa = pos.curvature;

    // Note: This could falsify ego and scenario agents
    if (kappa != 0.0)
    {
        double curvatureVelocity;

        curvatureVelocity = 160 * (1 / (std::sqrt((std::abs(kappa)) / 1000)));

        spawnV = std::min(spawnV, curvatureVelocity);
    }

    spawnParameter.distance = spawnInfo.s.value;
    spawnParameter.positionX = pos.xPos;
    spawnParameter.positionY = pos.yPos;
    spawnParameter.yawAngle  = pos.yawAngle + agentBlueprint->GetSpawnParameter().heading;
    spawnParameter.velocity = spawnV;
    spawnParameter.acceleration = spawnInfo.acceleration.value;
    spawnParameter.nextTimeOffset = ::round(spawnInfo.TStart * 1000 / spawnPointConstants.standardCycleTime) *
                                    spawnPointConstants.standardCycleTime;

    return true;
}

bool SpawnPoint::IsLaneParameterAlreadyListed(LaneParameters laneParameters)
{
    for (const auto& storedParameters : sceneryParser.GetEgoAndSceneryParameters()) //egoAndSceneryLaneParameters
    {
        if (storedParameters.streamId == laneParameters.streamId)
        {
            return true;
        }
    }

    return false;
}

bool SpawnPoint::NewAgentIntersectsWithExistingAgent(int laneId, double offset)
{
    double distance = spawnInfo.s.value - currentDistanceToLaneStart;
    Position pos = GetWorld()->GetPositionByDistanceAndLane(distance, offset, spawnInfo.roadId, laneId);
    VehicleModelParameters vehicleModelParameters = currentBluePrint->GetVehicleModelParameters();

    return GetWorld()->IntersectsWithAgent(pos.xPos,
                                           pos.yPos,
                                           pos.yawAngle,
                                           vehicleModelParameters.length,
                                           vehicleModelParameters.width,
                                           vehicleModelParameters.distanceReferencePointToLeadingEdge);
}

std::tuple<bool, double> SpawnPoint::TryGetSpawnDistance(std::string roadId, int laneId, SpawnAttribute sAttribute)
{
    int trials = sAttribute.isStochastic ? NUMBER_OF_TRIALS_STOCHASTIC : 1;

    for (int trial = 0; trial < trials; trial++)
    {
        CalculateAttributeValue(sAttribute);
        auto spawnDistance = sAttribute.value;

        if (GetWorld()->IsSValidOnLane(roadId, laneId, spawnDistance))
        {
            return {true, spawnDistance};
        }
    }

    return {false, 0.0};
}

void SpawnPoint::CalculateAttributeValue(SpawnAttribute& attribute)
{
    if (attribute.isStochastic)
    {
        attribute.value = sampler->RollForStochasticAttribute(attribute.mean,
                          attribute.stdDeviation,
                          attribute.lowerBoundary,
                          attribute.upperBoundary);
    }
}

bool SpawnPoint::IsOffsetValidForLane(std::string roadId, int laneId, double distanceFromStart, double offset, double vehicleWidth)
{
    if (!GetWorld()->IsSValidOnLane(roadId, laneId, distanceFromStart))
    {
        LogError("Invalid offset. Lane is not available: " + std::to_string(laneId) + ". Distance from start: " +
                 std::to_string(distanceFromStart));
        return false;
    }

    //Check if lane width > vehicle width
    double laneWidth = GetWorld()->GetLaneWidth(roadId, laneId, distanceFromStart);
    if (vehicleWidth > laneWidth + std::abs(offset))
    {
        LogError("Invalid offset. Lane width < vehicle width: " + std::to_string(laneId) + ". Distance from start: " +
                 std::to_string(distanceFromStart) + ". Lane width: " + std::to_string(laneWidth) + ". Vehicle width: " + std::to_string(
                     vehicleWidth));
        return false;
    }

    //Is vehicle completely inside the lane
    bool isVehicleCompletelyInLane = (laneWidth - vehicleWidth) * 0.5 >= std::abs(offset);
    if (isVehicleCompletelyInLane)
    {
        return true;
    }

    //Is vehicle more than 50 % on the lane
    double allowedRange = laneWidth * 0.5;
    bool outsideAllowedRange =  std::abs(offset) > allowedRange;
    if (outsideAllowedRange)
    {
        LogError("Invalid offset. Vehicle not inside allowed range: " + std::to_string(laneId) + ". Invalid offset: " +
                 std::to_string(offset));
        return false;
    }

    //Is vehicle partly on invalid lane
    bool isOffsetToLeftLane = (offset >= 0);
    int otherLaneId = isOffsetToLeftLane ? (laneId + 1) : (laneId - 1);

    if (!GetWorld()->IsSValidOnLane(roadId, otherLaneId, distanceFromStart)) //other lane is invalid
    {
        LogError("Invalid offset. Other lane is invalid: " + std::to_string(laneId) + ". Invalid offset: " + std::to_string(
                     offset));
        return false;
    }

    return true;
}

double SpawnPoint::GetPreRunTimeTillSpawn()
{
    return spawnPointParameters.carsPerSecond == 0.0 ? static_cast<double>(INFINITY) : 0.0;
}

bool SpawnPoint::GenerateAgent(AgentBlueprintInterface* agentBlueprint)
{
    //Sets predefined values
    SetPredefinedParameter(agentBlueprint);

    //Chooses the next lane
    LaneParameters laneParameters;
        if (!GetSpawningLane(laneParameters))
    {
        UpdateSpawnPointState();
        agentBlueprint->Reset();
        LogCurrentDebugStatus("Rerolled agent because of invalid laneParameters.");

        return GenerateAgent(agentBlueprint);
    }

    //Probability related parameters of the blueprint
    if(!agentBlueprintProvider->SampleAgent(*agentBlueprint, laneParameters.laneCategory, scenarioAgentIterator))
    {
        return false;
    }

    //Calculate SpawnCarInfo
    if (!GetNextSpawnCarInfo(laneParameters, agentBlueprint))
    {
        agentBlueprint->Reset();
        LogCurrentDebugStatus("Could not get next spawncar info.");
        return GenerateAgent(agentBlueprint);
    }

    //Spawn related parameters are filled into the blueprint
    if (!CalculateSpawnParameter(agentBlueprint, laneParameters))
    {
        agentBlueprint->Reset();
        LogCurrentDebugStatus("Rerolled agent because SpawnParameters could not be calculated.");

        return GenerateAgent(agentBlueprint);
    }

    return true;
}

void SpawnPoint::LogCurrentDebugStatus(std::string message)
{
    std::stringstream log;
    log.str(std::string());
    log << COMPONENTNAME << " " << message;
    LOG(CbkLogLevel::Debug, log.str());
}

void SpawnPoint::LogWarning(std::string message)
{
    std::stringstream log;
    log.str(std::string());
    log << COMPONENTNAME << " " << message;
    LOG(CbkLogLevel::Warning, log.str());
}

void SpawnPoint::LogError(std::string message)
{
    std::stringstream log;
    log.str(std::string());
    log << COMPONENTNAME << " " << message;
    LOG(CbkLogLevel::Error, log.str());
}

bool SpawnPoint::AreSpawningCoordinatesValid(std::string roadId, int laneId, double distanceReferencePoint,
        double offset)
{
    if (!GetWorld()->IsSValidOnLane(roadId, laneId, distanceReferencePoint))
    {
        LogWarning("S is not valid for vehicle on lane: " + std::to_string(laneId) + ". Invalid s: " + std::to_string(
                       distanceReferencePoint));
        return false;
    }

    if (!IsOffsetValidForLane(roadId, laneId, distanceReferencePoint, offset, currentBluePrint->GetVehicleModelParameters().width))
    {
        LogError("Offset is not valid for vehicle on lane: " + std::to_string(laneId) + ". Invalid offset: " + std::to_string(
                     offset));

        return false;
    }

    if (NewAgentIntersectsWithExistingAgent(laneId, offset))
    {
        LogWarning("New Agent intersects existing agent on lane: " + std::to_string(laneId) + ".");
        return false;
    }

    if (GetWorld()->GetDistanceToEndOfDrivingLane(roadId, laneId, distanceReferencePoint,
                                           INFINITY) < spawnPointConstants.minimumDistanceToEndOfLane)
    {
        LogWarning("End of lane: " + std::to_string(laneId) + " is too close.");
        return false;
    }
    return true;
}

bool SpawnPoint::IsWithinEgoRadius(double distanceToReferencePoint)
{
    if (distanceToReferencePoint < egoSpawnParameters.s.value - egoSpawnParameters.spawnRadius ||
            distanceToReferencePoint > egoSpawnParameters.s.value + egoSpawnParameters.spawnRadius)
    {
        return false;
    }

    return true;
}

bool SpawnPoint::GetNextEgoLane(LaneParameters& laneParameters)
{
    bool spawnDistanceValid;
    double spawnDistance;

    std::tie(spawnDistanceValid, spawnDistance) = TryGetSpawnDistance(egoSpawnParameters.egoRoadId,
                                                                      egoSpawnParameters.egoLaneId,
                                                                      egoSpawnParameters.s);

    if (spawnDistanceValid == false)
    {
        LogError("Could not retrieve ego lane.");
        return false;
    }

    auto queryResult = GetWorld()->QueryLane(egoSpawnParameters.egoRoadId, egoSpawnParameters.egoLaneId, spawnDistance);
    assert(queryResult.valid); // valid, as spawnDistance is checked before (spawnDistanceValid)

    egoSpawnParameters.s.value = spawnDistance;
    CalculateAttributeValue(egoSpawnParameters.offset);

    laneParameters =
    {
        queryResult.streamId,
        egoSpawnParameters.egoLaneId,
        queryResult.laneCategory,
        queryResult.startDistance,
        egoSpawnParameters.offset.value
    };

    return true;
}

bool SpawnPoint::GetNextSceneryLane(LaneParameters& laneParameters)
{
    if (scenario->GetScenarioEntities().size() > 0)
    {
        auto scenerySpawnInfo = scenario->GetScenarioEntities().at(scenarioAgentIterator)->spawnInfo;

        bool spawnDistanceValid;
        double spawnDistance;

        std::tie(spawnDistanceValid, spawnDistance) = TryGetSpawnDistance(scenerySpawnInfo.roadId,
                                                                          scenerySpawnInfo.ILane,
                                                                          scenerySpawnInfo.s);

        if (spawnDistanceValid == false)
        {
            LogError("Could not retrieve scenery lane.");
            return false;
        }

        CalculateAttributeValue(scenerySpawnInfo.offset);

        auto queryResult = GetWorld()->QueryLane(scenerySpawnInfo.roadId, scenerySpawnInfo.ILane, spawnDistance);
        assert(queryResult.valid); // valid, as spawnDistance is checked before (spawnDistanceValid)

        sceneryCarCurrentSPosition = spawnDistance;
        sceneryCarCurrentOffset = scenerySpawnInfo.offset.value;

        laneParameters =
        {
            queryResult.streamId,
            scenerySpawnInfo.ILane,
            queryResult.laneCategory,
            queryResult.startDistance,
            scenerySpawnInfo.offset.value
        };

        return true;
    }

    return false;
}

bool SpawnPoint::GetNextCommonLane(LaneParameters& laneParameters)
{
    laneParameters = sceneryParser.GetEgoAndSceneryParameters().at(egoAndSceneryLanesIndex);
    laneParameters.offset = 0;

    return true;
}

bool SpawnPoint::GetNextRespawnLane(LaneParameters& laneParameters)
{
    AgentInterface* egoAgent = GetWorld()->GetEgoAgent();

    double distanceStartToEgo = egoAgent ?
                                egoAgent->GetDistanceToStartOfRoad() :
                                egoSpawnParameters.s.value;
    std::string roadId = egoAgent ?
                         egoAgent->GetRoadId() :
                         egoSpawnParameters.egoRoadId;

    auto lanes = GetWorld()->GetDrivingLanesAtDistance(roadId, distanceStartToEgo);

    if (lanes.empty())
    {
        return false;
    }

    auto index = sampler->RollUniformDistributedVectorIndex(lanes.size());
    auto laneId = lanes.at(index);

    auto queryResult = GetWorld()->QueryLane(roadId, laneId, distanceStartToEgo);
    assert(queryResult.valid); // valid, as it is a result of GetDrivingLanesAtDistance

    laneParameters =
    {
        queryResult.streamId,
        laneId,
        queryResult.laneCategory,
        queryResult.startDistance,
        laneParameters.offset
    };

    return true;
}

bool SpawnPoint::GetNextPlacementLane(LaneParameters& laneParameters)
{
    return sceneryParser.ConsumeCommonLaneParameters(laneParameters); //peak
}


bool SpawnPoint::RemoveLastPlacementLane()
{
    return sceneryParser.RemoveLastCommonLaneParameter(); //pop
}

bool SpawnPoint::GetNextSpawningLane(LaneParameters& laneParameters)
{
    if (sceneryParser.GetEntryLanes().empty())
    {
        return false;
    }

    auto lanesOfFirstSection = sceneryParser.GetEntryLanes();
    auto index = sampler->RollUniformDistributedVectorIndex(lanesOfFirstSection.size());
    laneParameters = lanesOfFirstSection.at(index);

    return true;
}

bool SpawnPoint::GetSpawningLane(LaneParameters& laneParameters)
{
    switch (spawnPointState)
    {
        case SpawnPointState::EgoPlacement:
            if (!GetNextEgoLane(laneParameters))
            {
                throw std::runtime_error("Ego lane does not exist! RunConfiguration does not match SceneryConfiguration.");
            }
            break;

        case SpawnPointState::SceneryPlacement:
            if (!GetNextSceneryLane(laneParameters))
            {
                LogCurrentDebugStatus("No scenery car listed.");
                return false;
            }
            break;

        case SpawnPointState::CommonSpecificFrontPlacement:
            if (!GetNextCommonLane(laneParameters))
            {
                LogCurrentDebugStatus("Could not get lane parameters for CommonSpecificFrontPlacement");
                return false;
            }
            break;

        case SpawnPointState::CommonSpecificRearPlacement:
            if (!GetNextCommonLane(laneParameters))
            {
                LogCurrentDebugStatus("Could not get lane parameters for CommonSpecificRearPlacement");
                return false;
            }
            break;

        case SpawnPointState::CommonPreRunPlacement:
            if (!GetNextPlacementLane(laneParameters))
            {
                LogCurrentDebugStatus("Could not get lane parameters for CommonPreRunPlacement");
                return false;
            }
            break;

        case SpawnPointState::CommonSpawning:
            if (!GetNextSpawningLane(laneParameters))
            {
                LogCurrentDebugStatus("Could not get lane parameters for CommonSpawning");
                return false;
            }
            break;

        case SpawnPointState::Respawning:
            if (!GetNextRespawnLane(laneParameters))
            {
                //  throw std::runtime_error("Could not get lane parameters for Respawning");
            }
            break;

        default:
            LogError("SpawnPoint State is not handled.");
            break;
    }

    currentDistanceToLaneStart = laneParameters.distanceToLaneStart;

    return true;
}

void SpawnPoint::UpdateSpawnPointState()
{
    switch (spawnPointState)
    {
        case SpawnPointState::EgoPlacement:
            spawnPointState = SpawnPointState::SceneryPlacement;
            if (scenario->GetScenarioEntities().size() == 0)
            {
                UpdateSpawnPointState();
            }
            break;

        case SpawnPointState::SceneryPlacement:
            spawnPointState = SpawnPointState::CommonSpecificFrontPlacement;
            break;

        case SpawnPointState::CommonSpecificFrontPlacement:
            spawnPointState = SpawnPointState::CommonSpecificRearPlacement;
            break;

        case SpawnPointState::CommonSpecificRearPlacement:
            spawnPointState = SpawnPointState::CommonPreRunPlacement;
            break;

        case SpawnPointState::CommonPreRunPlacement:
            spawnPointState = SpawnPointState::CommonSpawning;
            break;

        default:
            break;
    }
}

bool SpawnPoint::IsSceneryCompleted()
{
    return (scenarioAgentIterator > scenario->GetScenarioEntities().size() - 1);
}
