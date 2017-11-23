/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <cassert>
#include <sstream>
#include <QDir>
#include <iostream>
#include <fstream>
#include "observation_osc_implementation.h"
#include <list>

const std::string Observation_Osc_Implementation::PeriodicTypeStrings[] =
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

Observation_Osc_Implementation::Observation_Osc_Implementation(StochasticsInterface *stochastics,
                                                               WorldInterface *world,
                                                               const ParameterInterface *parameters,
                                                               const CallbackInterface *callbacks) :
    ObservationInterface(stochastics, world, parameters, callbacks)
{
    // read parameters
    try
    {

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

void Observation_Osc_Implementation::SlavePreHook(const std::string &path)
{
    Q_UNUSED(path);

    for(int runNumber = 0; runNumber<10;runNumber++){
    //for(int runNumber = 0; runNumber < runConfig->GetNumberInvocations(); ++runNumber){

        // create run folder (./Run_X)
        Run_folder = Par_folder + "/Run_" + std::to_string(runNumber);
        QDir rundir(Run_folder.c_str());

        if(!rundir.exists() && !rundir.mkpath("."))
        {
            std::stringstream ss;
            ss << COMPONENTNAME << " could not create folder: " << Run_folder;
            LOG(CbkLogLevel::Error, ss.str());
            throw std::runtime_error(ss.str());
        }

        for(int agentID = 0; agentID<10; agentID++){
            tmpPath = Run_folder + "/agID_" + std::to_string(agentID) + "_" + Par_tmpFilename;
            finalPath = Run_folder + "/agID_" + std::to_string(agentID) + "_" + Par_finalFilename;

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
        }
    }
}

void Observation_Osc_Implementation::SlavePreRunHook()
{
    timeChannel.clear();
    channels.clear();
    agentsPositionX.clear();

    Run_folder = Par_folder + "/Run_" + std::to_string(runNumber);

    for(int agentID = 0; agentID<10; agentID++){
        tmpPath = Run_folder + "/agID_" + std::to_string(agentID) + "_" + Par_tmpFilename;
        finalPath = Run_folder + "/agID_" + std::to_string(agentID) + "_" + Par_finalFilename;



        if(QFile::exists(QString::fromStdString(tmpPath)))
        {
            QFile::remove(QString::fromStdString(tmpPath));
        }

        if(QFile::exists(QString::fromStdString(finalPath)))
        {
            QFile::remove(QString::fromStdString(finalPath));
        }

    }


}

void Observation_Osc_Implementation::SlaveUpdateHook(int time, RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    RecordAllAgents(time);
}

void Observation_Osc_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    Run_folder = Par_folder + "/Run_" + std::to_string(runNumber);

    for(int agentID = 0; agentID<AgentLists.size(); agentID++){
        tmpPath = Run_folder + "/agID_" + std::to_string(agentID) + "_" + Par_tmpFilename;
        finalPath = Run_folder + "/agID_" + std::to_string(agentID) + "_" + Par_finalFilename;

        std::list<VehicleState> agentData = AgentLists[agentID];

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
        fileStream->writeStartElement(OpenSCENARIOTag);

        fileStream->writeStartElement(FileHeaderTag);
        fileStream->writeAttribute(revMajorAttribute, revMajorAttributeValue);
        fileStream->writeAttribute(revMajorAttribute, revMajorAttributeValue);
        fileStream->writeAttribute(dateAttribute, dateAttributeValue);
        fileStream->writeAttribute(descriptionAttribute, descriptionAttributeValue);
        fileStream->writeAttribute(authorAttribute, authorAttributeValue);
        fileStream->writeEndElement();

        fileStream->writeStartElement(CatalogTag);
        fileStream->writeAttribute(CatalogNameAttribute, CatalogNameAttributeValue);

        fileStream->writeStartElement(TrajectoryTag);

        fileStream->writeStartElement(ParameterDeclarationTag);

        fileStream->writeStartElement(ParameterTag);
        fileStream->writeAttribute(ParameterNameAttribute, "name151");
        fileStream->writeAttribute(ParameterTypeAttribute, "int");
        fileStream->writeAttribute(ParameterValueAttribute, "value151");
        fileStream->writeEndElement(); // end Parameter
        fileStream->writeStartElement(ParameterTag);
        fileStream->writeAttribute(ParameterNameAttribute, "name152");
        fileStream->writeAttribute(ParameterTypeAttribute, "double");
        fileStream->writeAttribute(ParameterValueAttribute, "value151");
        fileStream->writeEndElement(); // end Parameter
        fileStream->writeStartElement(ParameterTag);
        fileStream->writeAttribute(ParameterNameAttribute, "name153");
        fileStream->writeAttribute(ParameterTypeAttribute, "String");
        fileStream->writeAttribute(ParameterValueAttribute, "value153");
        fileStream->writeEndElement(); // end Parameter


        fileStream->writeEndElement(); // end ParameterDeclaration


        if(Par_WriteOutputForVisualisation)
        {
            std::stringstream ss;
            ss << COMPONENTNAME << " append log to file: " << tmpPath;
            LOG(CbkLogLevel::Debug, ss.str());


            // write VertexTag
            //std::vector<double> values = agentsPositionX->second;
            //                std::vector<std::string> XPos = channels.at(agentIndexX);
            //                std::vector<std::string> YPos = channels.at(agentIndexY);

            std::list<VehicleState>::iterator t;
            for(t = agentData.begin(); t != agentData.end(); ++t)
            {
                double x = t->getxpos();
                double y = t->getypos();

                double yaw = t->getyawangle();

                double time = t->gettime();

                fileStream->writeStartElement(VertexTag);
                fileStream->writeStartElement(PositionTag);
                fileStream->writeStartElement(WorldTag);
                fileStream->writeAttribute("x", QString::number(x));
                fileStream->writeAttribute("y", QString::number(y));
                fileStream->writeAttribute("z", "0");
                fileStream->writeAttribute("h", QString::number(yaw));
                fileStream->writeAttribute("p", "0");
                fileStream->writeAttribute("r", "0");
                fileStream->writeEndElement();

                fileStream->writeEndElement(); //end Position

                fileStream->writeStartElement(ShapeTag);
                fileStream->writeAttribute(referenceAttribute,QString::number(time));

                fileStream->writeStartElement(PolylineTag);
                fileStream->writeEndElement(); //end Polyline

                fileStream->writeEndElement(); //end shape
                fileStream->writeEndElement(); //end vertex



            }

            fileStream->writeEndElement(); //end Trajectory
            fileStream->writeEndElement(); //end Catalog
            fileStream->writeEndElement(); //end OpenSCENARIO-Header


        }
        fileStream->writeEndDocument();
        file->flush();
        file->close();

        // finalize results
        file->rename(QString::fromStdString(finalPath));

    }

    //Write Agents position to csv
    WriteAgentPositionsToCSV();
    runNumber++;
}

void Observation_Osc_Implementation::SlavePostHook()
{

}

void Observation_Osc_Implementation::RecordAllAgents(int time)
{
    for (const auto &it: GetWorld()->GetAgents()){
        const AgentInterface *agent= it.second;
        RecordAgentState(time, agent);
    }
}

void Observation_Osc_Implementation::RecordAgentState(int time, const AgentInterface *agent)
{
    int agentId = agent->GetAgentId();
    double agentXPos = agent->GetPositionX();
    double agentYPos = agent->GetPositionY();
    double agentYAwangle = agent->GetYawAngle();

    //VehicleState *AgentX = new VehicleState(agentXPos, agentYPos, agentYAwangle, time);
    VehicleState AgentX(agentXPos, agentYPos, agentYAwangle, time);
    if((agentId+1) > AgentLists.size()){
        AgentLists.resize(agentId+1);
    }
    AgentLists[agentId].push_back(AgentX);


    AddPositionXForCSV(agentId, time, agent->GetPositionX());


    // Position in Map einfügen
    Insert(time, agentId, Observation_Osc_Periodic_XPosition, std::to_string(agent->GetPositionX()));
    Insert(time, agentId, Observation_Osc_Periodic_YPosition, std::to_string(agent->GetPositionY()));
}

void Observation_Osc_Implementation::Insert(int time, int agentId, Observation_Osc_Periodic_Type valueType, const std::string &value)
{
    //hier wird Map mit Korrdinaten erstellt (?)
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

void Observation_Osc_Implementation::AddPositionXForCSV(int agentId, int time, double positionX)
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

void Observation_Osc_Implementation::WriteAgentPositionsToCSV()
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

double VehicleState::getxpos(){
    return xpos;
}
double VehicleState::getypos(){
    return ypos;
}
double VehicleState::getyawangle(){
    return yawangle;
}
double VehicleState::gettime(){
    return time;
}
