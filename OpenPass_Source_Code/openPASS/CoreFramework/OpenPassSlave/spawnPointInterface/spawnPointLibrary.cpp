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

#include "agentFactory.h"
#include "CoreFramework/CoreShare/log.h"
#include "observationBinding.h"
#include "spawnPoint.h"
#include "spawnPointLibrary.h"

namespace SimulationSlave
{

bool SpawnPointLibrary::Init()
{
    library = new (std::nothrow) QLibrary(QString::fromStdString(libraryPath));
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

    generateAgentFunc = (SpawnPointInterface_GenerateAgentType)library->resolve(DllGenerateAgentId.c_str());
    if(!generateAgentFunc)
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
            LOG_INTERN(LogLevel::DebugCore) << "unloading library " << libraryPath;
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

SpawnPoint *SpawnPointLibrary::CreateSpawnPoint(ParameterInterface *parameters,
                                                AgentFactoryInterface *agentFactory,
                                                WorldInterface *world,
                                                AgentBlueprintProviderInterface* agentBlueprintProvider,
                                                SamplerInterface *sampler,
                                                ScenarioInterface *scenario)
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
        spawnPointInterface = createInstanceFunc(world,
                                                 parameters,
                                                 callbacks,
                                                 agentBlueprintProvider,
                                                 sampler,
                                                 scenario);
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

    SpawnPoint *spawnPoint = new (std::nothrow) SpawnPoint(agentFactory,
                                                           spawnPointInterface,
                                                           this);
    if(!spawnPoint)
    {
        return nullptr;
    }

    spawnPoints.push_back(spawnPoint);
    return spawnPoint;
}

} // namespace SimulationSlave
