/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  algorithm_trajectoryFollower_implementation.h
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


//-----------------------------------------------------------------------------
//! Observer interface used for exchange with component model
//! avoids the need to include evaluation implementation during compilation of
//! component model -> component model only casts to intermediate interface and
//! calls functions of evaluation implementation by polymorphism
//! -- Only needed if extra output from the trajectory follower is requested
//-----------------------------------------------------------------------------
class Evaluation_Pcm_ObserverInterface : public ObservationInterface
{
public:
    using ObservationInterface::ObservationInterface;
    // removing operators
    Evaluation_Pcm_ObserverInterface(const Evaluation_Pcm_ObserverInterface&) = delete;
    Evaluation_Pcm_ObserverInterface(Evaluation_Pcm_ObserverInterface&&) = delete;
    Evaluation_Pcm_ObserverInterface& operator=(const Evaluation_Pcm_ObserverInterface&) = delete;
    Evaluation_Pcm_ObserverInterface& operator=(Evaluation_Pcm_ObserverInterface&&) = delete;
    //-----------------------------------------------------------------------------
    //! Standard destructor
    //-----------------------------------------------------------------------------
    virtual ~Evaluation_Pcm_ObserverInterface() = default;

#ifdef TEST_VELOCITY_CONTROLLER
    void SetDriverValues(double gas, double brake, double lowPassValue)
    {
        _gasPedalVec.push_back(gas);
        _brakePedalVec.push_back(brake);
        _lowPassValueVec.push_back(lowPassValue);
    }

protected:
    std::vector<double> _errorVel2Vec;
    std::vector<double> _gasPedalVec;
    std::vector<double> _brakePedalVec;
    std::vector<double> _lowPassValueVec;

    void resetObserver()
    {
        _errorVel2Vec.clear();
        _gasPedalVec.clear();
        _brakePedalVec.clear();
        _lowPassValueVec.clear();
    }
#endif

};


//-----------------------------------------------------------------------------
//! Implementation interface class for the module Evaluation_Pcm
//-----------------------------------------------------------------------------
class Evaluation_Pcm_Implementation : public Evaluation_Pcm_ObserverInterface
{
public:
    using Evaluation_Pcm_ObserverInterface::Evaluation_Pcm_ObserverInterface;

    // removing operators
    Evaluation_Pcm_Implementation(const Evaluation_Pcm_Implementation&) = delete;
    Evaluation_Pcm_Implementation(Evaluation_Pcm_Implementation&&) = delete;
    Evaluation_Pcm_Implementation& operator=(const Evaluation_Pcm_Implementation&) = delete;
    Evaluation_Pcm_Implementation& operator=(Evaluation_Pcm_Implementation&&) = delete;

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
        return sqrt(x*x+y*y);
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

    int _supposedCollisionTime;             //!< the time of the collision according to the pcm
    const std::vector<int>*    _timeVec;    //!< vector of the time steps
    const std::vector<double>* _xPosVec1;   //!< vector of the x-coordinates for the first agent
    const std::vector<double>* _yPosVec1;   //!< vector of the y-coordinates for the first agent
    const std::vector<double>* _uVelVec1;   //!< vector of the lateral velocities for the first agent
    const std::vector<double>* _vVelVec1;   //!< vector of the vertical velocities for the first agent
    const std::vector<double>* _psiVec1;    //!< vector of the yaw angle for the first agent
    const std::vector<double>* _xPosVec2;   //!< vector of the x-coordinates for the second agent
    const std::vector<double>* _yPosVec2;   //!< vector of the y-coordinates for the second agent
    const std::vector<double>* _uVelVec2;   //!< vector of the lateral velocities for the second agent
    const std::vector<double>* _vVelVec2;   //!< vector of the vertical velocities for the second agent
    const std::vector<double>* _psiVec2;    //!< vector of the yaw angle for the second agent

    const AgentInterface* _agent1;          //!< class with all information on the first agent
    const AgentInterface* _agent2;          //!< class with all information on the second agent

    int _counter;                           //!< counts the time steps
    double _errorSum_Participant1;          //!< summing up the errors of the positions of agent 1
    double _errorSum_Participant2;          //!< summing up the errors of the positions of agent 2

    std::string _resultFolderName;          //!< foldername for the result file
    std::string _runID;                     //!< id for the run
    std::string _PCMCaseNumber;             //!< name / number of the PCM case

    std::vector<RunResultInterface::Collision> collisions; //!< vector to safe collisions
};

#endif // EVALUATION_PCM_IMPLEMENTATION_H
