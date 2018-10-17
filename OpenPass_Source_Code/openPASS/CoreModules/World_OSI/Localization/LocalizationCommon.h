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

#include "globalDefinitions.h"
#include "WorldData.h"

namespace OWL {
class LaneWalker;
}

namespace World {
namespace Localization {

using LaneWalkers = std::vector<std::shared_ptr<OWL::LaneWalker>>;
using const_sectionIter = OWL::Interfaces::Sections::const_iterator;

} // Localization
} // World


