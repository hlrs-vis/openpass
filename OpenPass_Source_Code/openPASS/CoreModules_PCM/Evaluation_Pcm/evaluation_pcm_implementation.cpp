/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDir>

#include "evaluation_pcm_implementation.h"
#include "agent.h"
#include "collisionDetection.h"
#include "runResult.h"

//-----------------------------------------------------------------------------
//! Called by framework in master before each simulation run starts
//!
//! @return                      True on success
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::MasterPreHook()
{
    return;
}

//-----------------------------------------------------------------------------
//! Called by framework in master after each simulation run ends
//!
//! @param[in]     filename      Name of file containing the simulation run results from the slave
//! @return                      True on success
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::MasterPostHook(const std::string &filename)
{
    Q_UNUSED(filename);
    return;
}

//-----------------------------------------------------------------------------
//! Called by framework in slave before all simulation runs start
//!
//! @param[in]     path          Directory where simulation results will be stored
//! @return                      True on success
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::SlavePreHook(const std::string &path)
{
    Q_UNUSED(path);

    try {
        _supposedCollisionTime = GetParameters()->GetParametersInt().at(0);
        _timeVec = GetParameters()->GetParametersIntVector().at(1);//std::vector<int>(*GetParameters()->GetParametersIntVector().at(0));
        _xPosVec1 = GetParameters()->GetParametersDoubleVector().at(2);
        _yPosVec1 = GetParameters()->GetParametersDoubleVector().at(3);
        _uVelVec1 = GetParameters()->GetParametersDoubleVector().at(4);
        _vVelVec1 = GetParameters()->GetParametersDoubleVector().at(5);
        _psiVec1 = GetParameters()->GetParametersDoubleVector().at(6);

        _xPosVec2 = GetParameters()->GetParametersDoubleVector().at(7);
        _yPosVec2 = GetParameters()->GetParametersDoubleVector().at(8);
        _uVelVec2 = GetParameters()->GetParametersDoubleVector().at(9);
        _vVelVec2 = GetParameters()->GetParametersDoubleVector().at(10);
        _psiVec2 = GetParameters()->GetParametersDoubleVector().at(11);

        _resultFolderName = GetParameters()->GetParametersString().at(12);
        _PCMCaseNumber = GetParameters()->GetParametersString().at(13);
        _runID = GetParameters()->GetParametersString().at(14);

    } catch(const std::out_of_range&) {
        LOG(CbkLogLevel::Debug, "could not access parameters of evaluation module");
        return;
    }

    return;
}

//-----------------------------------------------------------------------------
//! Called by framework in slave before each simulation run starts
//!
//! @param[in]     path          Directory where simulation results will be stored
//! @return                      True on success
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::SlavePreRunHook()
{
    _agent1 = nullptr;
    _agent2 = nullptr;

    collisions.clear();

    _counter = 0;
    _errorSum_Participant1 = 0;
    _errorSum_Participant2 = 0;

#ifdef TEST_VELOCITY_CONTROLLER
    resetObserver();
#endif

    return;
}


