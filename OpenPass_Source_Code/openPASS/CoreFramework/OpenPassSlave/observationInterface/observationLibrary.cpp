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

#include <iostream>
#include <algorithm>
#include <QLibrary>
#include <sstream>

#include "CoreFramework/CoreShare/log.h"
#include "observationBinding.h"
#include "Interfaces/observationInterface.h"
#include "observationLibrary.h"
#include "observationModule.h"

namespace SimulationSlave {

bool ObservationLibrary::Init()
{
    library = new (std::nothrow) QLibrary(QString::fromStdString(libraryPath));
    if (!library)
    {
        return false;
    }

    if (!library->load())
    {
        LOG_INTERN(LogLevel::Error) << library->errorString().toStdString();
        delete library;
        library = nullptr;
        return false;
    }

    getVersionFunc = (ObservationInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if (!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (ObservationInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if (!createInstanceFunc)
    {
        return false;
    }

    destroyInstanceFunc = (ObservationInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if (!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "observation module could not be released";
        return false;
    }

    masterPreHookFunc = (ObservationInterface_MasterPreHook)library->resolve(DllMasterPreHookId.c_str());
    if (!masterPreHookFunc)
    {
        return false;
    }

    masterPostHookFunc = (ObservationInterface_MasterPostHook)library->resolve(DllMasterPostHookId.c_str());
    if (!masterPostHookFunc)
    {
        return false;
    }

    slavePreHookFunc = (ObservationInterface_SlavePreHook)library->resolve(DllSlavePreHookId.c_str());
    if (!slavePreHookFunc)
    {
        return false;
    }

    slavePreRunHookFunc = (ObservationInterface_SlavePreRunHook)library->resolve(DllSlavePreRunHookId.c_str());
    if (!slavePreRunHookFunc)
    {
        return false;
    }

    slaveUpdateHookFunc = (ObservationInterface_SlaveUpdateHook)library->resolve(DllSlaveUpdateHookId.c_str());
    if (!slavePreRunHookFunc)
    {
        return false;
    }

    slavePostRunHookFunc = (ObservationInterface_SlavePostRunHook)library->resolve(DllSlavePostRunHookId.c_str());
    if (!slavePostRunHookFunc)
    {
        return false;
    }

    slavePostHookFunc = (ObservationInterface_SlavePostHook)library->resolve(DllSlavePostHookId.c_str());
    if (!slavePostHookFunc)
    {
        return false;
    }

    slaveResultFileFunc = (ObservationInterface_SlaveResultFile)library->resolve(DllSlaveResultFileId.c_str());
    if (!slaveResultFileFunc)
    {
        return false;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded observation library " << library->fileName().toStdString()
                                        << ", version " << getVersionFunc();
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL: " << ex.what();
        return false;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    return true;
}

ObservationLibrary::~ObservationLibrary()
{
    if (!(observationModules.empty()))
    {
        LOG_INTERN(LogLevel::Warning) << "unloading library which is still in use";
    }

    if (library)
    {
        if (library->isLoaded())
        {
            LOG_INTERN(LogLevel::DebugCore) << "unloading observation library";
            library->unload();
        }

        delete library;
        library = nullptr;
    }
}

bool ObservationLibrary::ReleaseObservationModule(ObservationModule* observationModule)
{
    if (!library)
    {
        return false;
    }

    std::list<ObservationModule*>::iterator findIter = std::find(observationModules.begin(), observationModules.end(),
            observationModule);
    if (observationModules.end() == findIter)
    {
        LOG_INTERN(LogLevel::Warning) << "observation module doesn't belong to library";
        return false;
    }

    try
    {
        destroyInstanceFunc(observationModule->GetImplementation());
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "observation could not be released: " << ex.what();
        return false;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "observation could not be released";
        return false;
    }

    observationModules.remove(observationModule);

    return true;
}

ObservationModule* ObservationLibrary::CreateObservationModule(ParameterInterface* parameters,
        StochasticsInterface* stochastics,
        WorldInterface* world,
        EventNetworkInterface* eventNetwork)
{
    if (!library)
    {
        return nullptr;
    }

    if (!library->isLoaded())
    {
        if (!library->load())
        {
            return nullptr;
        }
    }

    ObservationInterface* observationInterface = nullptr;
    try
    {
        observationInterface = createInstanceFunc(stochastics,
                               world,
                               eventNetwork,
                               parameters,
                               callbacks);
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not create observation instance: " << ex.what();
        return nullptr;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "could not create observation instance";
        return nullptr;
    }

    if (!observationInterface)
    {
        return nullptr;
    }

    ObservationModule* observationModule = new (std::nothrow) ObservationModule(observationInterface, this);
    if (!observationModule)
    {
        return nullptr;
    }

    observationModules.push_back(observationModule);
    return observationModule;
}

} // namespace SimulationSlave
