/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//! @file  lowpass.h
//! @brief This file contains a simple lowpass filter using averaging of a
//!        specific number of past values.

#ifndef LOWPASS_H
#define LOWPASS_H

#include <vector>

//! Class of a simple low pass filter
//! Be careful to initialize it properly with a vector of initial values having
//! the length of the filter window

class LowPassFilter
{
public:

    //! Standard constructor
    LowPassFilter():
        values(1), numValues(1), k(0)
    {}

    //! Constructor with initial values
    //!
    //! @param[in]     initialValues      The vector of the initial values, its length sets the filter length
    LowPassFilter(std::vector<double> &initialValues):
        values(initialValues),
        numValues(initialValues.size()), k(0)
    {}

    //! Constructor with filter length, no initial values are set
    //!
    //! @param[in]     numValues          The filter length
    LowPassFilter(int numValues):
        values(numValues), numValues(numValues), k(0)
    {}

    //! Function replacing an old value by a new one
    //!
    //! @param[in]     value              The new value
    void newValue(double value)
    {
        values[k] = value;
        k = (k + 1) % numValues;
        return;
    }

    //! Function calculating the filtered value
    //! the average is calculated everytime anew
    //!
    //! @return                           The filtered value
    double getFilteredValue()
    {
        double sum = 0;
        for (int i = 0; i < numValues; ++i) {
            sum += values[i];
        }
        return sum / numValues;
    }

private:
    std::vector<double> values;     //!< a vector of values within one filter frame
    int numValues;                  //!< filter length = size of the filter frame
    int k;                          //!< current internal index where in the vector a current value should be set
};

#endif // LOWPASS_H