//-----------------------------------------------------------------------------
//! Called by framework in slave at each time step.
//! Evaluation module can indicate end of simulation run here.
//!
//! @param[in]     time          Current scheduling time
//! @param[in,out] runResult     Reference to run result
//! @return                      True on success
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::SlaveUpdateHook(int time, RunResultInterface &runResult)
{
    if (time!=_timeVec->at(_counter))
    {
        std::stringstream sstream;
        sstream << "**#** Evaluation Error: Time from scheduling: " << time << "  Time from _timeVec: " << _timeVec->at(_counter) << std::endl;
        LOG(CbkLogLevel::Debug, sstream.str());
    }


    if (_agent1 == nullptr || _agent2 == nullptr)
    {
        if(GetWorld()->GetAgents().size() < 2 ){
            return;
        }
        std::map<int, const AgentInterface*>::const_iterator it = GetWorld()->GetAgents().cbegin();
        const AgentInterface* tmpAgent = it->second;
        if (tmpAgent->GetAgentId()==0)
        {
            _agent1 = tmpAgent ;
            ++it;
            _agent2 = it->second;
        }
        else
        {
            _agent2 = tmpAgent ;
            ++it;
            _agent1 = it->second;
        }

        if (_agent1 == nullptr || _agent2 == nullptr)
        {
            LOG(CbkLogLevel::Debug, "Error in Evaluation Pcm: Agents are nullptr");
            return;
        }
    }

    double tmp1 = calc2norm( _xPosVec1->at(_counter) - _agent1->GetPositionX(), _yPosVec1->at(_counter) - _agent1->GetPositionY() );
    double tmp2 = calc2norm( _xPosVec2->at(_counter) - _agent2->GetPositionX(), _yPosVec2->at(_counter) - _agent2->GetPositionY() );
    double currErrorVel1 = _uVelVec1->at(_counter) - _agent1->GetVelocityX();
    double currErrorVel2 = _uVelVec2->at(_counter) - _agent2->GetVelocityX();

    std::stringstream sstream;
    sstream << " #*# Norms of errors at time: " << time << std::endl <<
               " agent1: DeltaPos = " << tmp1 << ", DeltaV = v_pcm-v_agent = " << currErrorVel1 << std::endl <<
               " agent2: DeltaPos = " << tmp2 << ", DeltaV = v_pcm-v_agent = " << currErrorVel2 << std::endl;
    LOG(CbkLogLevel::Debug, sstream.str());

#ifdef TEST_VELOCITY_CONTROLLER
    _errorVel2Vec.push_back(currErrorVel2);
#endif
    _errorSum_Participant1 += tmp1;
    _errorSum_Participant2 += tmp2;

    for (uint i = 0; i < runResult.GetCollisions()->size(); i++){
            collisions.push_back(runResult.GetCollisions()->at(i));
    }


    _counter++; // maybe use time

    return;
}

