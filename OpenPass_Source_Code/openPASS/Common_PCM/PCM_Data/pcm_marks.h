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
//! @file  pcm_marks.h
//! @brief class to store marks of pcm.
//!
//! This class is responsible to manage data of pcm marks which consists of pcm
//! lines.
//-----------------------------------------------------------------------------

#ifndef PCM_MARKS_H
#define PCM_MARKS_H

#include <QString>
#include "pcm_lineContainer.h"
#include "globalDefinitions.h"

/*!
 * \brief The PCM_Marks class
 * This class is responsible to manage data of pcm marks which consists of pcm
 * lines.
 */
class PCM_Marks : public PCM_LineContainer
{
public:
    //-----------------------------------------------------------------------------
    //! Default Constructor
    //-----------------------------------------------------------------------------
    PCM_Marks();
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     markType     type of mark to set initially
    //-----------------------------------------------------------------------------
    PCM_Marks(MarkType markType);

    // removing operators
    PCM_Marks(const PCM_Marks &) = default;
    PCM_Marks(PCM_Marks &&) = default;
    PCM_Marks &operator=(const PCM_Marks &) = delete;
    PCM_Marks &operator=(PCM_Marks &&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_Marks() = default;

    //-----------------------------------------------------------------------------
    //! Set the type of mark
    //!
    //! @param[in]     markType type of mark
    //! @return                 true on success
    //-----------------------------------------------------------------------------
    bool SetMarkType(MarkType markType);

    //-----------------------------------------------------------------------------
    //! Get the type marks
    //!
    //! @return                 mark type
    //-----------------------------------------------------------------------------
    MarkType GetMarkType() const;

private:
    MarkType markType;                 //!< type of marks
};

#endif // PCM_MARKS_H
