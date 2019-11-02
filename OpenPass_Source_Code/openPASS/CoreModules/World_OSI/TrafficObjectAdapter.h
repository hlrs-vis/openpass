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

#include "Interfaces/trafficObjectInterface.h"
#include "WorldObjectAdapter.h"
#include "Localization/Localization.h"

class TrafficObjectAdapter : public WorldObjectAdapter, public TrafficObjectInterface
{
private:
    double laneDirection;
    World::Localization::BaseTrafficObjectLocator locator;

    World::Localization::Result locateResult;
    mutable std::vector<GlobalRoadPosition> boundaryPoints;
    mutable World::Localization::Remainders remainders;

    void InitLaneDirection(double hdg);

public:
    TrafficObjectAdapter(OWL::Interfaces::WorldData& worldData,
                         World::Localization::Cache& localizationCache,
                         OWL::Primitive::AbsPosition position,
                         OWL::Primitive::Dimension dimension,
                         OWL::Primitive::AbsOrientation orientation);

    ObjectTypeOSI GetType() const override;
    double GetDistanceToStartOfRoad() const override;
    double GetDistanceToStartOfRoad(MeasurementPoint mp) const override;
    double GetVelocity(VelocityScope velocityScope = VelocityScope::Absolute) const override;
    double GetLaneDirection() const override;
    double GetLaneRemainder(Side side) const override;
    GlobalRoadPosition GetBoundaryPoint(Side side) const override;
    bool Locate() override;
    void Unlocate() override;

    // object is not inteded to be copied or assigned
    TrafficObjectAdapter(const TrafficObjectAdapter&) = delete;
    TrafficObjectAdapter(TrafficObjectAdapter&&) = delete;
    TrafficObjectAdapter& operator=(const TrafficObjectAdapter&) = delete;
    TrafficObjectAdapter& operator=(TrafficObjectAdapter&&) = delete;
    ~TrafficObjectAdapter() override;
};


