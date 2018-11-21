/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "vector2d.h"

/*!
 * \brief polygon in 2d
 *
 * This class represents a polygon in 2d.
 * It consists of several vertices which are vectors in 2d.
 * It provides several operations for working with the polygon.
 */
class Polygon
{
public:
    Polygon(std::vector<Common::Vector2d> vertices)
        : vertices(vertices)
    {}
    virtual ~Polygon() = default;

    /*!
     * \brief get centroid of the polygon
     * Algorithm to calculate the center of the polygon.
     *
     * Notes:
     * - the terms center of gravity and centroid are often used synonymous
     * - algorithm works for clockwise and counter-clockwise order of the vertices
     *
     * The algorithm is based on MATLAB-code provided by Harald Kolk (TU Graz).
     * See also: https://en.wikipedia.org/wiki/Centroid
     *
     * \param[out] cog   centroid of the polygon
     * \return           true if calculation was successful
     */
    bool CalculateCentroid( Common::Vector2d &cog);

    /*!
     * \brief get number of vertices of the polygon
     * \return number of vertices of the polygon
     */
    double GetNumberOfVertices();

    /*!
     * \brief get vertices of the polygon
     * \return vertices of polygon
     */
    std::vector<Common::Vector2d> GetVertices() const;

    /*!
     * \brief set vertices of the polygon
     * \param vertices of the polygon
     */
    void SetVertices(const std::vector<Common::Vector2d> &value);

    /*!
     * \brief translate the polygon
     * Translate the polygon along a given vector.
     *
     * \param translationVector         vector for translation
     */
    void Translate(Common::Vector2d translationVector);

private:
    std::vector<Common::Vector2d> vertices;
};

#endif // POLYGON_H