//-----------------------------------------------------------------------------
//! Called by framework in slave after each simulation run ends.
//! Evaluation module can evaluate the current simulation run here.
//!
//! @param[in]     runResult     Reference to run result
//! @return                      True on success
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    if (_agent1 == nullptr || _agent2 == nullptr)
    {
        return;
    }
    std::stringstream sstream;
    sstream << "\n\t\t" << "l_{1,2}-error agent " << _agent1->GetAgentId() << " = " << _errorSum_Participant1/_counter <<
               "\n\t\t" << "l_{1,2}-error agent " << _agent2->GetAgentId() << " = " << _errorSum_Participant2/_counter << std::endl;

    LOG(CbkLogLevel::Debug, sstream.str());

    // prepare calculation of pcm collision-point

    CD_Agent locAgent1;
    locAgent1.SetWidth(_agent1->GetWidth());
    locAgent1.SetLength(_agent1->GetLength());
    locAgent1.SetDistanceCOGtoLeadingEdge(_agent1->GetDistanceCOGtoLeadingEdge());

    CD_Agent locAgent2;
    locAgent2.SetWidth(_agent2->GetWidth());
    locAgent2.SetLength(_agent2->GetLength());
    locAgent2.SetDistanceCOGtoLeadingEdge(_agent2->GetDistanceCOGtoLeadingEdge());

    RunResult pcmRunResult;
    if (!getPcmRunResult(pcmRunResult, &locAgent1, &locAgent2))
    {
        return;
    }
    // do not access locAgent1 and locAgent2 from this point onwards
    // they might be inconsistent with respect to pcmRunResult
    if(!pcmRunResult.IsCollision())
    {
        return;
    }

    if(collisions.size())
    {
        RunResultInterface::Collision collision = collisions.at(0);
        const AgentInterface* agent1 = collision.opponent1.agent;
        const AgentInterface* agent2 = collision.opponent2.agent;

        const AgentInterface* locAgent1Ptr = (AgentInterface*)(&locAgent1);
        const AgentInterface* locAgent2Ptr = (AgentInterface*)(&locAgent2);

        auto pcmColPos = pcmRunResult.GetPositions();
        auto pcmColDist = pcmRunResult.GetDistances();
        auto pcmColPsi = pcmRunResult.GetYawAngles();
        auto pcmColVel = pcmRunResult.GetVelocities();

        std::tuple<double, double> position1 = std::make_tuple(collision.opponent1.position.x, collision.opponent1.position.y);
        std::tuple<double, double> position2 = std::make_tuple(collision.opponent2.position.x, collision.opponent2.position.y);
        std::tuple<double, double> velocities1 = std::make_tuple(collision.opponent1.velocity.x, collision.opponent1.velocity.y);
        std::tuple<double, double> velocities2 = std::make_tuple(collision.opponent2.velocity.x, collision.opponent2.velocity.y);
        std::tuple<double, double> pcmPos1 = pcmColPos->at(locAgent1Ptr);
        std::tuple<double, double> pcmPos2 = pcmColPos->at(locAgent2Ptr);
        std::tuple<double, double> pcmVel1 = pcmColVel->at(locAgent1Ptr);
        std::tuple<double, double> pcmVel2 = pcmColVel->at(locAgent2Ptr);

        double posError1 = calc2norm(std::get<0>(position1)-std::get<0>(pcmPos1), std::get<1>(position1)-std::get<1>(pcmPos1));
        double posError2 = calc2norm(std::get<0>(position2)-std::get<0>(pcmPos2), std::get<1>(position2)-std::get<1>(pcmPos2));
        double xiError1 = fabs(collision.opponent1.distanceOnBorder - pcmColDist->at(locAgent1Ptr));
        double xiError2 = fabs(collision.opponent2.distanceOnBorder - pcmColDist->at(locAgent2Ptr));
        double psiError1 = fabs(collision.opponent1.yawAngle - pcmColPsi->at(locAgent1Ptr));
        double psiError2 = fabs(collision.opponent2.yawAngle - pcmColPsi->at(locAgent2Ptr));
        double absvelError1 =   calc2norm(std::get<0>(velocities1)-std::get<0>(pcmVel1), std::get<1>(velocities1)-std::get<1>(pcmVel1));
        double tmp = calc2norm(std::get<0>(pcmVel1), std::get<1>(pcmVel1));
        double relvelError1 = (tmp>0)?  absvelError1 / tmp : 0;
        double absvelError2 =   calc2norm(std::get<0>(velocities2)-std::get<0>(pcmVel2), std::get<1>(velocities2)-std::get<1>(pcmVel2));
        tmp = calc2norm(std::get<0>(pcmVel2), std::get<1>(pcmVel2));
        double relvelError2 = (tmp>0)?  absvelError2 / tmp : 0;

        std::stringstream sstream;

        sstream << "Evaluation Pcm: collision between agent " << agent1->GetAgentId()
                << " and agent " << agent2->GetAgentId() << std::endl
                << " collision info agent " << agent1->GetAgentId() << "    :" <<
                   " position = (" << std::get<0>(position1) << ", " << std::get<1>(position1) << ")," <<
                   " xi = " << collision.opponent1.distanceOnBorder << "," <<
                   " psi = " << collision.opponent1.yawAngle << "," <<
                   " velocity = (" << std::get<0>(velocities1) << ", " << std::get<1>(velocities1) << ")" << std::endl
                << " pcm collision info agent " << agent1->GetAgentId() << ":" <<
                   " position = (" << std::get<0>(pcmPos1) << ", " << std::get<1>(pcmPos1) << ")," <<
                   " xi = " << pcmColDist->at(locAgent1Ptr) << "," <<
                   " psi = " << pcmColPsi->at(locAgent1Ptr) << "," <<
                   " velocity = (" << std::get<0>(pcmVel1) << ", " << std::get<1>(pcmVel1) << ")" << std::endl
                << " collision info agent " << agent2->GetAgentId() << "    :" <<
                   " position = (" << std::get<0>(position2) << ", " << std::get<1>(position2) << ")," <<
                   " xi = " << collision.opponent2.distanceOnBorder << "," <<
                   " psi = " << collision.opponent2.yawAngle << "," <<
                   " velocity = (" << std::get<0>(velocities2) << ", " << std::get<1>(velocities2) << ")"<< std::endl
                << " pcm collision info agent " << agent2->GetAgentId() << ":" <<
                   " position = (" << std::get<0>(pcmPos2) << ", " << std::get<1>(pcmPos2) << ")," <<
                   " xi = " << pcmColDist->at(locAgent2Ptr) << "," <<
                   " psi = " << pcmColPsi->at(locAgent2Ptr) << "," <<
                   " velocity = (" << std::get<0>(pcmVel2) << ", " << std::get<1>(pcmVel2) << ")" << std::endl;


       sstream << std::endl
                << " Position-error agent " << agent1->GetAgentId() << ": " << posError1 << std::endl
                << " Position-error agent " << agent2->GetAgentId() << ": " << posError2 << std::endl
                << " xi-error " << agent1->GetAgentId() << ": " << xiError1 << std::endl
                << " xi-error " << agent2->GetAgentId() << ": " << xiError2 << std::endl
                << " psi-error " << agent1->GetAgentId() << ": " << psiError1 << std::endl
                << " psi-error " << agent2->GetAgentId() << ": " << psiError2 << std::endl
                << " absvel-error " << agent1->GetAgentId() << ": " << absvelError1 << std::endl
                << " relvel-error " << agent1->GetAgentId() << ": " << relvelError1 << std::endl
                << " absvel-error " << agent2->GetAgentId() << ": " << absvelError2 << std::endl
                << " relvel-error " << agent2->GetAgentId() << ": " << relvelError2 << std::endl;

        sstream << std::endl << "compact: " <<  _errorSum_Participant1/_counter << ", "
                << _errorSum_Participant2/_counter << ", "
                << posError1 << ", " << posError2 << ", "
                << xiError1 << ", " << xiError2 << ", "
                << psiError1 << ", " << psiError2 << ", "
                << absvelError1 << ", " << absvelError2 << ", "
                << relvelError1 << ", " << relvelError2 << std::endl;

        LOG(CbkLogLevel::Debug, sstream.str());

        //generate csv.output file
        std::string fullResultFilePath = _resultFolderName + "/result.csv";
        char sep = ';';

        std::stringstream log;
        QString resultFolderName = QString::fromStdString(_resultFolderName);
        bool enableResultFile = !_resultFolderName.empty();
        if(enableResultFile && !QDir().exists(resultFolderName))
        {
            QDir().mkdir(resultFolderName);
        }

        if(!QDir().exists(resultFolderName))
        {
            enableResultFile = false;
            log << "result directory " << _resultFolderName << " does not exist and could not be created";
            LOG(CbkLogLevel::Debug, log.str());
        }

        if(enableResultFile)
        {
            bool isResultFileNew = !QFile().exists(QString::fromStdString(fullResultFilePath));
            std::ofstream resultFile;
            resultFile.open(fullResultFilePath, std::ofstream::out | std::ofstream::app);

            if(!resultFile.is_open())
            {
                log.str(std::string());
                log << "result File could not be opened";
                LOG(CbkLogLevel::Debug, log.str());

                return;
            }

            if(isResultFileNew)
            {
                writeHeaderCsvFile(resultFile, sep);
            }
            else
            {
                //resultfile contains something, assume there already is a header, write an empty line (i.e. only seps for each column)
                for (int i=0; i<20;++i)
                {
                    resultFile << sep;
                }
                resultFile << "\n";
            }

            // write out agent1
            resultFile << _runID << sep
                       << _PCMCaseNumber << sep
                       << agent1->GetAgentId() << sep
                       << (int)agent1->GetVehicleType()<< sep
                       << std::get<0>(position1) << sep
                       << std::get<1>(position1) << sep
                       << std::get<0>(pcmPos1) << sep
                       << std::get<1>(pcmPos1) << sep
                       << posError1 << sep
                       << std::get<0>(velocities1) << sep
                       << std::get<1>(velocities1) << sep
                       << std::get<0>(pcmVel1) << sep
                       << std::get<1>(pcmVel1) << sep
                       << absvelError1 << sep
                       << relvelError1 << sep
                       << collision.opponent1.yawAngle << sep
                       << pcmColPsi->at(locAgent1Ptr) << sep
                       << psiError1 << sep
                       << collision.opponent1.distanceOnBorder << sep
                       << pcmColDist->at(locAgent1Ptr) << sep
                       << xiError1 << "\n";

            // write out agent2; write run ID and PCMCaseNumber only once
            resultFile << sep << sep
                       << agent2->GetAgentId() << sep
                       << (int)agent2->GetVehicleType() << sep
                       << std::get<0>(position2) << sep
                       << std::get<1>(position2) << sep
                       << std::get<0>(pcmPos2) << sep
                       << std::get<1>(pcmPos2) << sep
                       << posError2 << sep
                       << std::get<0>(velocities2) << sep
                       << std::get<1>(velocities2) << sep
                       << std::get<0>(pcmVel2) << sep
                       << std::get<1>(pcmVel2) << sep
                       << absvelError2 << sep
                       << relvelError2 << sep
                       << collision.opponent2.yawAngle << sep
                       << pcmColPsi->at(locAgent2Ptr) << sep
                       << psiError2 << sep
                       << collision.opponent2.distanceOnBorder << sep
                       << pcmColDist->at(locAgent2Ptr) << sep
                       << xiError2 << "\n";

            resultFile.close();
        }

#ifdef TEST_VELOCITY_CONTROLLER
        if(QDir().exists(resultFolderName))
        {
            std::string vecFilePath = _resultFolderName + "/vec.csv";
            std::ofstream vecFile;
            vecFile.open(vecFilePath, std::ofstream::out | std::ofstream::app);
            vecFile << "\n";

            dumpVec(vecFile, _errorVel2Vec);
            dumpVec(vecFile, _gasPedalVec);
            dumpVec(vecFile, _brakePedalVec);
            dumpVec(vecFile, _lowPassValueVec);

            vecFile.close();
        }
#endif

    }
    else if(runResult.IsTimeOver())
    {
        LOG(CbkLogLevel::Debug, "Evaluation Pcm: evaluated time over -- no collision!");
    }
    else if(runResult.IsEndCondition())
    {
        LOG(CbkLogLevel::Debug, "evaluated end condition");
    }
    else
    {
        LOG(CbkLogLevel::Debug, "evaluated invalid result - no matching EndCondition");
        return;
    }


    return;
}


