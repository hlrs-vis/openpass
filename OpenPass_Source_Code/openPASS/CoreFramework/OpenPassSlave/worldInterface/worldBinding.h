/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  worldBinding.h
//! @brief This file contains the interface to the world library.
//-----------------------------------------------------------------------------

#ifndef WORLDBINDING_H
#define WORLDBINDING_H

#include "runConfig.h"
#include "callbacks.h"
#include "worldInterface.h"

namespace SimulationSlave
{

class WorldLibrary;
class FrameworkConfig;

class WorldBinding
{
public:
    WorldBinding(const FrameworkConfig *frameworkConfig,
                      SimulationCommon::Callbacks *callbacks);
    WorldBinding(const WorldBinding&) = delete;
    WorldBinding(WorldBinding&&) = delete;
    WorldBinding& operator=(const WorldBinding&) = delete;
    WorldBinding& operator=(WorldBinding&&) = delete;
    virtual ~WorldBinding();

    //-----------------------------------------------------------------------------
    //! Gets the world instance library and stores it,
    //! then creates a new worldInterface of the library.
    //!
    //! @param[in]  worldParameter      world parameter that is instantiated
    //! @return                         worldInterface created from the library
    //-----------------------------------------------------------------------------
    WorldInterface *Instantiate(WorldParameter *worldParameter);

    //-----------------------------------------------------------------------------
    //! Unloads the stochasticsInterface binding by deleting the library.
    //-----------------------------------------------------------------------------
    void Unload();

private:
    const FrameworkConfig *frameworkConfig;
    WorldLibrary *library = nullptr;
    SimulationCommon::Callbacks *callbacks;
};

} // namespace SimulationSlave

#endif // WORLDBINDING_H
