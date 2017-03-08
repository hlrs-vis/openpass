/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  observationNetwork.h
//! @brief This file implements the container of all observation modules.
//-----------------------------------------------------------------------------

#ifndef OBSERVATIONNETWORK_H
#define OBSERVATIONNETWORK_H

#include <list>
#include <map>
#include "runConfig.h"
#include "log.h"
#include "worldInterface.h"

namespace SimulationSlave
{

class ObservationBinding;
class ObservationModule;
class Stochastics;
class RunResult;

class ObservationNetwork
{
public:
    ObservationNetwork(ObservationBinding *binding) :
        binding(binding)
    {}
    ObservationNetwork(const ObservationNetwork&) = delete;
    ObservationNetwork(ObservationNetwork&&) = delete;
    ObservationNetwork& operator=(const ObservationNetwork&) = delete;
    ObservationNetwork& operator=(ObservationNetwork&&) = delete;
    virtual ~ObservationNetwork();

    //-----------------------------------------------------------------------------
    //! Instantiates observation module objects using the provided mapping and the
    //! stored observation binding by filling the observation network modules map
    //! with these objects and the ids of their observation instances priovided in
    //! the given map.
    //!
    //! @param[in]  observationInstances    Mapping from IDs to observation instances
    //!                                     that will be instantiated
    //! @param[in]  stochastics             The stochastics object
    //! @param[in]  world                   The world object
    //! @return                             Flag if the instantiation was successful
    //-----------------------------------------------------------------------------
    bool Instantiate(const std::map<int, SimulationCommon::RunConfig::ObservationInstance*> &observationInstances,
                     Stochastics *stochastics,
                     WorldInterface *world);

    //-----------------------------------------------------------------------------
    //! Returns the observation module mapping.
    //!
    //! @return                             Observation module mapping
    //-----------------------------------------------------------------------------
    std::map<int, ObservationModule*> &GetObservationModules();

    //-----------------------------------------------------------------------------
    //! Inits the observation network by calling the slavePreHook function pointer
    //! of the observation library with each observation module implementation as
    //! parameter.
    //!
    //! @return                             Flag if the init was successful
    //-----------------------------------------------------------------------------
    bool InitAll(const std::string &path);

    //-----------------------------------------------------------------------------
    //! Inits the network run by calling the slavePreRunHook function
    //! pointer of the observation library with each observation module instance.
    //!
    //! @return                             Flag if the run init was successful
    //-----------------------------------------------------------------------------
    bool InitRun();

    //-----------------------------------------------------------------------------
    //! Updates the time step by calling the SlaveUpdateHook function pointer of the
    //! observation library with each observation module instance and the provided
    //! parameters.
    //!
    //! @param[in]  time                    Current time
    //! @param[out] runResult               The results of the run
    //! @return                             Flag if the update was successful
    //-----------------------------------------------------------------------------
    bool UpdateTimeStep(int time, RunResult &runResult);

    //-----------------------------------------------------------------------------
    //! Finalizes the run by calling the SlaveUpdateHook function pointer of the
    //! observation library with each observation module instance and teh provided
    //! result parameter.
    //!
    //! @param[in] runResult                The results of the run
    //! @return                             Flag if the finalize was successful
    //-----------------------------------------------------------------------------
    bool FinalizeRun(const RunResult &result);

    //-----------------------------------------------------------------------------
    //! Finalizes the observation network by calling the masterPostHook function
    //! pointer of the observation library with each observation module instance
    //! (and its ID) in each network slave.
    //!
    //! @return                             Flag if the finalize was successful
    //-----------------------------------------------------------------------------
    bool FinalizeAll();

    //-----------------------------------------------------------------------------
    //! Clears the modules mapping and deletes the observation module instances
    //! within and also unloads the observation bindings.
    //-----------------------------------------------------------------------------
    void Clear();

private:    
    ObservationBinding *binding;
    std::map<int, ObservationModule*> modules;
};

} // namespace SimulationSlave

#endif // OBSERVATIONNETWORK_H
