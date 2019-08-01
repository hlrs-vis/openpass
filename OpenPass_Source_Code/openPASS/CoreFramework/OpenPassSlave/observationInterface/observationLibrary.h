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
//! @file  ObservationLibrary.h
//! @brief This file contains the internal representation of the library of an
//!        observation module.
//-----------------------------------------------------------------------------

#pragma once

#include <list>
#include <string>
#include <QLibrary>

#include "Interfaces/eventNetworkInterface.h"
#include "Interfaces/observationInterface.h"
#include "Interfaces/callbackInterface.h"

namespace SimulationSlave {

class ObservationModule;

class ObservationLibrary
{
public:
    typedef const std::string& (*ObservationInterface_GetVersion)();
    typedef ObservationInterface* (*ObservationInterface_CreateInstanceType)(StochasticsInterface* stochastics,
            WorldInterface* world,
            EventNetworkInterface* eventNetwork,
            const ParameterInterface* parameters,
            const CallbackInterface* callbacks);
    typedef void (*ObservationInterface_DestroyInstanceType)(ObservationInterface* implementation);
    typedef bool (*ObservationInterface_MasterPreHook)(ObservationInterface* implementation);
    typedef bool (*ObservationInterface_MasterPostHook)(ObservationInterface* implementation,
            const std::string& filename);
    typedef bool (*ObservationInterface_SlavePreHook)(ObservationInterface* implementation,
            const std::string& path);
    typedef bool (*ObservationInterface_SlavePreRunHook)(ObservationInterface* implementation);
    typedef bool (*ObservationInterface_SlaveUpdateHook)(ObservationInterface* implementation,
            int time,
            RunResultInterface& runResult);
    typedef bool (*ObservationInterface_SlavePostRunHook)(ObservationInterface* implementation,
            const RunResultInterface& runResult);
    typedef bool (*ObservationInterface_SlavePostHook)(ObservationInterface* implementation);
    typedef const std::string(*ObservationInterface_SlaveResultFile)(ObservationInterface* implementation);

    ObservationLibrary(const std::string libraryPath,
                       CallbackInterface* callbacks) :
        libraryPath(libraryPath),
        callbacks(callbacks)
    {}
    ObservationLibrary(const ObservationLibrary&) = delete;
    ObservationLibrary(ObservationLibrary&&) = delete;
    ObservationLibrary& operator=(const ObservationLibrary&) = delete;
    ObservationLibrary& operator=(ObservationLibrary&&) = delete;
    virtual ~ObservationLibrary();

    //-----------------------------------------------------------------------------
    //! Inits the observation library by obtaining the function pointers to all
    //! "interface" functions defined via typedef.
    //!
    //! @return     Flag if init was successful
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    //! Releases an observation module instance by calling the destroyInstance method
    //! on the instance and removing the instance from the list of observation modules.
    //!
    //! @return     Flag if release was successful
    //-----------------------------------------------------------------------------
    bool ReleaseObservationModule(ObservationModule* observationModule);

    //-----------------------------------------------------------------------------
    //! Creates an observation module based on the observation instance in the run
    //! config and the observation library stored in the mapping for the library
    //! name of the observation instance.
    //!
    //! @param[in]     observationInstance  Observation instance from the run config
    //! @param[in]     stochastics          The stochastics interface
    //! @param[in]     world                The world interface
    //! @return                             Observation module created from the
    //!                                     observation instance
    //-----------------------------------------------------------------------------
    ObservationModule* CreateObservationModule(ParameterInterface* parameters,
            StochasticsInterface* stochastics,
            WorldInterface* world,
            EventNetworkInterface* eventNetwork);

    bool SlavePreHook(ObservationInterface* implementation,
                      const std::string& path)
    {
        return slavePreHookFunc(implementation, path);
    }


    bool SlavePreRunHook(ObservationInterface* implementation)
    {
        return slavePreRunHookFunc(implementation);
    }

    bool SlaveUpdateHook(ObservationInterface* implementation,
                         int time,
                         RunResultInterface& runResult)
    {
        return slaveUpdateHookFunc(implementation, time, runResult);
    }

    bool SlavePostRunHook(ObservationInterface* implementation,
                          const RunResultInterface& runResult)
    {
        return slavePostRunHookFunc(implementation, runResult);
    }

    bool SlavePostHook(ObservationInterface* implementation)
    {
        return slavePostHookFunc(implementation);
    }

    const std::string SlaveResultFile(ObservationInterface* implementation)
    {
        return slaveResultFileFunc(implementation);
    }

private:
    const std::string DllGetVersionId = "OpenPASS_GetVersion";
    const std::string DllCreateInstanceId = "OpenPASS_CreateInstance";
    const std::string DllDestroyInstanceId = "OpenPASS_DestroyInstance";
    const std::string DllMasterPreHookId = "OpenPASS_MasterPreHook";
    const std::string DllMasterPostHookId = "OpenPASS_MasterPostHook";
    const std::string DllSlavePreHookId = "OpenPASS_SlavePreHook";
    const std::string DllSlavePreRunHookId = "OpenPASS_SlavePreRunHook";
    const std::string DllSlaveUpdateHookId = "OpenPASS_SlaveUpdateHook";
    const std::string DllSlavePostRunHookId = "OpenPASS_SlavePostRunHook";
    const std::string DllSlavePostHookId = "OpenPASS_SlavePostHook";
    const std::string DllSlaveResultFileId = "OpenPASS_SlaveResultFile";

    const std::string libraryPath;
    std::list<ObservationModule*> observationModules;
    QLibrary* library = nullptr;
    CallbackInterface* callbacks;
    ObservationInterface_GetVersion getVersionFunc{nullptr};
    ObservationInterface_CreateInstanceType createInstanceFunc{nullptr};
    ObservationInterface_DestroyInstanceType destroyInstanceFunc{nullptr};
    ObservationInterface_MasterPreHook masterPreHookFunc{nullptr};
    ObservationInterface_MasterPostHook masterPostHookFunc{nullptr};
    ObservationInterface_SlavePreHook slavePreHookFunc{nullptr};
    ObservationInterface_SlavePreRunHook slavePreRunHookFunc{nullptr};
    ObservationInterface_SlaveUpdateHook slaveUpdateHookFunc{nullptr};
    ObservationInterface_SlavePostRunHook slavePostRunHookFunc{nullptr};
    ObservationInterface_SlavePostHook slavePostHookFunc{nullptr};
    ObservationInterface_SlaveResultFile slaveResultFileFunc{nullptr};
};

} // namespace SimulationSlave


