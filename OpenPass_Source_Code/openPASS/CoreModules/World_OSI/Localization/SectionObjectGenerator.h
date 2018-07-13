/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

#include <map>

#include "LocalizationCommon.h"
#include "PointAggregator.h"
#include "OWL/DataTypes.h"

namespace World {
namespace Localization {

class SectionObjectGenerator
{
private:

    PointAggregator& pointAggregator;

    std::map<int, double> rearLaneCoverage;

    std::map<int, double> frontLaneCoverage;
    std::map<int, Remainder> frontLaneRemainder;

    Remainder remainder;
    double currentSectionDistanceOffset;

public:
    SectionObjectGenerator(PointAggregator& pointAggregator) : pointAggregator(pointAggregator) {}

    void Generate(OWL::MovingObject& baseTrafficObject);
    int GetFrontMainLaneId();
    int GetRearMainLaneId();
    std::map<int, double> GetFrontLaneCoverage() const;
    std::map<int, double> GetRearLaneCoverage() const;
    std::map<int, Remainder> GetFrontRemainder() const;

private:
    void FilterSmallCoverages();
    void FilterSmallCoverages(std::map<int, double> &laneCoverage);
};

} // namespace Localization
} // namespace World


