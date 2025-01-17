/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "utilities.h"
#include <cmath>


namespace Common
{

std::vector<double> LinearInterpolation::InterpolateLinear(const double start, const double end, const int totalPoints)
{
    std::vector<double> elements;

    for(int i = 0; i < totalPoints; ++i)
    {
        elements.push_back(start + (i * ((end-start) / (totalPoints-1))));
        //elements.push_back(start + (i * ((end-start+1) / (totalPoints))));  //not correct implemented yet
    }
    return elements;
}

double roundDoubleWithDecimals(double value, int decimals)
{
    return std::floor((value * (std::pow(10, decimals))) + 0.5)/(std::pow(10.0, decimals));
}

} //namespace Common

