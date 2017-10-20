/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  callbacks.h
//! @brief This file contains the implementation of the model callbacks to
//!        interact with the framework.
//-----------------------------------------------------------------------------

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <string>
#include "callbackInterface.h"

namespace SimulationCommon
{

//! callback implementation within framework
class Callbacks : public CallbackInterface
{
public:
    Callbacks() = default;
    Callbacks(const Callbacks&) = delete;
    Callbacks(Callbacks&&) = delete;
    Callbacks& operator=(const Callbacks&) = delete;
    Callbacks& operator=(Callbacks&&) = delete;
    virtual ~Callbacks() = default;

    // model callbacks

    //-----------------------------------------------------------------------------
    //! Logs a provided message yielded from the provided line in the provided file
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file which triggered the logging
    //! @param[in]     line        Line within the file that triggered the logging
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    virtual void Log(CbkLogLevel logLevel,
                     const char *file,
                     int line,
                     const std::string &message) const;
};

} // namespace SimulationCommon

#endif // CALLBACKS_H
