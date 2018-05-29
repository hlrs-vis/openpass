/******************************************************************************
* Copyright (c) 2018 in-tech on behalf of BMW AG
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//! \brief This file implements a generic 2D vector.

#pragma once

#include <vector>
//#include <boost/range/irange.hpp>

namespace Common
{
class LinearInterpolation
{
public:
    //-----------------------------------------------------------------------------
    //! @brief Calculate linear interpolated points with constant spacing.
    //!
    //! @param[in] start            Start of interval
    //! @param[in] end              End of interval
    //! @param[in] totalPoints      Total number of points returned (includes start
    //!                             and end point)
    //!
    //! @return                     Vector of interpolated points.
    //-----------------------------------------------------------------------------
    static std::vector<double> InterpolateLinear(const double start, const double end, const int totalPoints);
};

//-----------------------------------------------------------------------------
//! @brief Round doubles.
//!
//! Rounds doubles to a given amount of decimals.
//!
//! @param[in] value            Value which is rounded
//! @param[in] decimals         Amount of decimals.
//!
//! @return                     Rounded value.
//-----------------------------------------------------------------------------
double roundDoubleWithDecimals(double value, int decimals);

} // namespace Common


