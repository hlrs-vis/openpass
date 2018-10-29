/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDir>
#include "observation_ttc_implementation.h"

Observation_Ttc_Implementation::Observation_Ttc_Implementation(StochasticsInterface *stochastics,
                                                               WorldInterface *world,
                                                               const ParameterInterface *parameters,
                                                               const CallbackInterface *callbacks) :
    ObservationInterface(stochastics, world, parameters, callbacks)
{
    // read parameters
    try
    {
        Par_resultFolderName = GetParameters()->GetParametersString().at(0);
        Par_tempFileName = GetParameters()->GetParametersString().at(1);
        Par_finalFileName = GetParameters()->GetParametersString().at(2);
    }
    catch(...)
    {
        const std::string msg = COMPONENTNAME + " could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}

void Observation_Ttc_Implementation::SlavePreHook(const std::string &path)
{
    Q_UNUSED(path);
    QString resultFolderName = QString::fromStdString(Par_resultFolderName);
    if(!QDir().exists(resultFolderName))
    {
        QDir().mkdir(resultFolderName);
    }

    fullResultFilePath = Par_resultFolderName + "/" + Par_finalFileName;

    if(QFile().exists(QString::fromStdString(fullResultFilePath))){
        QFile::remove(QString::fromStdString(fullResultFilePath));
    }
}

void Observation_Ttc_Implementation::SlavePreRunHook()
{
    agentsMinTtc.clear();
    agentsTtc.clear();
    timeVector.clear();
}

//--------------------- update in run ----------------------------//
void Observation_Ttc_Implementation::SlaveUpdateHook(int time, RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    //fill time vector
    //While Initializing the framework this update function is called twice.
    //time = 0 would be twice in vector
    //Check for duplicates
    if(timeVector.size() == 0 || timeVector.back() != time)
    {
        timeVector.push_back(time);
    }

    //calculate ttc for all agents
    for(uint id = 0; id < GetWorld()->GetAgents().size(); id++)
    {
        int frontID = findFrontAgentID(id);
        if(frontID < 0){ //if nobody is in front, then continue
            continue;
        }

        const AgentInterface *agent = GetWorld()->GetAgent(id);
        const AgentInterface *frontAgent = GetWorld()->GetAgent(frontID);

        double deltaX = frontAgent->GetPositionX() - agent->GetPositionX();
        double deltaV = frontAgent->GetVelocityX() - agent->GetVelocityX();

        double ttc = - deltaX / deltaV;

        if(ttc < 0){// a negative ttc means that there will never be a collision
            ttc = INFINITY;
        }

        // save the minimal ttc
        std::map<int, double>::iterator agentsMinIterator = agentsMinTtc.find(id);
        if(agentsMinIterator == agentsMinTtc.end()){
            agentsMinTtc.emplace(std::make_pair(id, ttc));
        }else{
            if(agentsMinIterator->second > ttc){
                agentsMinIterator->second = ttc;
            }
        }

        //save all ttc
        std::map<int, std::map<int, double>>::iterator agentIterator = agentsTtc.find(id);
        if(agentIterator != agentsTtc.end()){
            agentIterator->second.emplace(time, ttc);
        }else{
            std::map<int, double> agentTtc;
            agentTtc.emplace(std::make_pair(time, ttc));
            agentsTtc.emplace(std::make_pair(id, agentTtc));
        }
    }
}

void Observation_Ttc_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    std::ofstream resultFile;
    resultFile.open(fullResultFilePath, std::ofstream::out | std::ofstream::app);

    if(!resultFile.is_open())
    {
        LOG(CbkLogLevel::Debug, "result File could not be opened");
        return;
    }
    std::string sep = ";";
    std::string endLine = "\n";

    //header
    resultFile  << "RunID" << sep
                << "AgentID" << sep
                << "minimal TTC" << sep;

    // write fist line all time steps
    for(uint i = 0; i < timeVector.size(); ++i){
        resultFile << timeVector.at(i) << sep;
    }
    resultFile << endLine;

    resultFile << runID; // write runID in first column
    //data
    for(auto const &agentID : agentsTtc){
        resultFile << sep; // leave first column always empty (reserved for runID)
        resultFile << agentID.first << sep; //write ID of agent
        resultFile << agentsMinTtc.find(agentID.first)->second << sep; //write minimal ttc of this agent
        for(uint i = 0; i < timeVector.size(); ++i){ //write all ttc at every specific time step
            std::map<int, double> agentTtcs = agentID.second;
            std::map<int, double>::iterator it = agentTtcs.find(timeVector.at(i));
            if(it != agentTtcs.end() ){// if ttc is found at specific time step, then write it
                resultFile << it->second;
            }// else leave blank
            resultFile << sep;
        }
        resultFile << endLine;
    }
    resultFile << endLine;

    resultFile.close();

    runID++;
}

void Observation_Ttc_Implementation::SlavePostHook()
{

}

int Observation_Ttc_Implementation::findFrontAgentID(int ownID)
{
    const AgentInterface *agent = GetWorld()->GetAgent(ownID);
    double posX = agent->GetPositionX();
    double minDistance = INFINITY;
    int frontID = -1;

    for (const auto &it: GetWorld()->GetAgents()){
        const AgentInterface *otherAgent = it.second;
        double posXother = otherAgent->GetPositionX();
        if ((otherAgent->GetAgentId() != ownID) && (posX < posXother))
        {
            double distance = posXother - posX;
            if(minDistance > distance){
                minDistance = distance;
                frontID = otherAgent->GetAgentId();
            }
        }
    }

    return frontID;
}
