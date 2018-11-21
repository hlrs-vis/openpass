#pragma once

#include "roadInterface.h"
#include "roadObjectInterface.h"

class RoadObject : public RoadObjectInterface
{
public:
    RoadObject(RoadInterface *road, const RoadObjectSpecification object) :
        road(road),
        object(object)
    {}

    //-----------------------------------------------------------------------------
    //! @brief Returns the unique identification string of the road object
    //! @return                     id
    //-----------------------------------------------------------------------------
    std::string GetId() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the s coordinate of the road object
    //! @return                     s coordinate [m]
    //-----------------------------------------------------------------------------
    double GetS() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the t coordinate of the road object
    //! @return                     t coordinate [m]
    //-----------------------------------------------------------------------------
    double GetT() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the heading of the road object (relative to road direction)
    //! @return                     heading [rad]
    //-----------------------------------------------------------------------------
    double GetHdg() const;

    // TODO: documentation, virtual?
    double GetHeight() const;
    double GetPitch() const;
    double GetRoll() const;

    //-----------------------------------------------------------------------------
    //! @brief Check, if road object is valid for provided lane id
    //!
    //! @param[in]  laneId          lane id to test validity for
    //!
    //! @return                     True if valid, false otherwise.
    //-----------------------------------------------------------------------------
    bool IsValidForLane(int laneId) const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the type of the road object
    //! @return                     type
    //-----------------------------------------------------------------------------
    RoadObjectType GetType() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the length of the road object
    //! @return                     length [m]
    //-----------------------------------------------------------------------------
    double GetLength() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the width of the road object
    //! @return                     width [m]
    //-----------------------------------------------------------------------------
    double GetWidth() const;

private:
    RoadInterface* road;
    const RoadObjectSpecification object;

public:
    RoadObject(const RoadObject&) = delete;
    RoadObject(RoadObject&&) = delete;
    RoadObject& operator=(const RoadObject&) = delete;
    RoadObject& operator=(RoadObject&&) = delete;
    virtual ~RoadObject() = default;
};
