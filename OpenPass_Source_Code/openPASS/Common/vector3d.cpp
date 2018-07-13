/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <cmath>
#include "vector3d.h"

namespace Common
{

Vector3d::Vector3d(double x, double y, double z) : x(x), y(y), z(z)
{}

void Vector3d::Translate(double x, double y, double z)
{
    this->x += x;
    this->y += y;
    this->z += z;
}

void Vector3d::Scale(double scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
}

void Vector3d::Add(const Vector3d &in)
{
    x += in.x;
    y += in.y;
    z += in.z;
}

void Vector3d::Sub(const Vector3d &in)
{
    x -= in.x;
    y -= in.y;
    z -= in.z;
}

bool Vector3d::Norm()
{
    double length = std::sqrt(x*x + y*y + z*z);
    if(0 == length)
    {
        return false;
    }

    x /= length;
    y /= length;
    z /= length;

    return true;
}

double Vector3d::Length() const
{
    return std::sqrt(x*x + y*y + z*z);
}

Vector3d Vector3d::operator-(const Vector3d &in)
{
    return Vector3d(x - in.x, y - in.y, z - in.z);
}

Vector3d Vector3d::operator+(const Vector3d &in)
{
    return Vector3d(x + in.x, y + in.y, z + in.z);
}

Vector3d Vector3d::operator*(double in)
{
    return Vector3d(x * in, y * in, z * in);
}

bool Vector3d::operator==(const Vector3d &in) const
{
    return x==in.x && y==in.y && z==in.z;
}

} // namespace Common
