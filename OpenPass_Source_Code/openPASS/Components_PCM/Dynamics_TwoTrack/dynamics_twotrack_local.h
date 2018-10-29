/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
