/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "globalDefinitions.h"

#include "LocalizationCommon.h"
#include "SearchInitializer.h"
#include "WorldData.h"

#pragma once

namespace World {
namespace Localization {

constexpr double BOUNDING_BOX_SAMPLE_WIDTH = 2.5;
constexpr double MOTION_PREVIEW_DISTANCE = 5.55; // 200km/h in 100ms

enum class Reference
{
    Front,
    Rear
};

class BaseTrafficObjectLocator
{
private:
    OWL::MovingObject* baseTrafficObject;
    const std::unordered_map<OWL::Id, OWL::Interfaces::Road*>& roads;

    int frontMainLaneId;
    int rearMainLaneId;

    std::map<int, Remainder> laneRemainder;
    std::map<int, double> frontLaneCoverages;
    std::map<int, double> rearLaneCoverages;

    bool allPointsLocated;
    RoadPosition referencePointPosition;
    SearchInitializer searchInitializer{};
    Cache& cache;

public:
    BaseTrafficObjectLocator(const std::unordered_map<OWL::Id, OWL::Interfaces::Road*>& roads, Cache& cache);

    void SetBaseTrafficObject(OWL::MovingObject* bto);

    void Locate(const polygon_t& boundingBox);
    void Unlocate();

    const polygon_t& GetBoundingBox();
    int GetMainLaneId(Reference reference);
    const std::map<int, double>& GetLaneCoverages(Reference reference);
    const std::map<int, Remainder>& GetLaneRemainder();
    RoadPosition GetReferencePoint();
    bool AllPointsLocated();
};

}
}
