/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
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
#include "eventDetectorLibrary.h"
#include "observationBinding.h"
#include "eventDetector.h"
#include "CoreFramework/CoreShare/log.h"

namespace SimulationSlave
{

bool EventDetectorLibrary::Init()
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

    getVersionFunc = (EventDetectorInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if(!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (EventDetectorInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if(!createInstanceFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not instantiate class from DLL";
        return false;
    }

    destroyInstanceFunc = (EventDetectorInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if(!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "event detector could not be released";
        return false;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded event detector library " << library->fileName().toStdString()
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

EventDetectorLibrary::~EventDetectorLibrary()
{
    if(!(eventDetectors.empty()))
    {
        LOG_INTERN(LogLevel::Warning) << "unloading library which is still in use";
    }

    if(library)
    {
        if(library->isLoaded())
        {
            LOG_INTERN(LogLevel::DebugCore) << "unloading event detector library ";
            library->unload();
        }

        delete library;
        library = nullptr;
    }
}

bool EventDetectorLibrary::ReleaseEventDetector(EventDetector *eventDetector)
{
    if(!library)
    {
        return false;
    }

    std::list<EventDetector*>::iterator findIter = std::find(eventDetectors.begin(), eventDetectors.end(), eventDetector);
    if(eventDetectors.end() == findIter)
    {
        LOG_INTERN(LogLevel::Warning) << "event detector doesn't belong to library";
        return false;
    }

    try
    {
        destroyInstanceFunc(eventDetector->GetImplementation());
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "event detector could not be released: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "event detector could not be released";
        return false;
    }

    eventDetectors.remove(eventDetector);

    return true;
}

EventDetector *EventDetectorLibrary::CreateEventDetector(std::string eventDetectorType,
                                                         ParameterInterface* parameters,
                                                         EventNetworkInterface* eventNetwork,
                                                         WorldInterface *world,
                                                         StochasticsInterface *stochastics)
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

    auto eventDetectorInterface = createInstanceFunc(world,
                                                     parameters,
                                                     eventDetectorType,
                                                     eventNetwork,
                                                     callbacks,
                                                     stochastics);

    if(!eventDetectorInterface)
    {
        throw std::runtime_error("Could not create EventDetector: " + eventDetectorType);
    }

    EventDetector *eventDetector = new EventDetector(eventDetectorInterface,
                                                     this);

    eventDetectors.push_back(eventDetector);
    return eventDetector;
}

} // namespace SimulationSlave
