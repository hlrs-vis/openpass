/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  pcm_point.h
//! @brief class to store point of pcm.
//!
//! This class is responsible to manage data of a pcm point.
//-----------------------------------------------------------------------------

#ifndef PCM_POINT_H
#define PCM_POINT_H

#include <QString>
#include <cmath>

/*!
 * \brief The PCM_Point class
 * This class is responsible to manage data of a pcm point.
 */
class PCM_Point
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     id       id of point
    //! @param[in]     inXpos   initial x-coordinate
    //! @param[in]     inYpos   initial y-coordinate
    //! @param[in]     inZpos   initial z-coordinate
    //-----------------------------------------------------------------------------
    PCM_Point(int id = -1, double inXpos = INFINITY, double inYpos = INFINITY,
              double inZpos = INFINITY);

    // removing operators
    PCM_Point(const PCM_Point &) = default;
    PCM_Point(PCM_Point &&) = default;
    PCM_Point &operator=(const PCM_Point &) = default;
    PCM_Point &operator=(PCM_Point &&) = default;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_Point() = default;

    //-----------------------------------------------------------------------------
    //! Getter function
    //! @return     id
    //-----------------------------------------------------------------------------
    int GetId() const ;

    //-----------------------------------------------------------------------------
    //! Getter function
    //! @return     x position
    //-----------------------------------------------------------------------------
    double GetX() const;

    //-----------------------------------------------------------------------------
    //! Getter function
    //! @return     y position
    //-----------------------------------------------------------------------------
    double GetY() const;

    //-----------------------------------------------------------------------------
    //! Getter function
    //! @return     z position
    //-----------------------------------------------------------------------------
    double GetZ() const;

private:
    int id = -1;     //!< id of point
    double x = 0;    //!< x position of point
    double y = 0;    //!< y position of point
    double z = 0;    //!< z position of point
};

#endif // PCM_POINT_H
