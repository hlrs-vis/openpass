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
/** @file  SpawnPointSceneryParser.h
*   @brief This file parses the scenery.
*
*   This file parses the scenery which is provided by the Scenery.xml
*   The different lanes are split into different types and categories which then are being used by the SpawnPoint.*/
//-----------------------------------------------------------------------------

#pragma once

#include "Common/spawnPointDefinitions.h"
#include "Interfaces/worldInterface.h"

namespace SpawnPointHelper {

//-----------------------------------------------------------------------------
/** \brief This class parses the scenery
*
*   \ingroup SpawnPoint */
//-----------------------------------------------------------------------------
class SceneryParser
{
public:
    SceneryParser(const WorldInterface& world, std::string parsedRoadId);

    /*!
    * \brief Parses the scenery.
    *
    * \details Parses the scenery into the right hand side driving lanes.
    *
    * @param[in]     egoSpawnParameters         SpawnParameters of the ego agent.
    */
    void Parse(EgoSpawnParameters egoSpawnParameters);

    /*!
    * \brief Returns the lanes of the first section.
    *
    * \details Returns the lanes of the first section.
    *
    * @return     Lanes of the first section.
    */
    std::vector<LaneParameters> GetEntryLanes();
    std::vector<LaneParameters> GetCommonLaneStreamParameters();

    bool ConsumeCommonLaneParameters(LaneParameters& laneParameter);
    bool RemoveLastCommonLaneParameter();


    std::vector<LaneParameters> GetEgoAndSceneryParameters();

    void PushEgoParameters(LaneParameters laneParameters);
    void PushSceneryParameters(LaneParameters laneParameters);

private:

    void ParseEntryLanes(std::string roadId);
    void ParseLaneStreamsInSpawningRadius(EgoSpawnParameters egoSpawnParameters);

    const WorldInterface& world;

    void RemoveFromCommonStreamParameters(LaneParameters laneParameters);

    std::vector<LaneParameters> egoAndSceneryParameters;
    std::vector<LaneParameters> commonLaneStreamParameters;
    std::vector<LaneParameters> entryLaneParameters;
};

} // namespace
