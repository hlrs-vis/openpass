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

#include "agent.h"
#include "CoreFramework/CoreShare/callbacks.h"
#include "componentType.h"
#include "modelBinding.h"
#include "modelLibrary.h"
#include "Interfaces/observationNetworkInterface.h"

namespace SimulationSlave
{

ModelBinding::ModelBinding(const std::string libraryPath,
                           CallbackInterface *callbacks):
    libraryPath(libraryPath),
    callbacks(callbacks)
{}

ModelBinding::~ModelBinding()
{
    Unload();
}

ComponentInterface *ModelBinding::Instantiate(std::shared_ptr<ComponentType>componentType,
                                     std::string componentName,
                                     StochasticsInterface *stochastics,
                                     WorldInterface *world,
                                     ObservationNetworkInterface *observationNetwork,
                                     Agent *agent,
                                     EventNetworkInterface *eventNetwork)
{
    const std::string name = componentType->GetModelLibrary();

    ModelLibrary *modelLibrary;
    try
    {
        modelLibrary = modelLibraries.at(name);
    }
    catch(const std::out_of_range&)
    {
        modelLibrary = nullptr;
    }

    if(!modelLibrary)
    {
        modelLibrary = new (std::nothrow) ModelLibrary(libraryPath,
                                                       name,
                                                       callbacks);
        if(!modelLibrary)
        {
            return nullptr;
        }

        if(!modelLibrary->Init())
        {
            delete modelLibrary;
            return nullptr;
        }

        if(!modelLibraries.insert({name, modelLibrary}).second)
        {
            delete modelLibrary;
            return nullptr;
        }
    }

    return modelLibrary->CreateComponent(componentType,
                                         componentName,
                                         stochastics,
                                         world,
                                         observationNetwork,
                                         agent,
                                         eventNetwork);
}

void ModelBinding::Unload()
{
    for(std::pair<const std::string, ModelLibrary*> &item : modelLibraries)
    {
        delete item.second;
    }

    modelLibraries.clear();
}

} // namespace SimulationSlave
