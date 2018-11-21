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
//! @file  pcm_object.h
//! @brief class to store a object of pcm.
//!
//! This class is responsible to manage data of pcm objects which consists of
//! pcm lines.
//-----------------------------------------------------------------------------

#ifndef PCM_OBJECT_H
#define PCM_OBJECT_H

#include "pcm_lineContainer.h"

/*!
 * \brief The PCM_Object class
 * This class is responsible to manage data of pcm objects which consists of
 * pcm lines.
 */
class PCM_Object : public PCM_LineContainer
{
public:
    //-----------------------------------------------------------------------------
    //! Default Constructor
    //-----------------------------------------------------------------------------
    PCM_Object() = default;
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     objectType     type of object
    //-----------------------------------------------------------------------------
    PCM_Object(int objectType);

    // removing operators
    PCM_Object(const PCM_Object &) = default;
    PCM_Object(PCM_Object &&) = default;
    PCM_Object &operator=(const PCM_Object &) = delete;
    PCM_Object &operator=(PCM_Object &&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_Object() = default;

    //-----------------------------------------------------------------------------
    //! Set the type of object
    //!
    //! @param[in]     objectType   type of object
    //! @return                     true on success
    //-----------------------------------------------------------------------------
    bool SetObjectType(int objectType);

    //-----------------------------------------------------------------------------
    //! Get the type object
    //!
    //! @return                 object type
    //-----------------------------------------------------------------------------
    int GetObjectType() const;

private:
    int objectType = -1;                 //!< type of object
};

#endif // PCM_OBJECT_H
