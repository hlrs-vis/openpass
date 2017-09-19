/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <cmath>
#include "vector2d.h"

namespace Common
{

Vector2d::Vector2d(double x, double y) : x(x), y(y)
{}

void Vector2d::Translate(double x, double y)
{
    this->x += x;
    this->y += y;
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

double Vector2d::Dot(const Vector2d &in)
{
    return x * in.x + y * in.y;
}

bool Vector2d::Norm()
{
    double length = std::sqrt(x*x + y*y);
    if(0 == length)
    {
        return false;
    }

    x /= length;
    y /= length;

    return true;
}

double Vector2d::Length() const
{
    return std::sqrt(x*x + y*y);
}

Vector2d Vector2d::operator-(const Vector2d &in)
{
    return Vector2d(x - in.x, y - in.y);
}

Vector2d Vector2d::operator+(const Vector2d &in)
{
    return Vector2d(x + in.x, y + in.y);
}

Vector2d Vector2d::operator*(double in)
{
    return Vector2d(x * in, y * in);
}

bool Vector2d::operator==(const Vector2d &in) const
{
    return x==in.x && y==in.y;
}

} // namespace Common
