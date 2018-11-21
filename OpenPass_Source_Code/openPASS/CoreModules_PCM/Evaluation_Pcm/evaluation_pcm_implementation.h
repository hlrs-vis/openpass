/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  evaluation_pcm_implementation.h
//! @brief This file contains the specialised model implementation interface
//!        used by the simulation framework.
//-----------------------------------------------------------------------------

#ifndef EVALUATION_PCM_IMPLEMENTATION_H
#define EVALUATION_PCM_IMPLEMENTATION_H

#include <iostream>
#include <fstream>

#include <string>
#include "observationInterface.h"
#include "agent.h"
#include "runResult.h"
#include "componentPorts.h"

/**
* \addtogroup CoreModules_PCM openPASS CoreModules pcm
* @{
* \addtogroup Evaluation_Pcm
*
* \brief Observation module to evaluate a dynamic model
*
* This module was used in a pre openPASS version to evaluate a FMU with a two track model inside.\n
* It was only adjusted to fit with the current version of openPASS.\n
* Its functionality may be deprecated.
*
* @see ObservationInterface
* @}
*/

/*!
 * \copydoc Evaluation_Pcm
 * \ingroup Evaluation_Pcm
 */
class Evaluation_Pcm_Implementation : public ObservationInterface
{
public:
    const std::string COMPONENTNAME = "Evaluation_Pcm";

    Evaluation_Pcm_Implementation(StochasticsInterface *stochastics,
                                  WorldInterface *world,
                                  const ParameterInterface *parameters,
                                  const CallbackInterface *callbacks);

    // removing operators
    Evaluation_Pcm_Implementation(const Evaluation_Pcm_Implementation &) = delete;
    Evaluation_Pcm_Implementation(Evaluation_Pcm_Implementation &&) = delete;
    Evaluation_Pcm_Implementation &operator=(const Evaluation_Pcm_Implementation &) = delete;
    Evaluation_Pcm_Implementation &operator=(Evaluation_Pcm_Implementation &&) = delete;

    //-----------------------------------------------------------------------------
    //! Standard destructor
    //-----------------------------------------------------------------------------
    virtual ~Evaluation_Pcm_Implementation() = default;

