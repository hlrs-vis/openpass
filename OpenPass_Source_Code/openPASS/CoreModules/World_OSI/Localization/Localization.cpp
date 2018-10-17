/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <exception>

#include "Localization.h"
#include "PointAggregator.h"
#include "PointLocator.h"
#include "SectionObjectGenerator.h"

namespace World {
namespace Localization {

BaseTrafficObjectLocator::BaseTrafficObjectLocator(const std::unordered_map<OWL::Id, OWL::Interfaces::Road*> &roads) :
    roads{roads}
{
}

void BaseTrafficObjectLocator::SetBaseTrafficObject(OWL::MovingObject* bto)
{
    this->baseTrafficObject = bto;
}

void BaseTrafficObjectLocator::Locate()
{
    PointAggregator pointAggregator;
    const OWL::Primitive::AbsPosition objectPosition = baseTrafficObject->GetAbsPosition();
    const OWL::Primitive::AbsOrientation objectOrientation = baseTrafficObject->GetAbsOrientation();

    //point_t referencePoint(objectPosition.x, objectPosition.y);
    OWL::SearchablePoint referencePoint { { objectPosition.x,
                                            objectPosition.y,
                                            objectOrientation.yaw },
                                            OWL::PointType::Reference };

    // NOTE: assume single road in world for now
    auto sections = roads.begin()->second->GetSections();
    OWL::PointLocator localizer(pointAggregator, sections, {referencePoint});
    localizer.locate();

    SectionObjectGenerator sectionObjectGenerator(pointAggregator);
    sectionObjectGenerator.Generate(*baseTrafficObject);

    frontMainLaneId = sectionObjectGenerator.GetFrontMainLaneId();
    rearMainLaneId = sectionObjectGenerator.GetRearMainLaneId();

    allPointsLocated = localizer.allPointsLocated();

    const SearchResults& points = pointAggregator.Get(OWL::PointType::Reference);
    if (!points.empty())
    {
        const OWL::Primitive::RoadCoordinate& point = points.front().point.coordinate;
        referencePointPosition.s = point.s;
        referencePointPosition.t = point.t;
        referencePointPosition.hdg = point.yaw;
    }

}

void BaseTrafficObjectLocator::Unlocate()
{
    // TODO
}

int BaseTrafficObjectLocator::GetMainLaneId(Reference reference)
{
    if(reference == Reference::Front)
        return frontMainLaneId;

    if(reference == Reference::Rear)
        return rearMainLaneId;

    throw std::invalid_argument("unknown reference value");
}

const std::map<int, double> &BaseTrafficObjectLocator::GetLaneCoverages(Reference reference)
{
    if(reference == Reference::Front)
        return frontLaneCoverages;

    if(reference == Reference::Rear)
        return rearLaneCoverages;

    throw std::invalid_argument("unknown reference value");
}

const std::map<int, Remainder> &BaseTrafficObjectLocator::GetLaneRemainder()
{
    return laneRemainder;
}

RoadPosition BaseTrafficObjectLocator::GetReferencePoint()
{
    return referencePointPosition;
}

bool BaseTrafficObjectLocator::AllPointsLocated()
{
    return allPointsLocated;
}

}
}
