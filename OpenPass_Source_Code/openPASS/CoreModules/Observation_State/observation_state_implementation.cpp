/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <cassert>
#include <sstream>
#include <QDir>
#include <iostream>
#include <fstream>
#include "observation_state_implementation.h"

const std::string Observation_State_Implementation::PeriodicTypeStrings[] =
{
    "PositionRoute",
    "Lane",
    "BrakeLight",
    "XPosition",
    "YPosition",
    "YawAngle",
    "Alpha",
    "TBrake",
    "VelocityEgo",
    "VelocityFront",
    "AccelerationEgo",
    "AccelerationFront",
    "DriverStateEgo",
    "DistanceToVehicleFront",
    "TTCToVehicleFront",
    "TimeToVehicleFront",
    "DeltaVToVehicleFront",
    "Attentive"
};

Observation_State_Implementation::Observation_State_Implementation(StochasticsInterface *stochastics,
                                                               WorldInterface *world,
                                                               const ParameterInterface *parameters,
                                                               const CallbackInterface *callbacks) :
    ObservationInterface(stochastics, world, parameters, callbacks)
{
    // read parameters
    try
    {
        Par_WriteOutputForVisualisation = parameters->GetParametersBool().at(0);

        // retrieve storage location
        Par_folder = parameters->GetParametersString().at(0);
        Par_tmpFilename = parameters->GetParametersString().at(1);
        Par_finalFilename = parameters->GetParametersString().at(2);
    }
    catch(...)
    {
        const std::string msg = COMPONENTNAME + " could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}

void Observation_State_Implementation::SlavePreHook(const std::string &path)
{
    Q_UNUSED(path);

    tmpPath = Par_folder + "/" + Par_tmpFilename;
    finalPath = Par_folder + "/" + std::to_string(runNumber) + "_" + Par_finalFilename;

    std::stringstream ss;
    ss << COMPONENTNAME << " retrieved storage location: " << finalPath;
    LOG(CbkLogLevel::Debug, ss.str());

    // setup environment
    QDir dir(Par_folder.c_str());
    if(!dir.exists() && !dir.mkpath(Par_folder.c_str()))
    {
        std::stringstream ss;
        ss << COMPONENTNAME << " could not create folder: " << Par_folder;
        LOG(CbkLogLevel::Error, ss.str());
        throw std::runtime_error(ss.str());
    }

    if(QFile::exists(QString::fromStdString(tmpPath)))
    {
        QFile::remove(QString::fromStdString(tmpPath));
    }

    if(QFile::exists(QString::fromStdString(finalPath)))
    {
        QFile::remove(QString::fromStdString(finalPath));
    }

    //---------------------------------------------------------
    resultPathCSV = Par_folder + "/" + resultFileCSV;

    if(QFile().exists(QString::fromStdString(resultPathCSV))){
        QFile::remove(QString::fromStdString(resultPathCSV));
    }

    runNumber = 0;
}

void Observation_State_Implementation::SlavePreRunHook()
{
    timeChannel.clear();
    channels.clear();
    agentsPositionX.clear();
    finalPath = Par_folder + "/" + std::to_string(runNumber) + "_" + Par_finalFilename;

    if(QFile::exists(QString::fromStdString(tmpPath)))
    {
        QFile::remove(QString::fromStdString(tmpPath));
    }

    if(QFile::exists(QString::fromStdString(finalPath)))
    {
        QFile::remove(QString::fromStdString(finalPath));
    }

    file = std::make_shared<QFile>(QString::fromStdString(tmpPath));
    if(!file->open(QIODevice::WriteOnly))
    {
        std::stringstream ss;
        ss << COMPONENTNAME << " could not create file: " << tmpPath;
        LOG(CbkLogLevel::Error, ss.str());
        throw std::runtime_error(ss.str());
    }

    fileStream = std::make_shared<QXmlStreamWriter>(file.get());
    fileStream->setAutoFormatting(true);
    fileStream->writeStartDocument();
    fileStream->writeStartElement(SimulationOutputTag);
    fileStream->writeStartElement(RunResultsTag);
}

void Observation_State_Implementation::SlaveUpdateHook(int time, RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    RecordAllAgents(time);
}

void Observation_State_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    fileStream->writeStartElement(RunResultTag);
    fileStream->writeAttribute(RunIdAttribute, QString::number(runNumber));

    if(Par_WriteOutputForVisualisation)
    {
        std::stringstream ss;
        ss << COMPONENTNAME << " append log to file: " << tmpPath;
        LOG(CbkLogLevel::Debug, ss.str());

        // write CyclicsTag
        fileStream->writeStartElement(CyclicsTag);

        // write header
        fileStream->writeStartElement(HeaderTag);

        std::stringstream headerLine;
        for(std::map<std::string, std::vector<std::string>>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if(it != channels.begin())
            {
                headerLine << ", ";
            }

            headerLine << it->first;
        }

        fileStream->writeCharacters(QString::fromStdString(headerLine.str()));

        // close HeaderTag
        fileStream->writeEndElement();

        // write SamplesTag
        fileStream->writeStartElement(SamplesTag);
        for(unsigned int timeIndex = 0; timeIndex < timeChannel.size(); ++timeIndex)
        {
            std::stringstream sampleLine;
            for(std::map<std::string, std::vector<std::string>>::iterator it = channels.begin(); it != channels.end(); ++it)
            {
                std::vector<std::string> values = it->second;

                if(it != channels.begin())
                {
                    sampleLine << ", ";
                }

                // not all channels are sampled until end of simulation time
                if(timeIndex < values.size())
                {
                    sampleLine << values.at(timeIndex);
                }
            }

            fileStream->writeStartElement(SampleTag);
            fileStream->writeAttribute(TimeAttribute, QString::number(timeChannel.at(timeIndex)));
            fileStream->writeCharacters(QString::fromStdString(sampleLine.str()));

            // close SampleTag
            fileStream->writeEndElement();
        }

        // close SamplesTag
        fileStream->writeEndElement();

        // close CyclicsTag
        fileStream->writeEndElement();
    }
    // close RunResultTag
    fileStream->writeEndElement();

    // close RunResultsTag
    fileStream->writeEndElement();

    // close SimulationOutputTag
    fileStream->writeEndElement();

    fileStream->writeEndDocument();
    file->flush();
    file->close();

    // finalize results
    file->rename(QString::fromStdString(finalPath));

    //Write Agents position to csv
    WriteAgentPositionsToCSV();

    ++runNumber;
}

void Observation_State_Implementation::SlavePostHook()
{

}

void Observation_State_Implementation::RecordAllAgents(int time)
{
    for (const auto &it: GetWorld()->GetAgents()){
        const AgentInterface *agent= it.second;
        RecordAgentState(time, agent);
    }
}

void Observation_State_Implementation::RecordAgentState(int time, const AgentInterface *agent)
{
    int agentId = agent->GetAgentId();

    AddPositionXForCSV(agentId, time, agent->GetPositionX());

    Insert(time, agentId, Observation_State_Periodic_XPosition, std::to_string(agent->GetPositionX()));
    Insert(time, agentId, Observation_State_Periodic_YPosition, std::to_string(agent->GetPositionY()));
    Insert(time, agentId, Observation_State_Periodic_VelocityEgo, std::to_string(agent->GetVelocityX()));
    Insert(time, agentId, Observation_State_Periodic_AccelerationEgo, std::to_string(agent->GetAccelerationX()));
    Insert(time, agentId, Observation_State_Periodic_YawAngle, std::to_string(agent->GetYawAngle()));
    Insert(time, agentId, Observation_State_Periodic_PositionRoute, std::to_string(agent->GetRoadPosition().s));
    Insert(time, agentId, Observation_State_Periodic_BrakeLight, std::to_string(agent->GetBrakeLight()));
}

void Observation_State_Implementation::Insert(int time, int agentId, Observation_State_Periodic_Type valueType, const std::string &value)
{
    if(timeChannel.size() == 0 || timeChannel.back() != time)
    {
        timeChannel.push_back(time);
    }

    int keyIndex = static_cast<int>(valueType);
    std::string key = (agentId < 10 ? "0" : "") + std::to_string(agentId) + ":" + PeriodicTypeStrings[keyIndex];


    if(channels.find(key) == channels.end())
    {// key doesn't exist yet

        std::vector<std::string> &values = channels[key];

        // fill up skipped time steps
        for(unsigned int i = 0; i < timeChannel.size() - 1; ++i)
        {
            values.push_back("");
        }

        values.push_back(value);

        return;
    }

    std::vector<std::string> &channel = channels.at(key);

    // fill up skipped time steps (e.g. another agent has been instantiated in between -> inserted new time step in scheduling)
    for(unsigned int i = channel.size(); i < timeChannel.size() - 1; ++i)
    {
        channel.push_back("");
    }

    channel.push_back(value);
}

void Observation_State_Implementation::AddPositionXForCSV(int agentId, int time, double positionX)
{
    std::map<int, std::map<int, double>>::iterator agentIterator = agentsPositionX.find(agentId);
    if(agentIterator != agentsPositionX.end()){
        agentIterator->second.emplace(time, positionX);
    }else{
        std::map<int, double> agentPosition;
        agentPosition.emplace(std::make_pair(time, positionX));
        agentsPositionX.emplace(std::make_pair(agentId, agentPosition));
    }
}

void Observation_State_Implementation::WriteAgentPositionsToCSV()
{
    std::ofstream resultFile;
    resultFile.open(resultPathCSV, std::ofstream::out | std::ofstream::app);

    if(!resultFile.is_open())
    {
        LOG(CbkLogLevel::Debug, "result File could not be opened");
        return;
    }
    std::string sep = ";";
    std::string endLine = "\n";

    //header
    resultFile  << "RunID" << sep
                << "AgentID" << sep;

    // write in fist line all time steps
    for(uint i = 0; i < timeChannel.size(); ++i){
        resultFile << timeChannel.at(i) << sep;
    }
    resultFile << endLine;

    resultFile << runNumber; // write runID in first column
    //data
    for(auto const &agentID : agentsPositionX){
        resultFile << sep; // leave first column always empty (reserved for runID)
        resultFile << agentID.first << sep; //write ID of agent
        for(uint i = 0; i < timeChannel.size(); ++i){ //write all positions at every specific time step
            std::map<int, double> agentPositions = agentID.second;
            std::map<int, double>::iterator it = agentPositions.find(timeChannel.at(i));
            if(it != agentPositions.end() ){// if position is found at specific time step, then write it
                resultFile << it->second;
            }else{
                resultFile << 0;
            }
            resultFile << sep;
        }
        resultFile << endLine;
    }
    resultFile << endLine;

    resultFile.close();
}
