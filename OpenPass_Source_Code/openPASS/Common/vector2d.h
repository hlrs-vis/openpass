/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//! \brief This file implements a generic 2D vector.

#ifndef VECTOR2D_H
#define VECTOR2D_H

namespace Common {

/*!
 * class for 2d vectors in cartesian coordinate system
 */
class Vector2d
{
public:
    /*!
     * create 2d vector from pair (x,y)
     *
     * \param[in] x     x-value
     * \param[in] y     y-value
     */
    Vector2d(double x = 0, double y = 0);
    Vector2d(const Vector2d &) = default;
    Vector2d(Vector2d &&) = default;
    Vector2d &operator=(const Vector2d &) = default;
    Vector2d &operator=(Vector2d &&) = default;
    virtual ~Vector2d() = default;

    /*!
     * translation of vector
     *
     * \param[in] x    x-value of displacement vector
     * \param[in] y    y-value of displacement vector
     */
    void Translate(double x, double y);

    /*!
     * \brief Translate
     * translation of vector via another vector
     * \param[in] translationVector vector of translation
     */
    void Translate(Vector2d translationVector);

    /*!
     * rotates vector by angle
     *
     * \param[in] angle     angle, in radians
     */
    void Rotate(double angle);

    /*!
     * scales vector by a factor
     *
     * \param[in] scale     scaling factor
     */
    void Scale(double scale);

    /*!
     * adds a vector
     *
     * \param[in] in     added 2d vector
     */
    void Add(const Vector2d &in);

    /*!
     * subtracts a vector
     *
     * \param[in] in     subtracted 2d vector
     */
    void Sub(const Vector2d &in);

    /*!
     * scalar product / dot product
     *
     * \param[in] in      2d vector
     * \return returns dot product of the 2 vectors
     */
    double Dot(const Vector2d &in);

    /*!
     * cross product with Z=0
     *
     * \param[in] in      2d vector
     * \return returns z-component of the cross product
     */
    double Cross(const Vector2d &in);

    /*!
     * Normalizes the 2d vector
     *
     * Each component of the vector is devided by the length of the vector.
     *
     * In case of a vector with length 0, the vector cannot be normalized and false is returned.
     *
     * \return returns true if vector could be normalized, false otherwise
     */
    bool Norm();

    /*!
     * returns length of the vector
     *
     * \return length of the vector
     */
    double Length() const;

    /*!
     * \brief Angle
     * returns the angle of the vector [-pi,+pi]
     * \return angle of vector
     */
    double Angle() const;

    Vector2d operator-(const Vector2d &in);
    Vector2d operator+(const Vector2d &in);
    Vector2d operator*(double in);
    bool operator==(const Vector2d &in) const;

    double x;
    double y;
};

} // namespace Common

#endif // VECTOR2D_H
