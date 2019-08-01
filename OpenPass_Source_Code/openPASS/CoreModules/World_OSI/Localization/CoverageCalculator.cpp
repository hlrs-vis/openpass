/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include <memory>
#include <exception>

#include "CoverageCalculator.h"

namespace World {
namespace Localization {

Coverages CoverageCalculator::Process(const PointAggregatorInterface& pointAggregator)
{
    auto streamType = pointAggregator.GetStreamType();
    std::unique_ptr<CoverageEngine> coverageEngine;

    switch (streamType)
    {
        case StreamType::Empty:
            return Coverages();
        case StreamType::Single:
            coverageEngine = make_unique<SingleCoverageEngine>(pointAggregator);
            break;
        case StreamType::Neighbours:
            coverageEngine = make_unique<NeighbouringCoverageEngine>(pointAggregator);
            break;
        default:
            throw std::logic_error("Not implemented yet");
    }

    return coverageEngine->Calculate();
}

} // namespace Localization
} // namespace World
