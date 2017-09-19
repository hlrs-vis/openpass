/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  stochasticsBinding.h
//! @brief This file contains the interface to the stochastics library.
//-----------------------------------------------------------------------------

#ifndef STOCHASTICSBINDING_H
#define STOCHASTICSBINDING_H

#include "runConfig.h"
#include "callbacks.h"
#include "stochasticsInterface.h"

namespace SimulationSlave
{

class StochasticsLibrary;
class FrameworkConfig;

class StochasticsBinding
{
public:
    StochasticsBinding(const FrameworkConfig *frameworkConfig,
                      SimulationCommon::Callbacks *callbacks);
    StochasticsBinding(const StochasticsBinding&) = delete;
    StochasticsBinding(StochasticsBinding&&) = delete;
    StochasticsBinding& operator=(const StochasticsBinding&) = delete;
    StochasticsBinding& operator=(StochasticsBinding&&) = delete;
    virtual ~StochasticsBinding();

    //-----------------------------------------------------------------------------
    //! Gets the stochastics instance library and stores it,
    //! then creates a new stochasticsInterface of the library.
    //!
    //! @param[in]  stochasticsInstance stochastics instance that is instantiated
    //! @return                         StochasticsInterface created from the library
    //-----------------------------------------------------------------------------
    StochasticsInterface *Instantiate(SimulationCommon::RunConfig::StochasticsInstance *stochasticsInstance);

    //-----------------------------------------------------------------------------
    //! Unloads the stochasticsInterface binding by deleting the library.
    //-----------------------------------------------------------------------------
    void Unload();

private:
    const FrameworkConfig *frameworkConfig;
    StochasticsLibrary *library = nullptr;
    SimulationCommon::Callbacks *callbacks;
};

} // namespace SimulationSlave

#endif // STOCHASTICSBINDING_H