    //-----------------------------------------------------------------------------
    //! Called by framework in master before each simulation run starts
    //!
    //! @return                      True on success
    //-----------------------------------------------------------------------------
    virtual void MasterPreHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in master after each simulation run ends
    //!
    //! @param[in]     filename      Name of file containing the simulation run results from the slave
    //! @return                      True on success
    //-----------------------------------------------------------------------------
    virtual void MasterPostHook(const std::string &filename);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before all simulation runs start
    //!
    //! @param[in]     path          Directory where simulation results will be stored
    //! @return                      True on success
    //-----------------------------------------------------------------------------
    virtual void SlavePreHook(const std::string &path);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before each simulation run starts
    //!
    //! @param[in]     path          Directory where simulation results will be stored
    //! @return                      True on success
    //-----------------------------------------------------------------------------
    virtual void SlavePreRunHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave at each time step.
    //! Evaluation module can indicate end of simulation run here.
    //!
    //! @param[in]     time          Current scheduling time
    //! @param[in,out] runResult     Reference to run result
    //! @return                      True on success
    //-----------------------------------------------------------------------------
    virtual void SlaveUpdateHook(int time, RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after each simulation run ends.
    //! Evaluation module can evaluate the current simulation run here.
    //!
    //! @param[in]     runResult     Reference to run result
    //! @return                      True on success
    //-----------------------------------------------------------------------------
    virtual void SlavePostRunHook(const RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end.
    //!
    //! @return                      True on success
    //-----------------------------------------------------------------------------
    virtual void SlavePostHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end to transfer the
    //! evaluation module results to the master.
    //!
    //! @return                      File to be transferred
    //-----------------------------------------------------------------------------
    virtual const std::string SlaveResultFile();

private:

    //-----------------------------------------------------------------------------
    //! Calculates the 2-norm of the two inputs
    //!
    //! @param[in]  x               first input
    //! @param[in]  y               second input
    //! @return                     norm
    //-----------------------------------------------------------------------------
    double calc2norm(double x, double y)
    {
        return sqrt(x * x + y * y);
    }


    //-----------------------------------------------------------------------------
    //! Returns the result of the pcm run
    //!
    //! @param[out] pcmRunResult    the class containing all information on the result
    //! @param[in]  locAgent1       first agent
    //! @param[in]  locAgent2       second agent
    //! @return                     true on success
    //-----------------------------------------------------------------------------
    bool getPcmRunResult(RunResult &pcmRunResult,
                         CD_Agent *locAgent1,
                         CD_Agent *locAgent2);

    //-----------------------------------------------------------------------------
    //! (Re-)Sets the transient values of the agent to the ones that hold at the
    //! given time step
    //!
    //! @param[in]  index           the index of the considered time step
    //! @param[in]  locAgent1       first agent
    //! @param[in]  locAgent2       second agent
    //-----------------------------------------------------------------------------
    void setAgentPropertiesAtTimeStep(int index,
                                      CD_Agent *locAgent1,
                                      CD_Agent *locAgent2);

    //-----------------------------------------------------------------------------
    //! Writes the header of the output-csv
    //!
    //! @param[in]  resultFile      the stream towards the outfile
    //! @param[in]  sep             seperator char for the csv
    //-----------------------------------------------------------------------------
    void writeHeaderCsvFile(std::ofstream &resultFile, char sep);

    //-----------------------------------------------------------------------------
    //! Writes the input vector into the file
    //!
    //! @param[in]  resultFile      the stream towards the outfile
    //! @param[in]  vec             the vector that should be written into the file
    //-----------------------------------------------------------------------------
    void dumpVec(std::ofstream &file, std::vector<double> vec);

    std::map<int, externalParameter<int>*> parameterMapInt;
    std::map<int, externalParameter<const std::vector<int> *>*> parameterMapIntVector;
    std::map<int, externalParameter<const std::vector<double> *>*> parameterMapDoubleVector;
    std::map<int, externalParameter<std::string>*> parameterMapString;
    /** \addtogroup Evaluation_Pcm
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in runConfiguration file.
     *      @{
     */
    externalParameter<int> supposedCollisionTime {0, &parameterMapInt };               //!< the time of the collision according to the pcm

    externalParameter<const std::vector<int> *> timeVec {1, &parameterMapIntVector };  //!< vector of the time steps

    externalParameter<const std::vector<double> *> xPosVec1 {2, &parameterMapDoubleVector };   //!< vector of the x-coordinates for the first agent
    externalParameter<const std::vector<double> *> yPosVec1 {3, &parameterMapDoubleVector };   //!< vector of the y-coordinates for the first agent
    externalParameter<const std::vector<double> *> uVelVec1 {4, &parameterMapDoubleVector };   //!< vector of the lateral velocities for the first agent
    externalParameter<const std::vector<double> *> vVelVec1 {5, &parameterMapDoubleVector };   //!< vector of the vertical velocities for the first agent
    externalParameter<const std::vector<double> *> psiVec1 {6, &parameterMapDoubleVector };    //!< vector of the yaw angle for the first agent

    externalParameter<const std::vector<double> *> xPosVec2 {7, &parameterMapDoubleVector };   //!< vector of the x-coordinates for the second agent
    externalParameter<const std::vector<double> *> yPosVec2 {8, &parameterMapDoubleVector };   //!< vector of the y-coordinates for the second agent
    externalParameter<const std::vector<double> *> uVelVec2 {9, &parameterMapDoubleVector };   //!< vector of the lateral velocities for the second agent
    externalParameter<const std::vector<double> *> vVelVec2 {10, &parameterMapDoubleVector };  //!< vector of the vertical velocities for the second agent
    externalParameter<const std::vector<double> *> psiVec2 {11, &parameterMapDoubleVector };   //!< vector of the yaw angle for the second agent

    externalParameter<std::string> resultFolderName {12, &parameterMapString }; //!< foldername for the result file
    externalParameter<std::string> runID {13, &parameterMapString };            //!< id for the run
    externalParameter<std::string> PCMCaseNumber {14, &parameterMapString };    //!< name / number of the PCM case

    /**
     *      @}
     *  @}
     */

    const AgentInterface *agent1;          //!< class with all information on the first agent
    const AgentInterface *agent2;          //!< class with all information on the second agent

    unsigned int counter;                  //!< counts the time steps
    double errorSum_Participant1;          //!< summing up the errors of the positions of agent 1
    double errorSum_Participant2;          //!< summing up the errors of the positions of agent 2

    std::vector<RunResultInterface::Collision> collisions; //!< vector to safe collisions
};

#endif // EVALUATION_PCM_IMPLEMENTATION_H
