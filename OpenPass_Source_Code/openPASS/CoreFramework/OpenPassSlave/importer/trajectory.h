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

#include "Interfaces/trajectoryInterface.h"

using WorldCoordinateTrajectory = std::map<int, Position>;
using RoadCoordinateTrajectory = std::map<int, RoadPosition>;

class Trajectory : public TrajectoryInterface
{
public:
    Trajectory();

    bool AddRoadCoordinate(int time, RoadPosition roadCoordinate);
    bool AddWorldCoordinate(int time, Position worldCoordinate);

    TrajectoryType GetTrajectoryType();
    RoadCoordinateTrajectory *GetRoadCoordinates();
    WorldCoordinateTrajectory *GetWorldCoordinates();

    void SetTrajectoryType(TrajectoryType trajectoryType);

private:
    TrajectoryType type;
    RoadCoordinateTrajectory roadCoordinates;
    WorldCoordinateTrajectory worldCoordinates;
};
