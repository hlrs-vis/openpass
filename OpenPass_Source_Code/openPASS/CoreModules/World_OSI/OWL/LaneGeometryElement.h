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

#include <algorithm>

#include "OWL/Primitives.h"
#include "OWL/LaneGeometryJoint.h"

namespace OWL {

namespace Primitive {

struct LaneGeometryElement
{
    LaneGeometryElement(const LaneGeometryJoint current,
                        const LaneGeometryJoint next) :
        joints
        {
            current,
            next
        },
        box{
            std::min({joints.current.points.left.x, joints.next.points.left.x, joints.current.points.right.x, joints.next.points.right.x}),
            std::max({joints.current.points.left.x, joints.next.points.left.x, joints.current.points.right.x, joints.next.points.right.x}),
            std::min({joints.current.points.left.y, joints.next.points.left.y, joints.current.points.right.y, joints.next.points.right.y}),
            std::max({joints.current.points.left.y, joints.next.points.left.y, joints.current.points.right.y, joints.next.points.right.y})
        }{}

    struct Joints
    {
        LaneGeometryJoint current;
        LaneGeometryJoint next;
    } const joints;

    struct Box
    {
        double x_min;
        double x_max;
        double y_min;
        double y_max;

        bool Within(double x, double y) const {
            return x >= x_min && x <= x_max &&
                   y >= y_min && y <= y_max;
        }
    } const box;

//    LaneGeometryElement operator =(const LaneGeometryElement &rhs)
//    {
//        if(&rhs != this)
//        {
//            (Joints &)joints = rhs.joints;
//            (Box &)box = rhs.box;
//        }
//        return *this;
//    }
};

} // namespace Primitive
} // namespace OWL
