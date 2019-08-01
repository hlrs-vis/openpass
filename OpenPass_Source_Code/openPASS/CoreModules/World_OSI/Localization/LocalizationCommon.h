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
//! @file
//! @brief Common headers and typedefinitions for localization namespace

#pragma once

#include <memory>

#include "Common/boostGeometryCommon.h"
#include "Common/globalDefinitions.h"
#include "LocalizationCache.h"
#include "WorldData.h"

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&& ... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct SearchResult
{
    const OWL::Lane& lane;
    OWL::LocatedPoint point;
};

struct CoverageInformation
{
    std::shared_ptr<const SearchResult> left;
    std::shared_ptr<const SearchResult> right;

    double remainderLeft {0.0};
    double remainderRight {0.0};
};

struct Remainder
{
    Remainder() = default;
    Remainder(double left, double right) : left{left}, right{right}
    {}

    double left {0.0};
    double right {0.0};
};

namespace World {
namespace Localization {

using Coverages = std::vector<CoverageInformation>;
using Remainders = std::vector<Remainder>;
using const_sectionIter = OWL::Interfaces::Sections::const_iterator;

polygon_t GetBoundingBox(double x, double y, double length, double width, double rotation, double center);

} // Localization
} // World


