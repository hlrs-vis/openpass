/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#ifndef OBSERVATION_TTC_IMPLEMENTATION_H
#define OBSERVATION_TTC_IMPLEMENTATION_H

#include <string>
#include <tuple>
#include <QFile>
#include <QTextStream>
#include "agentInterface.h"
#include "observationInterface.h"

/**
* \addtogroup CoreModules_Basic openPASS CoreModules basic
* @{
* \addtogroup Observation_Ttc
* @{
* \brief logs the time to collision (ttc) of all agents
*
* This component logs the time to collision (ttc) of all agents.
*
* The file format is csv to easily access ttc data.
* Output data is the minimal ttc and the ttc at each time step of each agent.
*
* \section Observation_Ttc_Inputs Inputs
* none
*
* \section Observation_Ttc_Outputs Outputs
* none
*
* \section Observation_Ttc_ConfigParameters Parameters to be specified in runConfiguration.xml
* type | id | meaning | corresponding external paramter
* -----|----|---------|----------------------------------
* string | 0 | folder where simulation output shall be stored      | foldername for the result file
* string | 1 | filename for temporary simulation output            | file name for the temporary file
* string | 2 | filename for final simulation output (*.xml)        | file name for final result file
*
* @}
* @} */

/*!
 * \brief logs the time to collision (ttc) of all agents
 *
 * This component logs the time to collision (ttc) of all agents.
 *
 * The file format is csv to easily access ttc data.
 * Output data is the minimal ttc and the ttc at each time step of each agent.
 *
 * \ingroup Observation_Ttc
 */
class Observation_Ttc_Implementation : public ObservationInterface
{
public:
    const std::string COMPONENTNAME = "Observation_Ttc";

    Observation_Ttc_Implementation(StochasticsInterface *stochastics,
                                   WorldInterface *world,
                                   const ParameterInterface *parameters,
                                   const CallbackInterface *callbacks);
    Observation_Ttc_Implementation(const Observation_Ttc_Implementation &) = delete;
    Observation_Ttc_Implementation(Observation_Ttc_Implementation &&) = delete;
    Observation_Ttc_Implementation &operator=(const Observation_Ttc_Implementation &) = delete;
    Observation_Ttc_Implementation &operator=(Observation_Ttc_Implementation &&) = delete;
    virtual ~Observation_Ttc_Implementation() = default;

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

    //-----------------------------------------------------------------------------
    //! Finds the ID of the agent that is exactly in front of the agent.
    //!
    //! @return                      ID of agent in front
    int findFrontAgentID(int ownID);

private:

    /**
    * \addtogroup Observation_Ttc
    * @{
    *    @name External Parameters
    *       @{*/

    //! foldername for the result file
    std::string Par_resultFolderName;
    //! file name for the temporary file
    std::string Par_tempFileName;
    //! file name for final result file
    std::string Par_finalFileName;

    /** @} @} */

    //! A map of all agents and their ttc at every time.
    std::map<int, std::map<int, double>> agentsTtc;
    //! A map to save the minimal Ttc of an agent.
    std::map<int, double> agentsMinTtc;
    //! time vector
    std::vector<int> timeVector;
    //! full path name of result file
    std::string fullResultFilePath;
    //! ID of each run
    int runID = 0;
};

#endif // OBSERVATION_TTC_IMPLEMENTATION_H
