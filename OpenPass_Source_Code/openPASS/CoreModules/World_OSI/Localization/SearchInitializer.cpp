/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "SearchInitializer.h"
#include "LaneWalker.h"

World::Localization::SearchInitializer::SearchInitializer(bool valid, const_sectionIter sectionIter, const_sectionIter sectionIterEnd, double sectionOffset, LaneWalkers laneWalkers) :
    valid{valid},
    sectionIter{sectionIter},
    sectionIterEnd{sectionIterEnd},
    sectionOffset{sectionOffset}
{
    // clone objects, not it's pointers
    for (auto laneWalker : laneWalkers)
    {
        this->laneWalkers.push_back(std::make_shared<OWL::LaneWalker>(*laneWalker));
    }
}
