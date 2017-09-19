/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  observationInterface.h
//! @brief This file contains the interface of the observation modules to
//!        interact with the framework.
//-----------------------------------------------------------------------------

#ifndef OBSERVATIONINTERFACE_H
#define OBSERVATIONINTERFACE_H

#include <string>
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <tuple>
#include "parameterInterface.h"
#include "stochasticsInterface.h"
#include "worldInterface.h"
#include "callbackInterface.h"
#include "vector2d.h"
#include "runResultInterface.h"

//! Interface which has to be provided by observation modules
class ObservationInterface
{
public:
    ObservationInterface(StochasticsInterface *stochastics,
                         WorldInterface *world,
                         const ParameterInterface *parameters,
                         const CallbackInterface *callbacks) :
        stochastics(stochastics),
        world(world),
        parameters(parameters),
        callbacks(callbacks)
    {}
    ObservationInterface(const ObservationInterface &) = delete;
    ObservationInterface(ObservationInterface &&) = delete;
    ObservationInterface &operator=(const ObservationInterface &) = delete;
    ObservationInterface &operator=(ObservationInterface &&) = delete;
    virtual ~ObservationInterface() = default;

    //-----------------------------------------------------------------------------
    //! Called by framework in master before each simulation run starts
    //-----------------------------------------------------------------------------
    virtual void MasterPreHook() = 0;

    //-----------------------------------------------------------------------------
    //! Called by framework in master after each simulation run ends
    //!
    //! @param[in]     filename      Name of file containing the simulation run results from the slave
    //-----------------------------------------------------------------------------
    virtual void MasterPostHook(const std::string &filename) = 0;

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before all simulation runs start
    //!
    //! @param[in]     path          Directory where simulation results will be stored
    //-----------------------------------------------------------------------------
    virtual void SlavePreHook(const std::string &path) = 0;

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before each simulation run starts
    //!
    //! @param[in]     path          Directory where simulation results will be stored
    //-----------------------------------------------------------------------------
    virtual void SlavePreRunHook() = 0;

    //-----------------------------------------------------------------------------
    //! Called by framework in slave at each time step.
    //! Observation module can indicate end of simulation run here.
    //!
    //! @param[in]     time          Current scheduling time
    //! @param[in,out] runResult     Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlaveUpdateHook(int time, RunResultInterface &runResult) = 0;

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after each simulation run ends.
    //! Observation module can observe the current simulation run here.
    //!
    //! @param[in]     runResult     Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlavePostRunHook(const RunResultInterface &runResult) = 0;

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end.
    //-----------------------------------------------------------------------------
    virtual void SlavePostHook() = 0;

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end to transfer the
    //! observation module results to the master.
    //!
    //! @return                      File to be transferred
    //-----------------------------------------------------------------------------
    virtual const std::string SlaveResultFile() = 0;

protected:
    //-----------------------------------------------------------------------------
    //! Retrieves the stochastics functionality
    //!
    //! @return                      Stochastics functionality of framework
    //-----------------------------------------------------------------------------
    StochasticsInterface *GetStochastics() const
    {
        return stochastics;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the world
    //!
    //! @return                      World (scenery and agents)
    //-----------------------------------------------------------------------------
    WorldInterface *GetWorld() const
    {
        return world;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the configuration parameters of the observation module
    //!
    //! @return                      Configuration parameters
    //-----------------------------------------------------------------------------
    const ParameterInterface *GetParameters() const
    {
        return parameters;
    }

    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel      Importance of log
    //! @param[in]     file          Name of file where log is called
    //! @param[in]     line          Line within file where log is called
    //! @param[in]     message       Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if (callbacks) {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    // Access to the following members is provided by the corresponding member
    // functions.
    StochasticsInterface *stochastics;    //!< References stochastics functionality of the framework
    WorldInterface *world;                //!< References the world of the framework
    const ParameterInterface *parameters; //!< References the configuration parameters
    const CallbackInterface *callbacks;   //!< References the callback functions of the framework
};

#endif // OBSERVATIONINTERFACE_H
