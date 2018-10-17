#pragma once

#include "roadInterface.h"
#include "roadSignalInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a road signal
//-----------------------------------------------------------------------------
class RoadSignal : public RoadSignalInterface
{
public:
    RoadSignal(RoadInterface *road, const RoadSignalSpecification signal) :
        road{road},
        signal{signal}
    {}

    //-----------------------------------------------------------------------------
    /// @brief Returns the type of the signal (e.g. code according to StVO)
    /// @return type
    //-----------------------------------------------------------------------------
    RoadSignalType GetType() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the unique identification string of the signal
    /// @return id
    //-----------------------------------------------------------------------------
    virtual std::string GetId() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns a value depending in SI units
    ///        If a unit is specified, the according conversion is applied
    /// @see   OpenDRIVEFormatSpecRev1.4H.pdf Section 2.2 (page 12)
    /// @return value in SI units
    //-----------------------------------------------------------------------------
    double GetValue() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the s coordinate of the signal
    ///
    /// @return s [m]
    //-----------------------------------------------------------------------------
    double GetS() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the t coordinate of the signal
    ///
    /// @return s [m]
    //-----------------------------------------------------------------------------
    double GetT() const;

    //-----------------------------------------------------------------------------
    //! Returns the road from which this section is a part of.
    //!
    //! @return                         Road from which this section is a part of
    //-----------------------------------------------------------------------------
    RoadInterface *GetRoad()
    {
        return road;
    }

    //-----------------------------------------------------------------------------
    //! Returns if the signal is valid for a given lane
    //! @param[in] laneId   lane Id of interest
    //!
    //! @return             true if signal is valid
    //-----------------------------------------------------------------------------
    bool IsValidForLane(int laneId) const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the height of the signal
    ///
    /// @return height [m]
    //-----------------------------------------------------------------------------
    virtual double GetHeight() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the width of the signal
    ///
    /// @return height [m]
    //-----------------------------------------------------------------------------
    virtual double GetWidth() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the length of the signal
    ///
    /// @return height [m]
    //-----------------------------------------------------------------------------
    virtual double GetLength() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the pitch of the signal
    ///
    /// @return height [m]
    //-----------------------------------------------------------------------------
    virtual double GetPitch() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the roll of the signal
    ///
    /// @return height [m]
    //-----------------------------------------------------------------------------
    virtual double GetRoll() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns the yaw of the signal
    ///
    /// @return height [m]
    //-----------------------------------------------------------------------------
    virtual double GetYaw() const;

    //-----------------------------------------------------------------------------
    /// @brief Returns true if the signal is dynamic
    ///
    /// @return true if dynamic == "yes"
    //-----------------------------------------------------------------------------
    virtual bool GetIsDynamic() const;

private:
    RoadInterface* road;
    const RoadSignalSpecification signal;

    const double convert_km   =    1.00  / 1000.0;
    const double convert_ft   =    1.00  / 3.28084;
    const double convert_mile =    1.00  / 1609.344;
    const double convert_mph  = 1609.344 / 3600;
    const double convert_km_h = 1000.00  / 3600;
    const double convert_t    =    1.00  / 1000;

public:
    RoadSignal(const RoadSignal&) = delete;
    RoadSignal(RoadSignal&&) = delete;
    RoadSignal& operator=(const RoadSignal&) = delete;
    RoadSignal& operator=(RoadSignal&&) = delete;
    virtual ~RoadSignal() = default;
};
