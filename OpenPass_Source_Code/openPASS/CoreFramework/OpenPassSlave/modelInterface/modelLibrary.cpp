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
#include <sstream>
#include <stdexcept>
#include <QLibrary>

#include "agent.h"
#include "component.h"
#include "componentType.h"
#include "CoreFramework/CoreShare/log.h"
#include "modelBinding.h"
#include "modelLibrary.h"
#include "Interfaces/observationNetworkInterface.h"
#include "observationModule.h"

namespace SimulationSlave {

bool ModelLibrary::Init()
{
    #if defined(unix)
    QString path = QString(modelLibraryPath.c_str()) + QString("/lib") + QString(modelLibraryName.c_str());
    #elif defined (WIN32)
    QString path = QString(modelLibraryPath.c_str()) + QString("/") + QString(modelLibraryName.c_str());
    #else
#error undefined target platform
    #endif

    library = new (std::nothrow) QLibrary(path);
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

    getVersionFunc = (ModelInterface_GetVersion)library->resolve(DllGetVersionId.c_str());
    if (!getVersionFunc)
    {
        LOG_INTERN(LogLevel::Error) << "could not retrieve version information from DLL";
        return false;
    }

    createInstanceFunc = (ModelInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if (!createInstanceFunc)
    {
        return false;
    }

    createEventInstanceFunc = (UnrestrictedEventModelInterface_CreateInstanceType)library->resolve(DllCreateInstanceId.c_str());
    if(!createEventInstanceFunc)
    {
        return false;
    }

    destroyInstanceFunc = (ModelInterface_DestroyInstanceType)library->resolve(DllDestroyInstanceId.c_str());
    if (!destroyInstanceFunc)
    {
        LOG_INTERN(LogLevel::Warning) << "model could not be released";
        return false;
    }

    updateInputFunc = (ModelInterface_UpdateInputType)library->resolve(DllUpdateInputId.c_str());
    if (!updateInputFunc)
    {
        return false;
    }

    updateOutputFunc = (ModelInterface_UpdateOutputType)library->resolve(DllUpdateOutputId.c_str());
    if (!updateOutputFunc)
    {
        return false;
    }

    triggerFunc = (ModelInterface_TriggerType)library->resolve(DllTriggerId.c_str());
    if (!triggerFunc)
    {
        return false;
    }

    try
    {
        LOG_INTERN(LogLevel::DebugCore) << "loaded model library " << library->fileName().toStdString()
                                        << ", version " << getVersionFunc();

        return true;
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
}

ModelLibrary::~ModelLibrary()
{
    if (!(components.empty()))
    {
        LOG_INTERN(LogLevel::Warning) << "unloading library which is still in use";
    }

    if (library)
    {
        if (library->isLoaded())
        {
            LOG_INTERN(LogLevel::DebugCore) << "unloading library " << modelLibraryName;
            library->unload();
        }

        delete library;
        library = nullptr;
    }
}

bool ModelLibrary::ReleaseComponent(ComponentInterface* component)
{
    if (!library)
    {
        return false;
    }

    std::list<ComponentInterface*>::iterator findIter = std::find(components.begin(), components.end(), component);
    if (components.end() == findIter)
    {
        LOG_INTERN(LogLevel::Warning) << "model doesn't belong to library";
        return false;
    }

    try
    {
        destroyInstanceFunc(component->GetImplementation());
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "model could not be released: " << ex.what();
        return false;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "model could not be released";
        return false;
    }

    components.remove(component);

    // do not unload library here since shared_ptrs in other DLLs could still reference deleters within this DLL

    return true;
}

ComponentInterface* ModelLibrary::CreateComponent(std::shared_ptr<ComponentType> componentType,
        std::string componentName,
        StochasticsInterface* stochastics,
        WorldInterface* world,
        ObservationNetworkInterface* observationNetwork,
        Agent* agent,
        EventNetworkInterface* eventNetwork)
{
    if (!library)
    {
        return nullptr;
    }

    if (!library->isLoaded() && !library->load())
    {
        return nullptr;
    }

    std::unique_ptr<ComponentInterface> component(new (std::nothrow) Component(componentName, agent));
    if (!component)
    {
        return nullptr;
    }

    if (!component->SetModelLibrary(this))
    {
        return nullptr;
    }

    // TODO: This is a workaround, as the OSI use case only imports a single observation library -> implement new observation concept
    component->SetObservations(observationNetwork->GetObservationModules());

    ModelInterface* implementation = nullptr;

    try
    {
        if (modelLibraryName == "ComponentController")
        {
            implementation = createEventInstanceFunc(componentName,
                                                     componentType->GetInit(),
                                                     componentType->GetPriority(),
                                                     componentType->GetOffsetTime(),
                                                     componentType->GetResponseTime(),
                                                     componentType->GetCycleTime(),
                                                     stochastics,
                                                     world,
                                                     componentType->GetModelParameters(),
                                                     &component->GetObservations(),
                                                     agent->GetAgentAdapter(),
                                                     callbacks,
                                                     eventNetwork);
        }
        else
        {
            implementation = createInstanceFunc(componentName,
                                                componentType->GetInit(),
                                                componentType->GetPriority(),
                                                componentType->GetOffsetTime(),
                                                componentType->GetResponseTime(),
                                                componentType->GetCycleTime(),
                                                stochastics,
                                                world,
                                                componentType->GetModelParameters(),
                                                &component->GetObservations(),
                                                agent->GetAgentAdapter(),
                                                callbacks);
        }
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "could not create model instance: " << ex.what();
        return nullptr;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "could not create model instance";
        return nullptr;
    }

    if (!implementation)
    {
        return nullptr;
    }

    component->SetImplementation(implementation);

    ComponentInterface* componentPtr = component.release();
    components.push_back(componentPtr);

    return componentPtr;
}

} // namespace SimulationSlave
