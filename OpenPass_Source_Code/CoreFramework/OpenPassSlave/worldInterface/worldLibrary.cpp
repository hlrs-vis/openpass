/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "worldLibrary.h"
#include "log.h"

namespace SimulationSlave
{

bool WorldLibrary::Init()
{
#ifndef WIN32
    QString path = QString(worldLibraryPath.c_str()) + QString("/lib") + QString(libraryName.c_str());
#else
    QString path = QString(worldLibraryPath.c_str()) + QString("/") + QString(libraryName.c_str());
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

    getVersionFunc = (WorldInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if(!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (WorldInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if(!createInstanceFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not create instance from DLL";
        return nullptr;
    }

    destroyInstanceFunc = (WorldInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if(!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "world could not be released";
        return false;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded world library " << library->fileName().toStdString()
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

WorldLibrary::~WorldLibrary()
{
    if(worldInterface)
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

bool WorldLibrary::ReleaseWorld()
{
    if(!worldInterface)
    {
        return true;
    }

    if(!library)
    {
        return false;
    }

    try
    {
        destroyInstanceFunc(worldInterface);
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "world could not be released: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "world could not be released";
        return false;
    }

    worldInterface = nullptr;

    return true;
}

WorldInterface *WorldLibrary::CreateWorld()
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

    worldInterface = nullptr;
    try
    {
        worldInterface = createInstanceFunc(callbacks);
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

    if(!worldInterface)
    {
        return nullptr;
    }


    return worldInterface;
}

} // namespace SimulationSlave


