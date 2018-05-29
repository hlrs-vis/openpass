#pragma once

#include <string>
#include "roadElementTypes.h"
//#include "RoadElementInterface.h"

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
};



