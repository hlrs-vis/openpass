/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "modelBinding.h"
#include "modelLibrary.h"
#include "callbacks.h"
#include "agent.h"
#include "componentType.h"
#include "observationNetwork.h"
#include "frameworkConfig.h"

namespace SimulationSlave
{

ModelBinding::ModelBinding(const FrameworkConfig *frameworkConfig,
                           SimulationCommon::Callbacks *callbacks):
    frameworkConfig(frameworkConfig),
    callbacks(callbacks)
{}

ModelBinding::~ModelBinding()
{
    Unload();
}

Component *ModelBinding::Instantiate(ComponentType *componentType,
                                     int componentId,
                                     StochasticsInterface *stochastics,
                                     WorldInterface *world,
                                     ObservationNetwork *observationNetwork,
                                     Agent *agent)
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
        modelLibrary = new (std::nothrow) ModelLibrary(frameworkConfig->GetLibraryPath(),
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
                                         componentId,
                                         stochastics,
                                         world,
                                         observationNetwork,
                                         agent);
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
