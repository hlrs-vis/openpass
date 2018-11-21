/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "polygon.h"

bool Polygon::CalculateCentroid( Common::Vector2d &cog)
{
    // if empty, return empty vector
    if (vertices.size() == 0) {
        return false;
    }

    // close polygon by adding the first vertex at the end
    std::vector<Common::Vector2d> v;
    v = vertices;
    v.push_back(v[0]);

    double area = 0;
    cog.x = 0;
    cog.y = 0;

    for (unsigned int iV = 0; iV < vertices.size() ; iV++ ) {
        double det;
        det = ( v[iV].x * v[iV + 1].y - v[iV + 1].x * v[iV].y );
        area = area + det;
        cog.x = cog.x + ( v[iV].x + v[iV + 1].x ) * det;
        cog.y = cog.y + ( v[iV].y + v[iV + 1].y ) * det;
    }

    cog.Scale( 1 / (3 * area) );

    return true;
}

double Polygon::GetNumberOfVertices()
{
    return vertices.size();
}

std::vector<Common::Vector2d> Polygon::GetVertices() const
{
    return vertices;
}

void Polygon::SetVertices(const std::vector<Common::Vector2d> &value)
{
    vertices = value;
}

void Polygon::Translate(Common::Vector2d translationVector)
{
    for (unsigned int i = 0; i < vertices.size(); i++ ) {
        vertices.at(i).Translate(translationVector);
    }
}

