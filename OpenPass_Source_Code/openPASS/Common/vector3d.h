/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//! \brief This file implements a generic 3D vector.

#pragma once

namespace Common
{

/*!
 * class for 3d vectors in cartesian coordinate system
 */
class Vector3d
{
public:
    /*!
     * create 2d vector from pair (x, y, z)
     *
     * \param[in] x     x-value
     * \param[in] y     y-value
     * \param[in] z     z-value
     */
    Vector3d(double x = 0, double y = 0, double z = 0);
    Vector3d(const Vector3d&) = default;
    Vector3d(Vector3d&&) = default;
    Vector3d& operator=(const Vector3d&) = default;
    Vector3d& operator=(Vector3d&&) = default;
    virtual ~Vector3d() = default;

    /*!
     * translation of vector
     *
     * \param[in] x    x-value of displacement vector
     * \param[in] y    y-value of displacement vector
     * \param[in] z    z-value of displacement vector
     */
    void Translate(double x, double y, double z);

    /*!
     * scales vector by a factor
     *
     * \param[in] scale     scaling factor
     */
    void Scale(double scale);

    /*!
     * adds a vector
     *
     * \param[in] in     added 3d vector
     */
    void Add(const Vector3d &in);

    /*!
     * subtracts a vector
     *
     * \param[in] in     subtracted 3d vector
     */
    void Sub(const Vector3d &in);

    /*!
     * Normalizes the 3d vector
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

    Vector3d operator-(const Vector3d &in);
    Vector3d operator+(const Vector3d &in);
    Vector3d operator*(double in);
    bool operator==(const Vector3d &in) const;

    double x;
    double y;
    double z;
};

} // namespace Common


