#pragma once

#include <string>
#include "roadElementTypes.h"
//#include "RoadElementInterface.h"

class RoadSignalInterface// : public RoadElementInterface
{
public:
    RoadSignalInterface() = default;
    RoadSignalInterface(const RoadSignalInterface&) = delete;
    RoadSignalInterface(RoadSignalInterface&&) = delete;
    RoadSignalInterface& operator=(const RoadSignalInterface&) = delete;
    RoadSignalInterface& operator=(RoadSignalInterface&&) = delete;
    virtual ~RoadSignalInterface() = default;

    virtual std::string GetId() const = 0;
    virtual double GetS() const = 0;
    virtual bool IsValidForLane(int laneId) const = 0;
    virtual RoadSignalType GetType() const = 0;
    virtual double GetValue() const = 0;
};

