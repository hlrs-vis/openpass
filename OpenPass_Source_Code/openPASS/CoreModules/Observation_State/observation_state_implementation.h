/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef OBSERVATION_STATE_IMPLEMENTATION_H
#define OBSERVATION_STATE_IMPLEMENTATION_H

#include <string>
#include <tuple>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include "observationInterface.h"
#include "observationtypes.h"

/**
* \addtogroup CoreModules_Basic openPASS CoreModules basic
* @{
* \addtogroup Observation_State
* @{
* \brief logs the current X position of every agent at each time step.
*
* This component logs the states of the agents that are necessary for the
* visualisation or the evaluation into a file.
*
* The file format is xml to easily access time series or statistics data in an
* arbitrary editor.
*
* This component in this version is just for debug purpose.
*
* \section Observation_State_Inputs Inputs
* none
*
* \section Observation_State_Outputs Outputs
* none
*
* \section Observation_State_ConfigParameters Parameters to be specified in runConfiguration.xml
* type | id | meaning | corresponding external paramter
* -----|----|---------|----------------------------------
* string | 0 | folder where simulation output shall be stored      | Par_folder
* string | 1 | filename for temporary simulation output            | Par_tmpFilename
* string | 2 | filename for final simulation output (*.xml)        | Par_finalFilename
* bool   | 0 | flag if visualization data shall be written to file | Par_WriteOutputForVisualisation
*
* @}
* @} */

/*!
 * \brief logs the current X position of every agent at each time step.
 *
 * This component logs the states of the agents that are necessary for the
 * visualisation or the evaluation into a file.
 *
 * The file format is xml to easily access time series or statistics data in an
 * arbitrary editor.
 *
 * This component in this version is just for debug purpose.
 *
 * \ingroup Observation_State
 */
class Observation_State_Implementation : public ObservationInterface
{
public:
    const std::string COMPONENTNAME = "Observation_State";

    Observation_State_Implementation(StochasticsInterface *stochastics,
                                     WorldInterface *world,
                                     const ParameterInterface *parameters,
                                     const CallbackInterface *callbacks);
    Observation_State_Implementation(const Observation_State_Implementation &) = delete;
    Observation_State_Implementation(Observation_State_Implementation &&) = delete;
    Observation_State_Implementation &operator=(const Observation_State_Implementation &) = delete;
    Observation_State_Implementation &operator=(Observation_State_Implementation &&) = delete;
    virtual ~Observation_State_Implementation() = default;

    //-----------------------------------------------------------------------------
    //! Called by framework in master before each simulation run starts
    //-----------------------------------------------------------------------------
    virtual void MasterPreHook() {}

    //-----------------------------------------------------------------------------
    //! Called by framework in master after each simulation run ends
    //!
    //! @param[in]     filename      Name of file containing the simulation run results from the slave
    //-----------------------------------------------------------------------------
    virtual void MasterPostHook(const std::string &filename)
    {
        Q_UNUSED(filename)
    }

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
    //-----------------------------------------------------------------------------
    virtual const std::string SlaveResultFile()
    {
        return "";   //dummy
    }

private:
    void RecordAllAgents(int time);
    void RecordAgentState(int time, const AgentInterface *agent);
    void Insert(int time,
                int agentId,
                Observation_State_Periodic_Type valueType,
                const std::string &value);
    void AddPositionXForCSV(int agentId, int time, double positionX);
    void WriteAgentPositionsToCSV();

    static const std::string PeriodicTypeStrings[];
    static const std::string EventTypeStrings[];

    /**
    * \addtogroup Observation_State
    * @{
    *    @name External Parameters
    *       @{*/
    // from Parameters
    //! This flag states whether an output for visualisation should be written out or not.
    bool Par_WriteOutputForVisualisation = true;
    /** @} @} */

    std::vector<int> timeChannel;                                //!< time values
    std::map<std::string, std::vector<std::string>> channels;    //!< samples
    std::map<int, std::map<int, double>> agentsPositionX;
    std::shared_ptr<QXmlStreamWriter> fileStream;

    const QString RunResultTag = "RunResult";
    const QString RunIdAttribute = "RunId";

    int runNumber = 0;                                            //!< run number
    const QString RunResultsTag = "RunResults";
    const QString SimulationOutputTag = "SimulationOutput";
    const QString TimeAttribute = "Time";
    const QString CyclicsTag = "Cyclics";
    const QString HeaderTag = "Header";
    const QString SamplesTag = "Samples";
    const QString SampleTag = "Sample";

    std::string Par_folder;
    std::string Par_tmpFilename;
    std::string Par_finalFilename;
    std::string tmpPath;
    std::string finalPath;
    std::shared_ptr<QFile> file;
    std::string resultPathCSV;
    std::string resultFileCSV = "simulationPositionX.csv";
};

#endif // OBSERVATION_STATE_IMPLEMENTATION_H
