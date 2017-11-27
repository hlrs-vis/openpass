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



    runNumber = 0;
}

void Observation_Osc_Implementation::SlavePreRunHook()
{


    finalPath = Par_folder + "/" + std::to_string(runNumber) + "_" + Par_finalFilename;

    AgentLists.clear();

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

    // Start writing XML
    fileStream->writeStartElement(OpenSCENARIOTag);

    fileStream->writeStartElement(FileHeaderTag);
    fileStream->writeAttribute(revMajorAttribute, revMajorAttributeValue);
    fileStream->writeAttribute(revMajorAttribute, revMajorAttributeValue);
    fileStream->writeAttribute(dateAttribute, dateAttributeValue);
    fileStream->writeAttribute(descriptionAttribute, descriptionAttributeValue);
    fileStream->writeAttribute(authorAttribute, authorAttributeValue);
    fileStream->writeEndElement();

    fileStream->writeStartElement(CatalogTag);
    fileStream->writeAttribute(CatalogNameAttribute, "tCatalog_Run_" + QString::number(runNumber));
}

void Observation_Osc_Implementation::SlaveUpdateHook(int time, RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    RecordAllAgents(time);
}

void Observation_Osc_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    Q_UNUSED(runResult);


    for(int agentID = 0; agentID<AgentLists.size(); agentID++){


        std::list<VehicleState> agentData = AgentLists[agentID];



        //TrajectoryNameAttributeValue = "Agent_" + QString::number(agentID) + "_Trajectory";
        fileStream->writeStartElement(TrajectoryTag);
        fileStream->writeAttribute(TrajectoryNameAttribute, "Agent_" + QString::number(agentID) + "_Trajectory");

        fileStream->writeStartElement(ParameterDeclarationTag);
        fileStream->writeEndElement(); // end ParameterDeclaration


        if(Par_WriteOutputForVisualisation)
        {
            std::stringstream ss;
            ss << COMPONENTNAME << " append log to file: " << tmpPath;
            LOG(CbkLogLevel::Debug, ss.str());



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



        }


    }
    fileStream->writeEndElement(); //end Catalog
    fileStream->writeEndElement(); //end OpenSCENARIO-Header

    // finalize results
    fileStream->writeEndDocument();
    file->flush();
    file->close();

    file->rename(QString::fromStdString(finalPath));
    ++runNumber;
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
