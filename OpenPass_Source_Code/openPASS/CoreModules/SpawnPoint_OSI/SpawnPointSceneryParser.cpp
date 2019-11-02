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
/** @file  SpawnPointSceneryParser.cpp */
//-----------------------------------------------------------------------------

#include "SpawnPointSceneryParser.h"
#include <map>
#include <list>

namespace SpawnPointHelper {

SceneryParser::SceneryParser(const WorldInterface& world, std::string parsedRoadId) : world(world)
{
    ParseEntryLanes(parsedRoadId);
}

void SceneryParser::Parse(EgoSpawnParameters egoSpawnParameters)
{
    ParseLaneStreamsInSpawningRadius(egoSpawnParameters);
}

void SceneryParser::ParseEntryLanes(std::string roadId)
{
    // Respect only right hand side IDs: must start with -1 (OpenDrive Standard)
    for (int laneId = -1; laneId > -100; laneId--)
    {
        auto queryResult = world.QueryLane(roadId, laneId, 0);

        if (queryResult.valid == false)
        {
            break; // Missing lane IDs are not allowed (OpenDrive Standard)
        }

        if (queryResult.isDrivingLane)
        {
            entryLaneParameters.push_back(
            {
                queryResult.streamId,
                laneId,
                queryResult.laneCategory,
                queryResult.startDistance,
                0.0
            });
        }
    }
}

void SceneryParser::ParseLaneStreamsInSpawningRadius(EgoSpawnParameters egoSpawnParameters)
{
    auto startDistance = egoSpawnParameters.s.value - egoSpawnParameters.spawnRadius;
    auto endDistance = egoSpawnParameters.s.value + egoSpawnParameters.spawnRadius;

    for (const auto& queryResult : world.QueryLanes(egoSpawnParameters.egoRoadId, startDistance, endDistance))
    {
        if (queryResult.isDrivingLane)
        {
            commonLaneStreamParameters.push_back(
            {
                queryResult.streamId,
                world.GetLaneId(queryResult.streamId, queryResult.startDistance),
                queryResult.laneCategory,
                queryResult.startDistance,
                0.0
            });
        }
    }
}

void SceneryParser::RemoveFromCommonStreamParameters(LaneParameters laneParameters)
{
    auto laneParametersEqual = [&](const LaneParameters & storedParameters)
    {
        return storedParameters.streamId == laneParameters.streamId;
    };

    commonLaneStreamParameters.erase(std::remove_if(
                                         commonLaneStreamParameters.begin(),
                                         commonLaneStreamParameters.end(),
                                         laneParametersEqual
                                     ), commonLaneStreamParameters.end());
}

std::vector<LaneParameters> SceneryParser::GetEgoAndSceneryParameters()
{
    return egoAndSceneryParameters;
}

void SceneryParser::PushEgoParameters(LaneParameters laneParameters)
{
    if (!egoAndSceneryParameters.empty())
    {
        throw std::logic_error("Trying to push a second set of parameters for the ego");
    }

    PushSceneryParameters(laneParameters);
}

void SceneryParser::PushSceneryParameters(LaneParameters laneParameters)
{
    auto laneParametersEqual = [&](const LaneParameters & storedParameters)
    {
        return storedParameters.streamId == laneParameters.streamId;
    };

    if (std::find_if(egoAndSceneryParameters.begin(),
                     egoAndSceneryParameters.end(),
                     laneParametersEqual) == egoAndSceneryParameters.end())
    {
        egoAndSceneryParameters.push_back(laneParameters);
    }

    RemoveFromCommonStreamParameters(laneParameters);
}

std::vector<LaneParameters> SceneryParser::GetEntryLanes()
{
    return entryLaneParameters;
}

std::vector<LaneParameters> SceneryParser::GetCommonLaneStreamParameters()
{
    return commonLaneStreamParameters;
}


bool SceneryParser::ConsumeCommonLaneParameters(LaneParameters& laneParameter)
{
    if (commonLaneStreamParameters.empty())
    {
        return false;
    }

    laneParameter = commonLaneStreamParameters.back();

    return true;
}


bool SceneryParser::RemoveLastCommonLaneParameter()
{
    if (commonLaneStreamParameters.empty())
    {
        return false;
    }
    commonLaneStreamParameters.pop_back();

    return true;
}

} // namespace
