/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "stochasticsBinding.h"
#include "stochasticsLibrary.h"
#include "stochasticsInterface.h"
#include "frameworkConfig.h"
#include "runConfig.h"

namespace SimulationSlave
{

StochasticsBinding::StochasticsBinding(const FrameworkConfig *frameworkConfig,
                                       SimulationCommon::Callbacks *callbacks) :
      frameworkConfig(frameworkConfig),
      callbacks(callbacks)
{}

StochasticsBinding::~StochasticsBinding()
{
    Unload();
}

StochasticsInterface *StochasticsBinding::Instantiate(SimulationCommon::RunConfig::StochasticsInstance *stochasticsInstance)
{
    if(!library)
    {
        library = new (std::nothrow) StochasticsLibrary(frameworkConfig->GetLibraryPath(),
                                                       stochasticsInstance->GetLibraryName(),
                                                       callbacks);
        if(!library)
        {
            return nullptr;
        }

        if(!library->Init())
        {
            delete library;
            return nullptr;
        }
    }

    return library->CreateStochastics();
}

void StochasticsBinding::Unload()
{
    if(library){
        library->ReleaseStochastics();
        delete library;
        library = nullptr;
    }
}

} // namespace SimulationSlave
