/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  callbackInterface.h
//! @brief This file contains the interface of the module callbacks to
//!        interact with the framework.
//-----------------------------------------------------------------------------

#ifndef CALLBACKINTERFACE_H
#define CALLBACKINTERFACE_H

#include <string>

//-----------------------------------------------------------------------------
//! The following macro should only be called within classes providing a Log() member function
//! (e.g. classes derived from ModelInterface, ObservationInterface SpawnPointInterface).
//-----------------------------------------------------------------------------
#define LOG(level, message) Log(level, __FILE__, __LINE__, message)

//-----------------------------------------------------------------------------
//! Log level for the log callback
//-----------------------------------------------------------------------------
enum class CbkLogLevel : int {
    Error = 0,      //!< indicates abortion of program flow (e.g. on return values indicating an error)
    Warning,        //!< indicates change of intended program flow (doesn't necessarily lead to an error)
    Info,           //!< used for diagnostic information (should not be used for periodic events to prevent overflow of log)
    Debug           //!< used for debugging information (might be used for periodic events)
};

//-----------------------------------------------------------------------------
//! Interface for callbacks to framework
//-----------------------------------------------------------------------------
class CallbackInterface
{
public:
    CallbackInterface() = default;
    CallbackInterface(const CallbackInterface &) = delete;
    CallbackInterface(CallbackInterface &&) = delete;
    CallbackInterface &operator=(const CallbackInterface &) = delete;
    CallbackInterface &operator=(CallbackInterface &&) = delete;
    virtual ~CallbackInterface() = default;

    //-------------------------------------------------------------------------
    //! Provides callback to LOG() macro.
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-------------------------------------------------------------------------
    virtual void Log(CbkLogLevel logLevel,
                     const char *file,
                     int line,
                     const std::string &message) const = 0;
};

#endif // CALLBACKINTERFACE_H
