/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/


#ifndef OBSERVATION_SCOPELOGGER_IMPLEMENTATION_H
#define OBSERVATION_SCOPELOGGER_IMPLEMENTATION_H

#include <string>
#include <tuple>
#include <QFile>
#include <QTextStream>
#include "agentInterface.h"
#include "observationInterface.h"
#include "componentPorts.h"
#include "XoscWriter/openScenarioWriter.h"

/**
 * \addtogroup CoreModules_PCM openPASS CoreModules pcm
 * @{
 * \addtogroup Observation_ScopeLogger
 *
 * \brief Observation module to log the agent's scope
 *
 * \details This module stores several information provided by input signals in a .csv file
 * (position, velocity, driver wishes etc.).
 *
 * @}
 */

/*!
 * \copydoc Observation_ScopeLogger
 * \ingroup Observation_ScopeLogger
 */
class Observation_ScopeLogger_Implementation : public ObservationInterface
{
public:
    const std::string COMPONENTNAME = "Observation_ScopeLogger";

    Observation_ScopeLogger_Implementation(StochasticsInterface *stochastics,
                                           WorldInterface *world,
                                           const ParameterInterface *parameters,
                                           const CallbackInterface *callbacks);
    Observation_ScopeLogger_Implementation(const Observation_ScopeLogger_Implementation &) = delete;
    Observation_ScopeLogger_Implementation(Observation_ScopeLogger_Implementation &&) = delete;
    Observation_ScopeLogger_Implementation &operator=(const Observation_ScopeLogger_Implementation &) =
        delete;
    Observation_ScopeLogger_Implementation &operator=(Observation_ScopeLogger_Implementation &&) =
        delete;
    virtual ~Observation_ScopeLogger_Implementation() = default;

    //-----------------------------------------------------------------------------
    //! Called by framework in master before each simulation run starts
    //-----------------------------------------------------------------------------
    virtual void MasterPreHook()
    {
        return;   //dummy
    }

    //-----------------------------------------------------------------------------
    //! Called by framework in master after each simulation run ends
    //!
    //! @param[in]     filename      Name of file containing the simulation run results from the slave
    //-----------------------------------------------------------------------------

    virtual void MasterPostHook(const std::string &filename)
    {
        Q_UNUSED(filename)
    } //dummy

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before all simulation runs start
    //!
    //! @param[in]     path          Directory where simulation results will be stored
    //-----------------------------------------------------------------------------

    virtual void SlavePreHook(const std::string &path);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before each simulation run starts.
    //-----------------------------------------------------------------------------
    virtual void SlavePreRunHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave at each time step.
    //! Observation module can indicate end of simulation run here.
    //!
    //! @param[in]     time        Current scheduling time
    //! @param[in,out] runResult   Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlaveUpdateHook(int time, RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after each simulation run ends.
    //! Observation module can observe the current simulation run here.
    //!
    //! @param[in]     runResult   Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlavePostRunHook(const RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end.
    //-----------------------------------------------------------------------------

    virtual void SlavePostHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end to transfer the
    //! observation module results to the master.
    //!
    //! @return                      File to be transferred
    virtual const std::string SlaveResultFile()
    {
        return "";   //dummy
    }

private:

    //-----------------------------------------------------------------------------
    //! Save all Data of one time Step
    //! @param[in]  time             current time step
    //! @param[in]  agent            current agent
    //! @param[out] timeStepData     map to fill with data
    //!
    //! @return
    void SaveTimeStepData(int time, const AgentInterface *agent,
                          std::map<int, std::vector<std::pair<QString, QVariant> > > &timeStepData);

    //-----------------------------------------------------------------------------
    //! Write simulation results to csv
    //!
    //! @return
    void WriteResultCSV();

    //-----------------------------------------------------------------------------
    //! Write simulation results to openScenario File
    //!
    //! @return
    void WriteResultXosc();

    //! A map of all agents and their positions at every time.
    std::map<int, std::map<int, std::pair<double, double>>> agentsPositions;

    std::map<int, std::map<int, std::vector<std::pair<QString, QVariant>>>> agentData;
    //! time vector
    std::vector<int> timeVector;
    //! full path name of result file
    std::string resultFolder;
};

#endif // OBSERVATION_SCOPELOGGER_IMPLEMENTATION_H
