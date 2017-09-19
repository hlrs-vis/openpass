/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <QLibrary>
#include "stochasticsLibrary.h"
#include "log.h"

namespace SimulationSlave
{

bool StochasticsLibrary::Init()
{
#if defined(unix)
    QString path = QString(stochasticsLibraryPath.c_str()) + QString("/lib") + QString(libraryName.c_str());
#elif defined (WIN32)
    QString path = QString(stochasticsLibraryPath.c_str()) + QString("/") + QString(libraryName.c_str());
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

    getVersionFunc = (StochasticsInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if(!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (StochasticsInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if(!createInstanceFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not create instance from DLL";
        return false;
    }

    destroyInstanceFunc = (StochasticsInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if(!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "stochastics could not be released";
        return false;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded stochastics library " << library->fileName().toStdString()
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

StochasticsLibrary::~StochasticsLibrary()
{
    if(stochasticsInterface)
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

bool StochasticsLibrary::ReleaseStochastics()
{
    if(!stochasticsInterface){
        return true;
    }

    if(!library)
    {
        return false;
    }

    try
    {
        destroyInstanceFunc(stochasticsInterface);
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "stochastics could not be released: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "stochastics could not be released";
        return false;
    }

    stochasticsInterface = nullptr;

    return true;
}

StochasticsInterface *StochasticsLibrary::CreateStochastics()
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

    stochasticsInterface = nullptr;
    try
    {
        stochasticsInterface = createInstanceFunc(callbacks);
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not create stochastics instance: " << ex.what();
        return nullptr;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "could not create stochastics instance";
        return nullptr;
    }

    if(!stochasticsInterface)
    {
        return nullptr;
    }


    return stochasticsInterface;
}

} // namespace SimulationSlave
