/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <QLibrary>
#include "modelLibrary.h"
#include "component.h"
#include "modelBinding.h"
#include "agent.h"
#include "componentType.h"
#include "component.h"
#include "observationNetwork.h"
#include "observationModule.h"
#include "log.h"

namespace SimulationSlave
{

bool ModelLibrary::Init()
{
#if defined(unix)
    QString path = QString(modelLibraryPath.c_str()) + QString("/lib") + QString(modelLibraryName.c_str());
#elif defined (WIN32)
    QString path = QString(modelLibraryPath.c_str()) + QString("/") + QString(modelLibraryName.c_str());
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

    getVersionFunc = (ModelInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if(!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (ModelInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if(!createInstanceFunc)
    {
        return false;
    }

    destroyInstanceFunc = (ModelInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if(!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "model could not be released";
        return false;
    }

    updateInputFunc = (ModelInterface_UpdateInputType)library->resolve(DllUpdateInputId.c_str());
    if(!updateInputFunc)
    {
        return false;
    }

    updateOutputFunc = (ModelInterface_UpdateOutputType)library->resolve(DllUpdateOutputId.c_str());
    if(!updateOutputFunc)
    {
        return false;
    }

    triggerFunc = (ModelInterface_TriggerType)library->resolve(DllTriggerId.c_str());
    if(!triggerFunc)
    {
        return false;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded model library " << library->fileName().toStdString()
                                        << ", version " << getVersionFunc();

        return true;
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
}

ModelLibrary::~ModelLibrary()
{
    if(!(components.empty()))
    {
        LOG_INTERN(LogLevel::Warning) << "unloading library which is still in use";
    }

    if(library)
    {
        if(library->isLoaded())
        {
            LOG_INTERN(LogLevel::DebugCore) << "unloading library " << modelLibraryName;
            library->unload();
        }

        delete library;
        library = nullptr;
    }
}

bool ModelLibrary::ReleaseComponent(Component *component)
{
    if(!library)
    {
        return false;
    }

    std::list<Component*>::iterator findIter = std::find(components.begin(), components.end(), component);
    if(components.end() == findIter)
    {
        LOG_INTERN(LogLevel::Warning) << "model doesn't belong to library";
        return false;
    }

    try
    {
        destroyInstanceFunc(component->GetImplementation());
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "model could not be released: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "model could not be released";
        return false;
    }

    components.remove(component);

    // do not unload library here since shared_ptrs in other DLLs could still reference deleters within this DLL

    return true;
}

Component *ModelLibrary::CreateComponent(ComponentType *componentType,
                                         int componentId,
                                         StochasticsInterface *stochastics,
                                         WorldInterface *world,
                                         ObservationNetwork *observationNetwork,
                                         Agent *agent)
{
    if(!library)
    {
        return nullptr;
    }

    if(!library->isLoaded() &&!library->load())
    {
        return nullptr;
    }

    std::unique_ptr<Component> component(new (std::nothrow) Component(componentId, agent, world));
    if(!component)
    {
        return nullptr;
    }

    if(!component->SetModelLibrary(this))
    {
        return nullptr;
    }

    // link observations
    for(std::pair<const int, int> &item : componentType->GetObservationLinks())
    {
        int linkId = item.first;
        int observationRef = item.second;

        LOG_INTERN(LogLevel::DebugCore) << "  * link to observation " << observationRef << " (linkId " << linkId << ")";

        // resolve reference
        std::map<int, ObservationModule*> &observationModules = observationNetwork->GetObservationModules();
        ObservationModule* observationModule;
        try
        {
            observationModule = observationModules.at(observationRef);
        }
        catch(const std::out_of_range&)
        {
            LOG_INTERN(LogLevel::Error) << "observation reference configuration not valid";
            return nullptr;
        }

        if(!component->AddObservationLink(observationModule->GetImplementation(), linkId))
        {
            return nullptr;
        }
    }

    ModelInterface *implementation = nullptr;
    try
    {
        implementation = createInstanceFunc(componentId,
                                            componentType->GetInit(),
                                            componentType->GetPriority(),
                                            componentType->GetOffsetTime(),
                                            componentType->GetResponseTime(),
                                            componentType->GetCycleTime(),
                                            stochastics,
                                            world,
                                            &componentType->GetModelParameters(),
                                            &component->GetObservationLinks(),
                                            agent->GetAgentAdapter(),
                                            callbacks);
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not create model instance: " << ex.what();
        return nullptr;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "could not create model instance";
        return nullptr;
    }

    if(!implementation)
    {
        return nullptr;
    }

    component->SetImplementation(implementation);

    Component *componentPtr = component.release();
    components.push_back(componentPtr);

    return componentPtr;
}

} // namespace SimulationSlave
