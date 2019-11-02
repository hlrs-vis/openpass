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

#include "WorldData.h"
#include "LaneWalker.h"
#include "LocalizationCommon.h"

namespace World {
namespace Localization {

struct SearchInitializer
{
    bool valid{false};
    const OWL::Interfaces::Section* section{nullptr};
    double sectionOffset{0};
    LaneWalkers upstreamLaneWalkers{};
    LaneWalkers downstreamLaneWalkers{};

    SearchInitializer() {}

    SearchInitializer(bool valid,
                      const OWL::Interfaces::Section* section,
                      double sectionOffset,
                      LaneWalkers upstreamLaneWalkers,
                      LaneWalkers downstreamLaneWalkers);
};

} // Localization
} // World
