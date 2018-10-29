/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_point.h"

PCM_Point::PCM_Point(int id, double inXpos, double inYpos, double inZpos):
    id(id),
    x(inXpos),
    y(inYpos),
    z(inZpos)
{}

int PCM_Point::GetId() const
{
    return id;
}

double PCM_Point::GetX() const
{
    return x;
}

double PCM_Point::GetY() const
{
    return y;
}

double PCM_Point::GetZ() const
{
    return z;
}
