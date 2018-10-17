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

#include "LocalizationCommon.h"
#include "WorldData.h"

namespace World {
namespace Localization {

struct SearchInitializer
{
    bool valid{false};
    const_sectionIter sectionIter;
    const_sectionIter sectionIterEnd;
    double sectionOffset{0};
    LaneWalkers laneWalkers;

    SearchInitializer(){}

    SearchInitializer(bool valid,
                      const_sectionIter sectionIter,
                      const_sectionIter sectionIterEnd,
                      double sectionOffset,
                      LaneWalkers laneWalkers);
};

} // Localization
} // World
