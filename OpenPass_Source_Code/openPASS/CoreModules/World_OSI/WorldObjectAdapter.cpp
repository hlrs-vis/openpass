/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "Common/boostGeometryCommon.h"
#include "WorldData.h"
#include "WorldObjectAdapter.h"

WorldObjectAdapter::WorldObjectAdapter(OWL::Interfaces::WorldObject& baseTrafficObject) :
    baseTrafficObject{baseTrafficObject}
{
}

const polygon_t& WorldObjectAdapter::GetBoundingBox2D() const
{
    if (boundingBoxNeedsUpdate)
    {
        boundingBox = CalculateBoundingBox();
        boundingBoxNeedsUpdate = false;
    }

    return boundingBox;
}

double WorldObjectAdapter::GetDistanceReferencePointToLeadingEdge() const
{
    return baseTrafficObject.GetDimension().length / 2.0;
}

double WorldObjectAdapter::GetHeight() const
{
    return baseTrafficObject.GetDimension().height;
}

int WorldObjectAdapter::GetId() const
{
    return baseTrafficObject.GetId();
}

double WorldObjectAdapter::GetLength() const
{
    return baseTrafficObject.GetDimension().length;
}

double WorldObjectAdapter::GetPositionX() const
{
    return baseTrafficObject.GetReferencePointPosition().x;
}

double WorldObjectAdapter::GetPositionY() const
{
    return baseTrafficObject.GetReferencePointPosition().y;
}

double WorldObjectAdapter::GetWidth() const
{
    return baseTrafficObject.GetDimension().width;
}

double WorldObjectAdapter::GetYaw() const
{
    return baseTrafficObject.GetAbsOrientation().yaw;
}
    
double WorldObjectAdapter::GetAcceleration() const
{
    return baseTrafficObject.GetAbsAccelerationDouble();
}

const OWL::Interfaces::WorldObject& WorldObjectAdapter::GetBaseTrafficObject() const
{
    return baseTrafficObject;
}

const polygon_t WorldObjectAdapter::CalculateBoundingBox() const
{
    double length = GetLength();
    double width = GetWidth();
    double rotation = GetYaw();

    double x = GetPositionX();
    double y = GetPositionY();

    double center = GetDistanceReferencePointToLeadingEdge();

    double halfWidth = width / 2.0;

    point_t boxPoints[]
    {
        {center - length, -halfWidth},
        {center - length,  halfWidth},
        {center,           halfWidth},
        {center,          -halfWidth},
        {center - length, -halfWidth}
    };

    polygon_t box;
    polygon_t boxTemp;
    bg::append(box, boxPoints);

    bt::translate_transformer<double, 2, 2> translate(x, y);

    // rotation in mathematical negativ order (boost) -> invert to match
    bt::rotate_transformer<bg::radian, double, 2, 2> rotate(-rotation);

    bg::transform(box, boxTemp, rotate);
    bg::transform(boxTemp, box, translate);

    return box;
}

namespace WorldObjectCommon {

double GetFrontDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge)
{
    auto l_front = distanceReferencePointToLeadingEdge;
    auto l_rear = distanceReferencePointToLeadingEdge - length;
    auto w = width / 2;

    auto s1 = l_front * cos(hdg) - w * sin(hdg);
    auto s2 = l_front * cos(hdg) + w * sin(hdg);
    auto s3 = l_rear  * cos(hdg) - w * sin(hdg);
    auto s4 = l_rear  * cos(hdg) + w * sin(hdg);

    return std::max({s1, s2, s3, s4});
}

double GetRearDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge)
{
    auto l_front = distanceReferencePointToLeadingEdge;
    auto l_rear = distanceReferencePointToLeadingEdge - length;
    auto w = width / 2;

    auto s1 = l_front * cos(hdg) - w * sin(hdg);
    auto s2 = l_front * cos(hdg) + w * sin(hdg);
    auto s3 = l_rear  * cos(hdg) - w * sin(hdg);
    auto s4 = l_rear  * cos(hdg) + w * sin(hdg);

    return std::min({s1, s2, s3, s4});
}
} // namespace WorldObjectCommon
