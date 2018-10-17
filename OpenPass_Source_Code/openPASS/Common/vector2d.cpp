/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <cmath>
#include "vector2d.h"

#define EPSILON 1e-9

namespace Common {

Vector2d::Vector2d(double x, double y) : x(x), y(y)
{}

void Vector2d::Translate(double x, double y)
{
    this->x += x;
    this->y += y;
}

void Vector2d::Translate(Vector2d translationVector)
{
    this->x += translationVector.x;
    this->y += translationVector.y;
}

void Vector2d::Rotate(double angle)
{
    double cosAngle = std::cos(angle);
    double sinAngle = std::sin(angle);
    double tmpX = x * cosAngle - y * sinAngle;
    y = x * sinAngle + y * cosAngle;
    x = tmpX;
}

void Vector2d::Scale(double scale)
{
    x *= scale;
    y *= scale;
}

void Vector2d::Add(const Vector2d &in)
{
    x += in.x;
    y += in.y;
}

void Vector2d::Sub(const Vector2d &in)
{
    x -= in.x;
    y -= in.y;
}

double Vector2d::Dot(const Vector2d &in) const
{
    return x * in.x + y * in.y;
}

double Vector2d::Cross(const Vector2d &in) const
{
    return x * in.y - y * in.x;
}

bool Vector2d::Norm()
{
    double length = std::sqrt(x * x + y * y);
    if (0 == length) {
        return false;
    }

    x /= length;
    y /= length;

    return true;
}

double Vector2d::Length() const
{
    return std::sqrt(x * x + y * y);
}

double Vector2d::Angle() const
{
    return atan2(y, x);
}

Vector2d Vector2d::operator-(const Vector2d &in) const
{
    return Vector2d(x - in.x, y - in.y);
}

Vector2d Vector2d::operator+(const Vector2d &in) const
{
    return Vector2d(x + in.x, y + in.y);
}

Vector2d Vector2d::operator*(double in) const
{
    return Vector2d(x * in, y * in);
}

bool Vector2d::operator==(const Vector2d &in) const
{
    return (std::abs(x - in.x) < EPSILON) &&
           (std::abs(y - in.y) < EPSILON);
}

} // namespace Common
