/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <limits>
#include <vector>

#include "globalDefinitions.h"

enum class SpawnPointState
{
    EgoPlacement,
    SceneryPlacement,
    CommonSpecificFrontPlacement,
    CommonSpecificRearPlacement,
    CommonPreRunPlacement,
    CommonSpawning,
    Respawning
};

struct SpawnAttribute
{
    bool isStochastic = false;
    double value = -999;
    double mean = -999;
    double stdDeviation = -999;
    double lowerBoundary = -999;
    double upperBoundary = -999;
};


struct LaneParameters
{
    LaneParameters() {}

    LaneParameters(uint64_t streamId, int laneId, LaneCategory laneCategory, double distanceToLaneStart, double offset) :
        streamId{streamId},
        laneId{laneId},
        laneCategory{laneCategory},
        distanceToLaneStart{distanceToLaneStart},
        offset{offset}
    {}

    uint64_t streamId{0};
    int laneId{0};
    LaneCategory laneCategory {LaneCategory::Undefined};
    double distanceToLaneStart{0};
    double offset{0};
};

struct SpawnPointConstants
{
    const int standardCycleTime = 100;
    const double egoSpawnRadius = 1000;
    const double minimumGapInSeconds = 1.0;
    const double nonPlatoonGapExtension = 10.0;
    const double minimumDistanceToStart = 0;
    const double stdDeviationVelocity = 2.777777777777;
    const double minimumDistanceToEndOfLane = 50;
    const std::vector<double> maximumVelocityPerLane { 46.51, 40.99, 35.86, 35.86, 35.86, 35.86, 35.86, 35.86, 35.86, 35.86};
};

struct SpawnPointParameters
{
    double carsPerSecond;
    double homogenity;
    double platoonRate;
    double trafficVelocity;
};

struct EgoSpawnParameters
{
    std::string egoRoadId {};
    int egoLaneId{-1};
    double spawnRadius{1000};
    SpawnAttribute s;
    SpawnAttribute offset;
    SpawnAttribute acceleration;
};

class SpawnInfo
{
public:
    SpawnInfo() {}
    SpawnInfo(double t, double s, double v, std::string roadId, int lane, double offset, double acceleration, double heading = 0):
        TStart(t), roadId(roadId), ILane(lane), heading(heading)
    {
        this->s.value = s;
        this->offset.value = offset;
        this->velocity.value = v;
        this->acceleration.value = acceleration;
    }

    double TStart = -999; // time
    std::string roadId = "";
    int ILane = -999;
    double heading = 0;

    SpawnAttribute offset;
    SpawnAttribute s;
    SpawnAttribute velocity;
    SpawnAttribute acceleration;
};
