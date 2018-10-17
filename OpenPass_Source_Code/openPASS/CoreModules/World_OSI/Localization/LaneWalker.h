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

#include <OWL/DataTypes.h>

namespace OWL
{

class LaneWalker
{
private:
    const Interfaces::Lane& lane;

    Interfaces::LaneGeometryElements::const_iterator element;
    Interfaces::LaneGeometryElements::const_iterator endOfElements;

public:
    LaneWalker(const Interfaces::Lane& lane);

    void Step();
    bool IsDepleted() const;

    const Interfaces::Lane& GetLane() const;
    const Primitive::LaneGeometryElement& GetLaneGeometryElement() const;
    double GetSOffset() const;
};

}
