/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadLaneOffset.h
//! @brief This file contains a class representing a lane offset,
//!        i.e. a lateral shift of the reference line, as a cubic polynomial.
//-----------------------------------------------------------------------------

#ifndef ROADLANEOFFSET
#define ROADLANEOFFSET

//-----------------------------------------------------------------------------
//! Class representing a lane offset, i.e. a lateral shift of the reference line,
//! as a cubic polynomial.
//-----------------------------------------------------------------------------
class RoadLaneOffset
{
public:
    RoadLaneOffset(double s,
                   double a,
                   double b,
                   double c,
                   double d) :
        s(s),
        a(a),
        b(b),
        c(c),
        d(d)
    {}
    RoadLaneOffset(const RoadLaneOffset&) = delete;
    RoadLaneOffset(RoadLaneOffset&&) = delete;
    RoadLaneOffset& operator=(const RoadLaneOffset&) = delete;
    RoadLaneOffset& operator=(RoadLaneOffset&&) = delete;
    virtual ~RoadLaneOffset() = default;

    //-----------------------------------------------------------------------------
    //! Returns the start position (s-coordinate) of the offset.
    //!
    //! @return                         start position of the offset
    //-----------------------------------------------------------------------------
    double GetS() const
    {
        return s;
    }

    //-----------------------------------------------------------------------------
    //! Returns the constant factor of the polynomial.
    //!
    //! @return                         constant factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetA() const
    {
        return a;
    }

    //-----------------------------------------------------------------------------
    //! Returns the linear factor of the polynomial.
    //!
    //! @return                         linear factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetB() const
    {
        return b;
    }

    //-----------------------------------------------------------------------------
    //! Returns the quadratic factor of the polynomial.
    //!
    //! @return                         quadratic factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetC() const
    {
        return c;
    }

    //-----------------------------------------------------------------------------
    //! Returns the cubic factor of the polynomial.
    //!
    //! @return                         cubic factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetD() const
    {
        return d;
    }

private:
    double s;
    double a;
    double b;
    double c;
    double d;
};

#endif // ROADLANEOFFSET

