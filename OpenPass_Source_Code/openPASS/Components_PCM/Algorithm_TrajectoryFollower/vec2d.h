/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//! @file  vec2d.h
//! @brief This file contains a definition of a vector class for vectors in
//!        two dimensional euclidean space.

#ifndef VEC2D_H
#define VEC2D_H

#define _USE_MATH_DEFINES
#include <math.h>

//! class for vectors in two dimensional euclidean space.
class Vec2D{
public:
    //public access for ease of use and limited overhead
    double x = 0;             //!< the x coordinate
    double y = 0;             //!< the y coordinate

    //! Standard Constructor
    Vec2D(){}

    //! Constructor
    //!
    //! @param[in]     a      The x coordinate
    //! @param[in]     b      The y coordinate
    Vec2D(double a,double b):
        x(a), y(b)
    {}

    //! Operator performing the vector sum
    //!
    //! @param[in]     r      The vector to be added to the current one
    //! @return               The sum of the current and the input vector
    inline Vec2D operator+(Vec2D r)
    {
        return Vec2D(x+r.x,y+r.y);
    }

    //! Operator performing the vector difference
    //!
    //! @param[in]     u      The vector to be substracted from the current one
    //! @return               The difference of the current and the input vector
    inline Vec2D operator-(Vec2D u)
    {
        return Vec2D(x-u.x,y-u.y);
    }

    //! Operator performing a scalar multiplication
    //!
    //! @param[in]     r      The multiplicator
    //! @return               The multiplied vector
    inline Vec2D operator*(double r)
    {
        return Vec2D(x*r,y*r);
    }

    //! Operator performing a dot product
    //!
    //! @param[in]     u      The vector to be multiplied to the current one
    //! @return               The dot product between the two vectors
    inline double operator%(Vec2D u)
    {
        return x*u.x+y*u.y;
    }

    //! Function calculating the norm of the current vector
    //!
    //! @return               The norm of the current vector
    inline double norm()
    {
        return sqrt(x*x+y*y);
    }

    //! Function calculating the squared norm of the current vector
    //!
    //! @return               The squared norm of the current vector
    inline double squaredNorm()
    {
        return x*x+y*y;
    }

    //! Operator that normalises the current vector
    //!
    //! @return               The normalised current vector
    inline Vec2D operator!()
    {
        double s = sqrt(x*x+y*y);
        if(s==0) return *this; //zero vector cannot be normalized
        return*this*(1/s);
    }
};


#endif // VEC2D_H
