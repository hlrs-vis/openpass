/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <iostream>
#include <algorithm>
#include <QLibrary>
#include <sstream>
#include "observationLibrary.h"
#include "observationInterface.h"
#include "observationBinding.h"
#include "runConfig.h"
#include "observationModule.h"
#include "log.h"

namespace SimulationSlave
{

bool ObservationLibrary::Init()
{
#if defined(unix)
    QString path = QString(observationLibraryPath.c_str()) + QString("/lib") + QString(libraryName.c_str());
#elif defined (WIN32)
    QString path = QString(observationLibraryPath.c_str()) + QString("/") + QString(libraryName.c_str());
#else
    error: "undefined target platform"
#endif
#ifdef _DEBUG
		path += "d";
#endif

    library = new (std::nothrow) QLibrary(path);
    if(!library)
    {
        return false;
    }

    if(!library->load())
    {
        LOG_INTERN(LogLevel::Error) << library->errorString().toStdString();
        delete library;
        library = nullptr;
        return false;
    }

    getVersionFunc = (ObservationInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if(!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (ObservationInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if(!createInstanceFunc)
    {
        return nullptr;
    }

    destroyInstanceFunc = (ObservationInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if(!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "observation module could not be released";
        return false;
    }

    masterPreHookFunc = (ObservationInterface_MasterPreHook)library->resolve(DllMasterPreHookId.c_str());
    if(!masterPreHookFunc)
    {
        return nullptr;
    }

    masterPostHookFunc = (ObservationInterface_MasterPostHook)library->resolve(DllMasterPostHookId.c_str());
    if(!masterPostHookFunc)
    {
        return nullptr;
    }

    slavePreHookFunc = (ObservationInterface_SlavePreHook)library->resolve(DllSlavePreHookId.c_str());
    if(!slavePreHookFunc)
    {
        return nullptr;
    }

    slavePreRunHookFunc = (ObservationInterface_SlavePreRunHook)library->resolve(DllSlavePreRunHookId.c_str());
    if(!slavePreRunHookFunc)
    {
        return nullptr;
    }

    slaveUpdateHookFunc = (ObservationInterface_SlaveUpdateHook)library->resolve(DllSlaveUpdateHookId.c_str());
    if(!slaveUpdateHookFunc)
    {
        return nullptr;
    }

    slavePostRunHookFunc = (ObservationInterface_SlavePostRunHook)library->resolve(DllSlavePostRunHookId.c_str());
    if(!slavePostRunHookFunc)
    {
        return nullptr;
    }

    slavePostHookFunc = (ObservationInterface_SlavePostHook)library->resolve(DllSlavePostHookId.c_str());
    if(!slavePostHookFunc)
    {
        return nullptr;
    }

    slaveResultFileFunc = (ObservationInterface_SlaveResultFile)library->resolve(DllSlaveResultFileId.c_str());
    if(!slaveResultFileFunc)
    {
        return nullptr;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded observation library " << library->fileName().toStdString()
                                        << ", version " << getVersionFunc();
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    return true;
}

ObservationLibrary::~ObservationLibrary()
{
    if(!(observationModules.empty()))
    {
        LOG_INTERN(LogLevel::Warning) << "unloading library which is still in use";
    }

    if(library)
    {
        if(library->isLoaded())
        {
            LOG_INTERN(LogLevel::DebugCore) << "unloading library " << libraryName;
            library->unload();
        }

        delete library;
        library = nullptr;
    }
}

bool ObservationLibrary::ReleaseObservationModule(ObservationModule *observationModule)
{
    if(!library)
    {
        return false;
    }

    std::list<ObservationModule*>::iterator findIter = std::find(observationModules.begin(), observationModules.end(), observationModule);
    if(observationModules.end() == findIter)
    {
        LOG_INTERN(LogLevel::Warning) << "observation module doesn't belong to library";
        return false;
    }

    try
    {
        destroyInstanceFunc(observationModule->GetImplementation());
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "observation could not be released: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "observation could not be released";
        return false;
    }

    observationModules.remove(observationModule);

    return true;
}

ObservationModule *ObservationLibrary::CreateObservationModule(SimulationCommon::RunConfig::ObservationInstance *observationInstance,
                                                               StochasticsInterface *stochastics,
                                                               WorldInterface *world)
{
    if(!library)
    {
        return nullptr;
    }

    if(!library->isLoaded())
    {
        if(!library->load())
        {
            return nullptr;
        }
    }

    ObservationInterface *observationInterface = nullptr;
    try
    {
        observationInterface = createInstanceFunc(stochastics,
                                                  world,
                                                  &observationInstance->GetObservationParameters(),
                                                  callbacks);
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not create observation instance: " << ex.what();
        return nullptr;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "could not create observation instance";
        return nullptr;
    }

    if(!observationInterface)
    {
        return nullptr;
    }

    ObservationModule *observationModule = new (std::nothrow) ObservationModule(observationInstance, observationInterface, this);
    if(!observationModule)
    {
        return nullptr;
    }

    observationModules.push_back(observationModule);
    return observationModule;
}

} // namespace SimulationSlave
