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

#include <string>
#include "roadElementTypes.h"

class RoadObjectInterface// : public RoadElementInterface
{
public:
    RoadObjectInterface() = default;
    RoadObjectInterface(const RoadObjectInterface&) = delete;
    RoadObjectInterface(RoadObjectInterface&&) = delete;
    RoadObjectInterface& operator=(const RoadObjectInterface&) = delete;
    RoadObjectInterface& operator=(RoadObjectInterface&&) = delete;
    virtual ~RoadObjectInterface() = default;

    virtual RoadObjectType GetType() const = 0;
    virtual std::string GetId() const = 0;
    virtual double GetS() const = 0;
    virtual double GetT() const = 0;
    virtual bool IsValidForLane(int laneId) const = 0;
    virtual double GetLength() const = 0;
    virtual double GetWidth() const = 0;
    virtual double GetHdg() const = 0;
    virtual double GetHeight() const = 0;
    virtual double GetPitch() const = 0;
    virtual double GetRoll() const = 0;
};



