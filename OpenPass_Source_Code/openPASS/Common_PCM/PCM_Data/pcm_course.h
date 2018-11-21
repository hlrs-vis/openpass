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
//! @file  pcm_course.h
//! @brief class to store a course which consists of pcm points.
//!
//! This class is responsible to manage data of a pcm course.
//-----------------------------------------------------------------------------

#ifndef PCM_COURSE_H
#define PCM_COURSE_H

#include <map>
#include "pcm_pointContainer.h"

/*!
 * \brief The PCM_Line class
 * This class is responsible to manage data of a pcm course.
 */
class PCM_Course : public PCM_PointContainer
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     betNr     betNr of course
    //-----------------------------------------------------------------------------
    PCM_Course(int betNr);

    // removing operators
    PCM_Course(const PCM_Course &) = default;
    PCM_Course(PCM_Course &&) = default;
    PCM_Course &operator=(const PCM_Course &) = delete;
    PCM_Course &operator=(PCM_Course &&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_Course() = default;

    //-----------------------------------------------------------------------------
    //! Getter function
    //! @return     betNr
    //-----------------------------------------------------------------------------
    int GetBetNr() const;

private:
    int betNr = -1; //!< BETNR
};

#endif // PCM_COURSE_H
