/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <iostream>
#include <algorithm>
#include <QLibrary>
#include <sstream>
#include "spawnPointLibrary.h"
#include "spawnPointInterface.h"
#include "observationBinding.h"
#include "runConfig.h"
#include "agentFactory.h"
#include "spawnPoint.h"
#include "log.h"

namespace SimulationSlave
{

bool SpawnPointLibrary::Init()
{
#if defined(unix)
    QString path = QString(spawnPointLibraryPath.c_str()) + QString("/lib") + QString(libraryName.c_str());
#elif defined (WIN32)
    QString path = QString(spawnPointLibraryPath.c_str()) + QString("/") + QString(libraryName.c_str());
#else
    error: "undefined target platform"
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

    getVersionFunc = (SpawnPointInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if(!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (SpawnPointInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if(!createInstanceFunc)
    {
        return false;
    }

    destroyInstanceFunc = (SpawnPointInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if(!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "spawn point could not be released";
        return false;
    }

    setSpawnItemFunc = (SpawnPointInterface_SetSpawnItemType)library->resolve(DllSetSpawnItemId.c_str());
    if(!setSpawnItemFunc)
    {
        return false;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded spawn point library " << library->fileName().toStdString()
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

SpawnPointLibrary::~SpawnPointLibrary()
{
    if(!(spawnPoints.empty()))
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

bool SpawnPointLibrary::ReleaseSpawnPoint(SpawnPoint *spawnPoint)
{
    if(!library)
    {
        return false;
    }

    std::list<SpawnPoint*>::iterator findIter = std::find(spawnPoints.begin(), spawnPoints.end(), spawnPoint);
    if(spawnPoints.end() == findIter)
    {
        LOG_INTERN(LogLevel::Warning) << "spawn point doesn't belong to library";
        return false;
    }

    try
    {
        destroyInstanceFunc(spawnPoint->GetImplementation());
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "spawn point could not be released: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "spawn point could not be released";
        return false;
    }

    spawnPoints.remove(spawnPoint);

    return true;
}

SpawnPoint *SpawnPointLibrary::CreateSpawnPoint(SimulationCommon::RunConfig::SpawnPointInstance *spawnPointInstance,
                                                AgentFactory *agentFactory,
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

    SpawnPointInterface *spawnPointInterface = nullptr;
    try
    {
        spawnPointInterface = createInstanceFunc(stochastics,
                                                 world,
                                                 &spawnPointInstance->GetSpawnPointParameters(),
                                                 callbacks);
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not create spawn point instance: " << ex.what();
        return nullptr;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "could not create spawn point instance";
        return nullptr;
    }

    if(!spawnPointInterface)
    {
        return nullptr;
    }

    SpawnPoint *spawnPoint = new (std::nothrow) SpawnPoint(spawnPointInstance,
                                                           agentFactory,
                                                           spawnPointInterface,
                                                           this,
                                                           world);
    if(!spawnPoint)
    {
        return nullptr;
    }

    spawnPoints.push_back(spawnPoint);
    return spawnPoint;
}

} // namespace SimulationSlave
