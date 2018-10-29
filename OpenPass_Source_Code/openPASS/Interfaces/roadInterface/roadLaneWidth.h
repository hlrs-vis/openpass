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
//! @file  roadLaneWidth.h
//! @brief This file contains a class representing the polynomial defining the
//!        lane width.
//!        The polynomial has the form width = a + b*ds + c*ds^2 + d*ds^3.
//-----------------------------------------------------------------------------

#ifndef ROADLANEWIDTH
#define ROADLANEWIDTH

//-----------------------------------------------------------------------------
//! Class representing the polynomial defining the lane width. The polynomial has
//! the form width = a + b*ds + c*ds^2 + d*ds^3.
//-----------------------------------------------------------------------------
class RoadLaneWidth
{
public:
    RoadLaneWidth(double sOffset,
                  double a,
                  double b,
                  double c,
                  double d) :
        sOffset(sOffset),
        a(a),
        b(b),
        c(c),
        d(d)
    {}
    RoadLaneWidth(const RoadLaneWidth&) = delete;
    RoadLaneWidth(RoadLaneWidth&&) = delete;
    RoadLaneWidth& operator=(const RoadLaneWidth&) = delete;
    RoadLaneWidth& operator=(RoadLaneWidth&&) = delete;
    virtual ~RoadLaneWidth() = default;

    //-----------------------------------------------------------------------------
    //! Returns the starting position relative to the position of the preceding
    //! lane section.
    //!
    //! @return                         Offset relative to the preceding lane section
    //-----------------------------------------------------------------------------
    double GetSOffset() const
    {
        return sOffset;
    }

    //-----------------------------------------------------------------------------
    //! Returns the constant factor from the polynomial.
    //!
    //! @return                         Constant factor from the polynomial
    //-----------------------------------------------------------------------------
    double GetA() const
    {
        return a;
    }

    //-----------------------------------------------------------------------------
    //! Returns the linear factor from the polynomial.
    //!
    //! @return                         Linear factor from the polynomial
    //-----------------------------------------------------------------------------
    double GetB() const
    {
        return b;
    }

    //-----------------------------------------------------------------------------
    //! Returns the quadratic factor from the polynomial.
    //!
    //! @return                         Quadratic factor from the polynomial
    //-----------------------------------------------------------------------------
    double GetC() const
    {
        return c;
    }

    //-----------------------------------------------------------------------------
    //! Returns the cubic factor from the polynomial.
    //!
    //! @return                         Cubic factor from the polynomial
    //-----------------------------------------------------------------------------
    double GetD() const
    {
        return d;
    }

private:
    double sOffset;
    double a;
    double b;
    double c;
    double d;
};

#endif // ROADLANEWIDTH

