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

#include "globalDefinitions.h"

enum class TrajectoryType {
    WorldCoordinatesAbsolute = 0,
    RoadCoordinatesAbsolute = 1,
    RoadCoordinatesRelative = 2,
};

class TrajectoryInterface
{
public:
    TrajectoryInterface() = default;
    virtual ~TrajectoryInterface() = default;

    virtual bool AddRoadCoordinate(int time, RoadPosition roadCoordinate) = 0;
    virtual bool AddWorldCoordinate(int time, Position worldCoordinate) = 0;

    virtual TrajectoryType GetTrajectoryType() = 0;
    virtual std::map<int, RoadPosition> *GetRoadCoordinates() = 0;
    virtual std::map<int, Position> *GetWorldCoordinates() = 0;

    virtual void SetTrajectoryType(TrajectoryType trajectoryType) = 0;
};
