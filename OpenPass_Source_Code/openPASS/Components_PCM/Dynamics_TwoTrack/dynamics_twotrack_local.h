/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef DYNAMICS_TWOTRACK_LOCAL_H
#define DYNAMICS_TWOTRACK_LOCAL_H


#include "vector2d.h"

template<typename DT>
DT Saturate(const DT value, const DT limitLow, const DT limitHigh)
{
    if (!(value > limitLow)) {
        return limitLow;
    } else if (value > limitHigh) {
        return limitHigh;
    } else {
        return value;
    }
}

#endif // DYNAMICS_TWOTRACK_LOCAL_H
