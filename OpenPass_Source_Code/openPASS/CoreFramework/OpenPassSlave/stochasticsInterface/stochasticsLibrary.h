/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  StochasticsLibrary.h
//! @brief This file contains the internal representation of the library of a
//!        stochasticsInterface.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <QLibrary>
#include "stochasticsBinding.h"
#include "Interfaces/stochasticsInterface.h"
#include "stochastics.h"
#include "Interfaces/callbackInterface.h"

namespace SimulationSlave
{

class StochasticsLibrary
{
public:
    typedef const std::string &(*StochasticsInterface_GetVersion)();
    typedef StochasticsInterface *(*StochasticsInterface_CreateInstanceType)(
            const CallbackInterface *callbacks);
    typedef void (*StochasticsInterface_DestroyInstanceType)(StochasticsInterface *implementation);

    StochasticsLibrary(const std::string libraryPath,
                       CallbackInterface *callbacks):
        libraryPath(libraryPath),
        callbacks(callbacks)
    {}
    StochasticsLibrary(const StochasticsLibrary&) = delete;
    StochasticsLibrary(StochasticsLibrary&&) = delete;
    StochasticsLibrary& operator=(const StochasticsLibrary&) = delete;
    StochasticsLibrary& operator=(StochasticsLibrary&&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor, deletes the stored library (unloads it if necessary)
    //-----------------------------------------------------------------------------
    virtual ~StochasticsLibrary();

    //-----------------------------------------------------------------------------
    //! Creates a QLibrary based on the path from the constructor and stores function
    //! pointer for getting the library version, creating and destroying instances
    //! and setting the stochasticsInterface item
    //! (see typedefs for corresponding signatures).
    //!
    //! @return                 Null pointer
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    //! Delete the stochasticsInterface and the library
    //!
    //! @return                         Flag if the release was successful
    //-----------------------------------------------------------------------------
    bool ReleaseStochastics();

    //-----------------------------------------------------------------------------
    //! Make sure that the library exists and is loaded, then call the "create instance"
    //! function pointer using the parameters from the stochastics instance to get
    //! a stochastics interface, which is then used to instantiate a stochasticsInterface
    //! which is stored.
    //!
    //! @return                         stochasticsInterface created
    //-----------------------------------------------------------------------------
    StochasticsInterface *CreateStochastics();

private:
    const std::string DllGetVersionId = "OpenPASS_GetVersion";
    const std::string DllCreateInstanceId = "OpenPASS_CreateInstance";
    const std::string DllDestroyInstanceId = "OpenPASS_DestroyInstance";

    std::string libraryPath;
    StochasticsInterface *stochasticsInterface = nullptr;
    QLibrary *library = nullptr;
    CallbackInterface *callbacks;
    StochasticsInterface_GetVersion getVersionFunc{nullptr};
    StochasticsInterface_CreateInstanceType createInstanceFunc{nullptr};
    StochasticsInterface_DestroyInstanceType destroyInstanceFunc{nullptr};
};

} // namespace SimulationSlave