//-----------------------------------------------------------------------------
//! Writes the input vector into the file
//!
//! @param[in]  resultFile      the stream towards the outfile
//! @param[in]  vec             the vector that should be written into the file
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::dumpVec(std::ofstream& file, std::vector<double> vec)
{
    for (unsigned int ii=0; ii< vec.size(); ii++)
    {
        file << vec[ii] << "\t";
    }
    file << "\n";
}


//-----------------------------------------------------------------------------
//! Called by framework in slave after all simulation runs end.
//!
//! @return                      True on success
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::SlavePostHook()
{
    return;
}


//-----------------------------------------------------------------------------
//! Called by framework in slave after all simulation runs end to transfer the
//! evaluation module results to the master.
//!
//! @return                      File to be transferred
//-----------------------------------------------------------------------------
const std::string Evaluation_Pcm_Implementation::SlaveResultFile()
{
    return "";
}


//-----------------------------------------------------------------------------
//! Returns the result of the pcm run
//!
//! @param[out] pcmRunResult    the class containing all information on the result
//! @param[in]  locAgent1       first agent
//! @param[in]  locAgent2       second agent
//! @return                     true on success
//-----------------------------------------------------------------------------
bool Evaluation_Pcm_Implementation::getPcmRunResult(RunResult &pcmRunResult,
                                                    CD_Agent *locAgent1,
                                                    CD_Agent *locAgent2)
{
    // get the time step, where the collision is supposed to occur first
    int fixedStepSize = 10;
    size_t indexCollision = _supposedCollisionTime/fixedStepSize;
    if (_timeVec->at(indexCollision)!=_supposedCollisionTime)
    {
        LOG(CbkLogLevel::Debug, "Error in Evaluation Pcm: Collision time not at expected position.");
        return false;
    }

    // check for collision
    bool isCollision;
    setAgentPropertiesAtTimeStep(indexCollision, locAgent1, locAgent2);
    if (!CollisionDetection::IsCollision(locAgent1, locAgent2, isCollision))
    {
        LOG(CbkLogLevel::Debug, "Error in Evaluation Pcm: could not process collision detection 1.");
        return false;
    }

    if (isCollision)
    {
        size_t lastIndexCollision = indexCollision;
        while (isCollision)
        {
            if (0 == indexCollision)
            {
                if(!CollisionDetection::CreateResult(locAgent1, locAgent2, pcmRunResult)) {
                    return false;
                }

                return true;
            }

            lastIndexCollision = indexCollision;
            --indexCollision;

            setAgentPropertiesAtTimeStep(indexCollision, locAgent1, locAgent2);
            if (!CollisionDetection::IsCollision(locAgent1, locAgent2, isCollision))
            {
                LOG(CbkLogLevel::Debug, "Error in Evaluation Pcm: could not process collision detection 2.");
                return false;
            }
        }

        setAgentPropertiesAtTimeStep(lastIndexCollision, locAgent1, locAgent2);
        if(!CollisionDetection::CreateResult(locAgent1, locAgent2, pcmRunResult)) {
            return false;
        }

        return true;
    } else {
        while (!isCollision)
        {
            ++indexCollision;
            if (indexCollision==_xPosVec1->size())
            {
                LOG(CbkLogLevel::Debug, "Error in Evaluation Pcm: Never collision in pcm case?");
                return false;
            }

            setAgentPropertiesAtTimeStep(indexCollision, locAgent1, locAgent2);
            if (!CollisionDetection::IsCollision(locAgent1, locAgent2, isCollision))
            {
                LOG(CbkLogLevel::Debug, "Error in Evaluation Pcm: could not process collision detection 3.");
                return false;
            }
        }

        if(!CollisionDetection::CreateResult(locAgent1, locAgent2, pcmRunResult)) {
            return false;
        }

        return true;
    }

    return false;
}


