/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  pcm_line.h
//! @brief class to store a line which consists of pcm points.
//!
//! This class is responsible to manage data of a pcm line.
//-----------------------------------------------------------------------------

#ifndef PCM_LINE_H
#define PCM_LINE_H

#include <QString>
#include <map>
#include "pcm_pointContainer.h"

/*!
 * \brief The PCM_Line class
 * This class is responsible to manage data of a pcm line.
 */
class PCM_Line : public PCM_PointContainer
{
public:

    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     id     id of line
    //-----------------------------------------------------------------------------
    PCM_Line(int id);

    // removing operators
    PCM_Line(const PCM_Line &) = default;
    PCM_Line(PCM_Line &&) = default;
    PCM_Line &operator=(const PCM_Line &) = delete;
    PCM_Line &operator=(PCM_Line &&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_Line() = default;

    //-----------------------------------------------------------------------------
    //! Getter function
    //! @return     id
    //-----------------------------------------------------------------------------
    int GetId() const;

private:
    int id = -1;                        //!< id of line
};

#endif // PCM_LINE_H
