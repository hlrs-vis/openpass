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
//! @file  pcm_viewObject.h
//! @brief class to store a viewObject of pcm.
//!
//! This class is responsible to manage data of pcm viewObjects which consists
//! of pcm lines.
//-----------------------------------------------------------------------------

#ifndef PCM_VIEWOBJECT_H
#define PCM_VIEWOBJECT_H

#include "pcm_lineContainer.h"

/*!
 * \brief The PCM_ViewObject class
 * This class is responsible to manage data of pcm viewObjects which consists
 * of pcm lines.
 */
class PCM_ViewObject : public PCM_LineContainer
{
public:
    //-----------------------------------------------------------------------------
    //! Default Constructor
    //-----------------------------------------------------------------------------
    PCM_ViewObject() = default;

    // removing operators
    PCM_ViewObject(const PCM_ViewObject &) = default;
    PCM_ViewObject(PCM_ViewObject &&) = default;
    PCM_ViewObject &operator=(const PCM_ViewObject &) = delete;
    PCM_ViewObject &operator=(PCM_ViewObject &&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_ViewObject() = default;
};

#endif // PCM_VIEWOBJECT_H