//-----------------------------------------------------------------------------
//! (Re-)Sets the transient values of the agent to the ones that hold at the
//! given time step
//!
//! @param[in]  index           the index of the considered time step
//! @param[in]  locAgent1       first agent
//! @param[in]  locAgent2       second agent
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::setAgentPropertiesAtTimeStep(int index,
                                                                 CD_Agent *locAgent1,
                                                                 CD_Agent *locAgent2)
{
    locAgent1->SetPositionX(_xPosVec1->at(index));
    locAgent1->SetPositionY(_yPosVec1->at(index));
    locAgent1->SetVelocityX(_uVelVec1->at(index));
    locAgent1->SetVelocityY(_vVelVec1->at(index));
    locAgent1->SetYawAngle(_psiVec1->at(index));

    locAgent2->SetPositionX(_xPosVec2->at(index));
    locAgent2->SetPositionY(_yPosVec2->at(index));
    locAgent2->SetVelocityX(_uVelVec2->at(index));
    locAgent2->SetVelocityY(_vVelVec2->at(index));
    locAgent2->SetYawAngle(_psiVec2->at(index));

}

//-----------------------------------------------------------------------------
//! Writes the header of the output-csv
//!
//! @param[in]  resultFile      the stream towards the outfile
//! @param[in]  sep             seperator char for the csv
//-----------------------------------------------------------------------------
void Evaluation_Pcm_Implementation::writeHeaderCsvFile(std::ofstream& resultFile, char sep)
{
    resultFile << "runID" << sep
               << "PCMCase" << sep
               << "agentID" << sep
               << "agentTypeID" << sep
               << "positionX" << sep
               << "positionY" << sep
               << "PCM_positionX" << sep
               << "PCM_positionY" << sep
               << "position_2norm_error" << sep
               << "velocityX" << sep
               << "velocityY" << sep
               << "PCM_velocityX" << sep
               << "PCM_velocityY" << sep
               << "abs_velocity_error" << sep
               << "rel_velocity_error" << sep
               << "psi" << sep
               << "PCM_psi" << sep
               << "psi_error" << sep
               << "xi" << sep
               << "PCM_xi" << sep
               << "xi_error\n";

    resultFile << sep
               << sep
               << sep
               << sep
               << "[m]" << sep
               << "[m]" << sep
               << "[m]" << sep
               << "[m]" << sep
               << "[m]" << sep
               << "[m/s]" << sep
               << "[m/s]" << sep
               << "[m/s]" << sep
               << "[m/s]" << sep
               << "[m/s]" << sep
               << "[1]" << sep
               << "[rad]" << sep
               << "[rad]" << sep
               << "[rad]" << sep
               << "[m]" << sep
               << "[m]" << sep
               << "[m]\n";
}
