/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "trajectory.h"

Trajectory::Trajectory()
{

}

bool Trajectory::AddRoadCoordinate(int time, RoadPosition roadCoordinate)
{
    if(roadCoordinates.find(time) != roadCoordinates.end())
    {
        return false;
    }

    roadCoordinates.insert({time, roadCoordinate});

    return true;
}

bool Trajectory::AddWorldCoordinate(int time, Position worldCoordinate)
{
    if(worldCoordinates.find(time) != worldCoordinates.end())
    {
        return false;
    }

    worldCoordinates.insert({time, worldCoordinate});

    return true;
}

TrajectoryType Trajectory::GetTrajectoryType()
{
    return type;
}

RoadCoordinateTrajectory *Trajectory::GetRoadCoordinates()
{
    return &roadCoordinates;
}

WorldCoordinateTrajectory *Trajectory::GetWorldCoordinates()
{
    return &worldCoordinates;
}


void Trajectory::SetTrajectoryType(TrajectoryType trajectoryType)
{
    this->type = trajectoryType;
}
