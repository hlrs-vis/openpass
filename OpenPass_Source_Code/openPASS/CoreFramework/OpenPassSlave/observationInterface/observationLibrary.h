/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  observationLibrary.h
//! @brief This file contains the internal representation of the library of an
//!        observation module.
//-----------------------------------------------------------------------------

#ifndef OBSERVATIONLIBRARY_H
#define OBSERVATIONLIBRARY_H

#include <list>
#include <string>
#include <QLibrary>
#include "runConfig.h"
#include "observationInterface.h"
#include "callbackInterface.h"

namespace SimulationSlave
{

class ObservationModule;

class ObservationLibrary
{
public:    
    typedef const std::string &(*ObservationInterface_GetVersion)();
    typedef ObservationInterface *(*ObservationInterface_CreateInstanceType)(StochasticsInterface *stochastics,
                                                                             WorldInterface *world,
                                                                             const ParameterInterface *parameters,
                                                                             const CallbackInterface *callbacks);
    typedef void (*ObservationInterface_DestroyInstanceType)(ObservationInterface *implementation);
    typedef bool (*ObservationInterface_MasterPreHook)(ObservationInterface *implementation);
    typedef bool (*ObservationInterface_MasterPostHook)(ObservationInterface *implementation,
                                                        const std::string &filename);
    typedef bool (*ObservationInterface_SlavePreHook)(ObservationInterface *implementation,
                                                      const std::string &path);
    typedef bool (*ObservationInterface_SlavePreRunHook)(ObservationInterface *implementation);
    typedef bool (*ObservationInterface_SlaveUpdateHook)(ObservationInterface *implementation,
                                                         int time,
                                                         RunResultInterface &runResult);
    typedef bool (*ObservationInterface_SlavePostRunHook)(ObservationInterface *implementation,
                                                          const RunResultInterface &runResult);
    typedef bool (*ObservationInterface_SlavePostHook)(ObservationInterface *implementation);
    typedef const std::string (*ObservationInterface_SlaveResultFile)(ObservationInterface *implementation);

    ObservationLibrary(const std::string &observationLibraryPath,
                       const std::string &libraryName,
                       CallbackInterface *callbacks) :
        observationLibraryPath(observationLibraryPath),
        libraryName(libraryName),
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
    bool ReleaseObservationModule(ObservationModule *observationModule);

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
    ObservationModule *CreateObservationModule(SimulationCommon::RunConfig::ObservationInstance *observationInstance,
                                               StochasticsInterface *stochastics,
                                               WorldInterface *world);

    bool MasterPreHook(ObservationInterface *implementation)
    {
        return masterPreHookFunc(implementation);
    }

    bool MasterPostHook(ObservationInterface *implementation,
                        const std::string &filename)
    {
        return masterPostHookFunc(implementation, filename);
    }

    bool SlavePreHook(ObservationInterface *implementation,
                      const std::string &path)
    {
        return slavePreHookFunc(implementation, path);
    }

    bool SlavePreRunHook(ObservationInterface *implementation)
    {
        return slavePreRunHookFunc(implementation);
    }

    bool SlaveUpdateHook(ObservationInterface *implementation,
                         int time,
                         RunResultInterface &runResult)
    {
        return slaveUpdateHookFunc(implementation, time, runResult);
    }

    bool SlavePostRunHook(ObservationInterface *implementation,
                          const RunResultInterface &runResult)
    {
        return slavePostRunHookFunc(implementation, runResult);
    }

    bool SlavePostHook(ObservationInterface *implementation)
    {
        return slavePostHookFunc(implementation);
    }

    const std::string SlaveResultFile(ObservationInterface *implementation)
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

    std::string observationLibraryPath;
    std::string libraryName;
    std::list<ObservationModule*> observationModules;
    QLibrary *library = nullptr;
    CallbackInterface *callbacks;
    ObservationInterface_GetVersion getVersionFunc;
    ObservationInterface_CreateInstanceType createInstanceFunc;
    ObservationInterface_DestroyInstanceType destroyInstanceFunc;
    ObservationInterface_MasterPreHook masterPreHookFunc;
    ObservationInterface_MasterPostHook masterPostHookFunc;
    ObservationInterface_SlavePreHook slavePreHookFunc;
    ObservationInterface_SlavePreRunHook slavePreRunHookFunc;
    ObservationInterface_SlaveUpdateHook slaveUpdateHookFunc;
    ObservationInterface_SlavePostRunHook slavePostRunHookFunc;
    ObservationInterface_SlavePostHook slavePostHookFunc;
    ObservationInterface_SlaveResultFile slaveResultFileFunc;
};

} // namespace SimulationSlave

#endif // OBSERVATIONLIBRARY_H
