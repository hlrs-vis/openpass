/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "observation_scopeLogger_implementation.h"

Observation_ScopeLogger_Implementation::Observation_ScopeLogger_Implementation(
    StochasticsInterface *stochastics,
    WorldInterface *world,
    const ParameterInterface *parameters,
    const CallbackInterface *callbacks) :
    ObservationInterface(stochastics, world, parameters, callbacks)
{
}

void Observation_ScopeLogger_Implementation::SlavePreHook(const std::string &path)
{
    resultFolder = path;

    if (!QDir().exists(QString::fromStdString(resultFolder)))
    {
        QDir().mkdir(QString::fromStdString(resultFolder));
    }
}

void Observation_ScopeLogger_Implementation::SlavePreRunHook()
{
    timeVector.clear();
}

//--------------------- update in run ----------------------------//
void Observation_ScopeLogger_Implementation::SlaveUpdateHook(int time,
                                                             RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    //fill time vector
    //While Initializing the framework this update function is called twice.
    //time = 0 would be twice in vector
    //Check for duplicates
    if (timeVector.size() == 0 || timeVector.back() != time)
    {
        timeVector.push_back(time);
    }

    //calculate timeStepData for all agents
    for (uint id = 0; id < GetWorld()->GetAgents().size(); id++)
    {
        const AgentInterface *agent = GetWorld()->GetAgent(id);

        std::map<int, std::map<int, std::vector<std::pair<QString, QVariant>>>>::iterator agentIterator
            = agentData.find(id);

        if (agentIterator != agentData.end())
        {
            SaveTimeStepData(time, agent, agentIterator->second);
        }
        else
        {
            std::map<int, std::vector<std::pair<QString, QVariant>>> newAgentData;
            SaveTimeStepData(time, agent, newAgentData);
            agentData.emplace(std::make_pair(id, newAgentData));
        }
    }
}

void Observation_ScopeLogger_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    std::string sep = ";";
    std::string endLine = "\n";

    for (auto const &agent : agentData)
    {
        int agentId = agent.first;
        QString resultFileName = QString::fromStdString(resultFolder)
                                 + "/dataLog_"
                                 + QString::number(agentId)
                                 + ".csv";

        if (QFile::exists(resultFileName))
        {
            QFile::remove(resultFileName);
        }

        std::ofstream resultFile;

        resultFile.open(resultFileName.toStdString(),
                        std::ofstream::out | std::ofstream::app);

        if (!resultFile.is_open())
        {
            LOG(CbkLogLevel::Debug, "result File could not be opened");
            return;
        }

        //header
        resultFile << "Time";

        std::map<int, std::vector<std::pair<QString, QVariant>>> dataStrings =
            agentData.begin()->second;
        std::vector<std::pair<QString, QVariant>> dataVector = dataStrings.begin()->second;
        for (std::pair<QString, QVariant> &data : dataVector)
        {
            resultFile << sep << data.first.toStdString();
        }

        resultFile << endLine;

        //data
        for (auto const &timeStep : agent.second)
        {
            resultFile << timeStep.first;

            for (const std::pair<QString, QVariant> &dataVector : timeStep.second)
            {
                resultFile << sep << dataVector.second.toString().toStdString();
            }
            resultFile << endLine;
        }

        resultFile.close();
    }
}

void Observation_ScopeLogger_Implementation::SlavePostHook()
{

}

void Observation_ScopeLogger_Implementation::SaveTimeStepData(int time, const AgentInterface *agent,
                                                              std::map<int, std::vector<std::pair<QString, QVariant>>> &timeStepData)
{
    std::vector<std::pair<QString, QVariant>> dataVector;

    dataVector.push_back(std::make_pair("XPos", agent->GetPositionX()));
    dataVector.push_back(std::make_pair("YPos", agent->GetPositionY()));
    dataVector.push_back(std::make_pair("YawAngle", agent->GetYawAngle()));
    dataVector.push_back(std::make_pair("Velocity", agent->GetVelocityAbsolute()));
    dataVector.push_back(std::make_pair("AccelerationIntention", agent->GetAccelerationIntention()));
    dataVector.push_back(std::make_pair("DecelerationIntention", agent->GetDecelerationIntention()));
    dataVector.push_back(std::make_pair("AngleIntention", agent->GetAngleIntention()));
    dataVector.push_back(std::make_pair("CollisionOccured", agent->GetCollisionState()));

    timeStepData.emplace(std::make_pair(time, dataVector));

}
