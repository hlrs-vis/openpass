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
#include <stdlib.h>

#include <chrono>
#include <ctime>
#include <locale>

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
        Par_tmpXoscName = parameters->GetParametersString().at(3);
        Par_finalXoscName = parameters->GetParametersString().at(4);
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

    // Trajectory files
    tmpPath = Par_folder + "/" + Par_tmpXoscName;
    finalPath = Par_folder + "/" + std::to_string(runNumber) + "_" + Par_finalFilename;

    //Szenario files
    tmpXoscPath = Par_folder + "/" + Par_tmpFilename;
    finalXoscPath = Par_folder + "/" + std::to_string(runNumber) + "_" + Par_finalXoscName;

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

    // remove files in case of existence
    if(QFile::exists(QString::fromStdString(tmpPath)))
    {
        QFile::remove(QString::fromStdString(tmpPath));
    }

    if(QFile::exists(QString::fromStdString(finalPath)))
    {
        QFile::remove(QString::fromStdString(finalPath));
    }
    if(QFile::exists(QString::fromStdString(tmpXoscPath)))
    {
        QFile::remove(QString::fromStdString(tmpXoscPath));
    }

    if(QFile::exists(QString::fromStdString(finalXoscPath)))
    {
        QFile::remove(QString::fromStdString(finalXoscPath));
    }
    dateAttributeValue = getStrDate();


    runNumber = 0;
}

void Observation_Osc_Implementation::SlavePreRunHook()
{


    finalPath = Par_folder + "/" + std::to_string(runNumber) + "_" + Par_finalFilename;
    finalXoscPath = Par_folder + "/" + std::to_string(runNumber) + "_" + Par_finalXoscName;

    AgentLists.clear();

    if(QFile::exists(QString::fromStdString(finalPath)))
    {
        QFile::remove(QString::fromStdString(finalPath));
    }
    if(QFile::exists(QString::fromStdString(tmpXoscPath)))
    {
        QFile::remove(QString::fromStdString(tmpXoscPath));
    }

    if(QFile::exists(QString::fromStdString(finalXoscPath)))
    {
        QFile::remove(QString::fromStdString(finalXoscPath));
    }

    // Trajectory file
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
    fileStream->writeStartElement("OpenSCENARIO");

    fileStream->writeStartElement("FileHeader");
    fileStream->writeAttribute("revMajor", "0");
    fileStream->writeAttribute("revMinor", "0");
    fileStream->writeAttribute("date", dateAttributeValue);
    fileStream->writeAttribute("description", "Studienarbeit");
    fileStream->writeAttribute("author", "Christoph Kirsch");
    fileStream->writeEndElement();

    fileStream->writeStartElement("Catalog");
    CatalogNameAttributeValue = "tCatalog_Run_" + QString::number(runNumber);
    fileStream->writeAttribute("name", CatalogNameAttributeValue);

    // XOSC file
    xoscfile = std::make_shared<QFile>(QString::fromStdString(tmpXoscPath));
    if(!xoscfile->open(QIODevice::WriteOnly))
    {
        std::stringstream ss;
        ss << COMPONENTNAME << " could not create file: " << tmpXoscPath;
        LOG(CbkLogLevel::Error, ss.str());
        throw std::runtime_error(ss.str());
    }

    fileStreamXosc = std::make_shared<QXmlStreamWriter>(xoscfile.get());
    fileStreamXosc->setAutoFormatting(true);
    fileStreamXosc->writeStartDocument();

    fileStreamXosc->writeStartElement("OpenSCENARIO");
    fileStreamXosc->writeStartElement("FileHeader");
    fileStreamXosc->writeAttribute("author", "Christoph Kirsch");
    fileStreamXosc->writeAttribute("date", dateAttributeValue);
    fileStreamXosc->writeAttribute("description", "Studienarbeit");
    fileStreamXosc->writeAttribute("revMajor", "0");
    fileStreamXosc->writeAttribute("revMinor", "0");
    fileStreamXosc->writeEndElement(); // end File Header Tag

    fileStreamXosc->writeStartElement("ParameterDeclaration");
    fileStreamXosc->writeEndElement(); // end ParameterDeclaration


    //      Catalogs Definitions

    fileStreamXosc->writeStartElement("Catalogs");
    fileStreamXosc->writeStartElement(VehicleCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",VehicleCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(DriverCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",DriverCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(PedestrianCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",PedestrianCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(PedestrianControllerCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",PedestrianControllerCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(MiscObjectCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",MiscObjectCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(EnvironmentCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",EnvironmentCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(ManeuverCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",ManeuverCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(TrajectoryCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",TrajectoryCatalogTagValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(RouteCatalogTag);
    fileStreamXosc->writeStartElement("Directory");
    fileStreamXosc->writeAttribute("path",RouteCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();

    fileStreamXosc->writeStartElement("RoadNetwork");
    fileStreamXosc->writeStartElement("Logics");
    fileStreamXosc->writeAttribute("filepath",LogicsFilePathValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement("SceneGraph");
    fileStreamXosc->writeAttribute("filepath",SceneGraphFilepathValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();







}

void Observation_Osc_Implementation::SlaveUpdateHook(int time, RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    RecordAllAgents(time);
}

void Observation_Osc_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    Q_UNUSED(runResult);

    //      Define Agents (Entities)
    fileStreamXosc->writeStartElement("Entities");
    for(int agentID = 0; agentID<AgentLists.size(); agentID++){


        ObjectNameValue = "Agent" + QString::number(agentID);

        fileStreamXosc->writeStartElement("Object");
        fileStreamXosc->writeAttribute("name",ObjectNameValue);
        fileStreamXosc->writeStartElement("CatalogReference");
        fileStreamXosc->writeAttribute("catalogName",VehicleCatalogTag);
        fileStreamXosc->writeAttribute("entryName",getCarModel());
        fileStreamXosc->writeEndElement(); //EndCatalogRefernce
        fileStreamXosc->writeStartElement("Controller");
        fileStreamXosc->writeStartElement("CatalogReference");
        fileStreamXosc->writeAttribute("catalogName",DriverCatalogTag);
        fileStreamXosc->writeAttribute("entryName",driverModel);
        fileStreamXosc->writeEndElement(); //End CatalogRefernceTag
        fileStreamXosc->writeEndElement(); //End ControllerTag
        fileStreamXosc->writeEndElement(); //End ObjectTag

    }
    fileStreamXosc->writeEndElement(); //End EntitiesTag

    fileStreamXosc->writeStartElement("Storyboard");
    fileStreamXosc->writeStartElement("Init");
    fileStreamXosc->writeStartElement("Actions");

    for(int agentID = 0; agentID<AgentLists.size(); agentID++){
        std::list<VehicleState> agentData = AgentLists[agentID];
        VehicleState firstTimeStep = agentData.front();
        init_xPos = firstTimeStep.getxpos();
        init_yPos = firstTimeStep.getypos();

        ObjectNameValue = "Agent" + QString::number(agentID);

        fileStreamXosc->writeStartElement("Private");
        fileStreamXosc->writeAttribute("object",ObjectNameValue);
        fileStreamXosc->writeStartElement("Action");
        fileStreamXosc->writeStartElement("Longitudinal");
        fileStreamXosc->writeStartElement("Speed");
        fileStreamXosc->writeStartElement("Dynamics");
        fileStreamXosc->writeAttribute("rate","0");
        fileStreamXosc->writeAttribute("shape","step");
        fileStreamXosc->writeEndElement(); //End DynamicsTag
        fileStreamXosc->writeStartElement("Target");
        fileStreamXosc->writeStartElement("Absolute");
        fileStreamXosc->writeAttribute("value","0");
        fileStreamXosc->writeEndElement(); //End AbsoluteTag
        fileStreamXosc->writeEndElement(); //End TargetTag
        fileStreamXosc->writeEndElement(); //End SpeedTag
        fileStreamXosc->writeEndElement(); //End LongitudinalTag
        fileStreamXosc->writeEndElement(); //End ActionTag
        fileStreamXosc->writeStartElement("Action");
        fileStreamXosc->writeStartElement("Position");

        // intialize agents with absolute world coorinates
        fileStreamXosc->writeStartElement("World");
        fileStreamXosc->writeAttribute("x",QString::number(init_xPos));
        fileStreamXosc->writeAttribute("y",QString::number(init_yPos));
        fileStreamXosc->writeAttribute("z",QString::number(init_zPos));

        // intialize agent road relative
//        fileStreamXosc->writeStartElement("Lane");
//        fileStreamXosc->writeAttribute("laneId",laneIdValue);
//        fileStreamXosc->writeAttribute("offset",offsetValue);
//        fileStreamXosc->writeAttribute("roadId",roadIdValue);
//        fileStreamXosc->writeAttribute("s",QString::number(sValue));

        fileStreamXosc->writeEndElement(); //End LaneTag
        fileStreamXosc->writeEndElement(); //End "Position"
        fileStreamXosc->writeEndElement(); //End ActionTag
        fileStreamXosc->writeEndElement(); //End PrivateTag
    }
    fileStreamXosc->writeEndElement(); //End ActionsTag
    fileStreamXosc->writeEndElement(); //End initTag

    fileStreamXosc->writeStartElement("Story");
    fileStreamXosc->writeAttribute("name", StoryNameValue);

    for(int agentID = 0; agentID<AgentLists.size(); agentID++){
        TrajectoryNameAttributeValue = "Agent_" + QString::number(agentID) + "_Trajectory";

        std::list<VehicleState> agentData = AgentLists[agentID];
        VehicleState firstTimeStep = agentData.front();
        VehicleState lastTimeStep = agentData.back();
        SimulationTimeStart = firstTimeStep.gettime()+5;
        SimulationTimeEnd = lastTimeStep.gettime()+10; //lastTimeStep->gettime();

        // XOSC File
        EntityNameValue = "Agent" + QString::number(agentID);
        ActNameValue = "MyAct" + QString::number(agentID);
        SequenceNameValue = "MySequence" + QString::number(agentID);
        ManeuverNameValue = "laneChange" + QString::number(agentID);
        EventNameValue = "MyLaneChangeEvent" + QString::number(agentID);
        ActionNameValue = "MyLaneChangeAction" + QString::number(agentID);




        fileStreamXosc->writeStartElement("Act");
        fileStreamXosc->writeAttribute("name", ActNameValue);
        fileStreamXosc->writeStartElement("Sequence");
        fileStreamXosc->writeAttribute("name", SequenceNameValue);
        fileStreamXosc->writeAttribute("numberOfExecutions", "1");
        fileStreamXosc->writeStartElement("Actors");
        fileStreamXosc->writeStartElement("Entity");
        fileStreamXosc->writeAttribute("name", EntityNameValue);
        fileStreamXosc->writeEndElement(); //End EntityTag
        fileStreamXosc->writeEndElement(); //End ActorsTag
        fileStreamXosc->writeStartElement("Maneuver");
        fileStreamXosc->writeAttribute("name", ManeuverNameValue);
        fileStreamXosc->writeStartElement("Event");
        fileStreamXosc->writeAttribute("name", EventNameValue);
        fileStreamXosc->writeAttribute("priority", "overwrite");
        fileStreamXosc->writeStartElement("Action");
        fileStreamXosc->writeAttribute("name", ActionNameValue);
        fileStreamXosc->writeStartElement("Private");
        fileStreamXosc->writeStartElement("Routing");
        fileStreamXosc->writeStartElement("FollowTrajectory");
        fileStreamXosc->writeStartElement("CatalogReference");
        fileStreamXosc->writeAttribute("catalogName", TrajectoryCatalogTag);
        fileStreamXosc->writeAttribute("entryName", TrajectoryNameAttributeValue);
        fileStreamXosc->writeEndElement(); //End CatalogReference
        fileStreamXosc->writeStartElement("Longitudinal");
        fileStreamXosc->writeStartElement("None");
        fileStreamXosc->writeEndElement(); //End NoneTag
        fileStreamXosc->writeEndElement(); //End LongitudinalTag
        fileStreamXosc->writeStartElement("Lateral");
        fileStreamXosc->writeAttribute("purpose", "position");
        fileStreamXosc->writeEndElement(); //End LateralTag
        fileStreamXosc->writeEndElement(); //End FollowTrajectory
        fileStreamXosc->writeEndElement(); //End RoutingTag
        fileStreamXosc->writeEndElement(); //End Private
        fileStreamXosc->writeEndElement(); //End Action
        fileStreamXosc->writeStartElement("Conditions");
        fileStreamXosc->writeStartElement("Start");
        fileStreamXosc->writeStartElement("ConditionGroup");
        fileStreamXosc->writeStartElement("Condition");
        fileStreamXosc->writeAttribute("delay", "0");
        fileStreamXosc->writeAttribute("edge", "rising");
        fileStreamXosc->writeAttribute("name", "");
        fileStreamXosc->writeStartElement("ByValue");
        fileStreamXosc->writeStartElement("SimulationTime");
        fileStreamXosc->writeAttribute("rule", "equal-to");
        fileStreamXosc->writeAttribute("value", QString::number(SimulationTimeStart));
        fileStreamXosc->writeEndElement(); //End SimulationTimeTag
        fileStreamXosc->writeEndElement(); //End ByValueTag
        fileStreamXosc->writeEndElement(); //End ConditionTag
        fileStreamXosc->writeEndElement(); //End ConditionGroupTag
        fileStreamXosc->writeEndElement(); //End StartTag
        fileStreamXosc->writeEndElement(); //End ConditionsTag
        fileStreamXosc->writeEndElement(); //End EventTag
        fileStreamXosc->writeEndElement(); //End ManeuverTag

        // here ends the Trajectory Maneuver in the XOSC File
        // add additional Maneuver to break at the end of the Trajectory
        fileStreamXosc->writeStartElement("Maneuver");
        fileStreamXosc->writeAttribute("name", "break");
        fileStreamXosc->writeStartElement("Event");
        fileStreamXosc->writeAttribute("name", EventNameValue);
        fileStreamXosc->writeAttribute("priority", "overwrite");
        fileStreamXosc->writeStartElement("Action");
        fileStreamXosc->writeAttribute("name", ActionNameValue);
        fileStreamXosc->writeStartElement("Private");
        fileStreamXosc->writeStartElement("Longitudinal");
        fileStreamXosc->writeStartElement("Speed");
        fileStreamXosc->writeStartElement("Dynamics");
        fileStreamXosc->writeAttribute("distance","0");
        fileStreamXosc->writeAttribute("rate","0");
        fileStreamXosc->writeAttribute("shape","step");
        fileStreamXosc->writeAttribute("time","0");
        fileStreamXosc->writeEndElement(); //End DynamicsTag
        fileStreamXosc->writeStartElement("Target");
        fileStreamXosc->writeStartElement("Absolute");
        fileStreamXosc->writeAttribute("value","0");
        fileStreamXosc->writeEndElement(); //End AbsoluteTag
        fileStreamXosc->writeEndElement(); //End targetTag
        fileStreamXosc->writeEndElement(); //End SpeedTag
        fileStreamXosc->writeEndElement(); //End LongitudinalTag
        fileStreamXosc->writeEndElement(); //End PrivateTag
        fileStreamXosc->writeEndElement(); //End ActionTag
        // write Conditions of Break Maneuver
        fileStreamXosc->writeStartElement("Conditions");
        fileStreamXosc->writeStartElement("Start");
        fileStreamXosc->writeStartElement("ConditionGroup");
        fileStreamXosc->writeStartElement("Condition");
        fileStreamXosc->writeAttribute("delay", "0");
        fileStreamXosc->writeAttribute("edge", "rising");
        fileStreamXosc->writeAttribute("name", "");
        fileStreamXosc->writeStartElement("ByState");
        fileStreamXosc->writeStartElement("AfterTermination");
        fileStreamXosc->writeAttribute("name", ManeuverNameValue);
        fileStreamXosc->writeAttribute("rule", "end");
        fileStreamXosc->writeAttribute("type", "maneuver");
        fileStreamXosc->writeEndElement(); //End AfterTerminationTag
        fileStreamXosc->writeEndElement(); //End ByState
        fileStreamXosc->writeEndElement(); //End ConditionTag
        fileStreamXosc->writeEndElement(); //End ConditionGroupTag
        fileStreamXosc->writeEndElement(); //End StartTag
        fileStreamXosc->writeEndElement(); //End ConditionsTag
        fileStreamXosc->writeEndElement(); //End EventTag
        fileStreamXosc->writeEndElement(); //End ManeuverTag
        // end of break maneuver

        // write Start and End Conditions of Act
        fileStreamXosc->writeEndElement(); //End SequenceTag
        fileStreamXosc->writeStartElement("Conditions");
        fileStreamXosc->writeStartElement("Start");
        fileStreamXosc->writeStartElement("ConditionGroup");
        fileStreamXosc->writeStartElement("Condition");
        fileStreamXosc->writeAttribute("delay", "0");
        fileStreamXosc->writeAttribute("edge", "rising");
        fileStreamXosc->writeAttribute("name", "");
        fileStreamXosc->writeStartElement("ByValue");
        fileStreamXosc->writeStartElement("SimulationTime");
        fileStreamXosc->writeAttribute("rule", "equal-to");
        fileStreamXosc->writeAttribute("value", "5.0");
        fileStreamXosc->writeEndElement(); //End SimulationTimeTag
        fileStreamXosc->writeEndElement(); //End ByValueTag
        fileStreamXosc->writeEndElement(); //End ConditionTag
        fileStreamXosc->writeEndElement(); //End ConditionGroupTag
        fileStreamXosc->writeEndElement(); //End StartTag
        fileStreamXosc->writeStartElement("End");
        fileStreamXosc->writeStartElement("ConditionGroup");
        fileStreamXosc->writeStartElement("Condition");
        fileStreamXosc->writeAttribute("delay", "0");
        fileStreamXosc->writeAttribute("edge", "rising");
        fileStreamXosc->writeAttribute("name", "");
        fileStreamXosc->writeStartElement("ByValue");
        fileStreamXosc->writeStartElement("SimulationTime");
        fileStreamXosc->writeAttribute("rule", "equal-to");
        fileStreamXosc->writeAttribute("value", QString::number(SimulationTimeEnd));
        fileStreamXosc->writeEndElement(); //End SimulationTimeTag
        fileStreamXosc->writeEndElement(); //End ByValueTag
        fileStreamXosc->writeEndElement(); //End ConditionTag
        fileStreamXosc->writeEndElement(); //End ConditionGroupTag
        fileStreamXosc->writeEndElement(); //End Endtag
        fileStreamXosc->writeEndElement(); //End ConditionsTag
        fileStreamXosc->writeEndElement(); //End ActTag

        // Trajectory File
        fileStream->writeStartElement("Trajectory");
        fileStream->writeAttribute("name", TrajectoryNameAttributeValue);
        fileStream->writeAttribute("closed", "false");
        fileStream->writeAttribute("domain", "time");

        fileStream->writeStartElement("ParameterDeclaration");
        fileStream->writeEndElement(); // end ParameterDeclaration


        if(Par_WriteOutputForVisualisation)
        {
            std::stringstream ss;
            ss << COMPONENTNAME << " append log to file: " << tmpPath;
            LOG(CbkLogLevel::Debug, ss.str());

            // Agent Stories


            std::list<VehicleState>::iterator t; // iterator
            for(t = agentData.begin(); t != agentData.end(); ++t)
            {
                if (t == agentData.begin()) {
                    continue;
                }
                time = t->gettime();
                x = t->getxpos();
                y = t->getypos();
                yaw = t->getyawangle();

                // write Trajectory Coordinates to XML
                fileStream->writeStartElement("Vertex");
                fileStream->writeStartElement("Position");
                fileStream->writeStartElement("World");
                fileStream->writeAttribute("x", QString::number(x));
                fileStream->writeAttribute("y", QString::number(y));
                fileStream->writeAttribute("z", "0");
                fileStream->writeAttribute("h", QString::number(yaw));
                fileStream->writeAttribute("p", "0");
                fileStream->writeAttribute("r", "0");
                fileStream->writeEndElement();

                fileStream->writeEndElement(); //end Position

                fileStream->writeStartElement("Shape");
                fileStream->writeAttribute("reference",QString::number(time));

                fileStream->writeStartElement("Polyline");
                fileStream->writeEndElement(); //end Polyline

                fileStream->writeEndElement(); //end shape
                fileStream->writeEndElement(); //end vertex



            }

            fileStream->writeEndElement(); //end Trajectory



        }


    }
    // continue with XOSC File
    fileStreamXosc->writeEndElement(); //End StoryTag

    fileStreamXosc->writeStartElement("End");
    fileStreamXosc->writeStartElement("ConditionGroup");
    fileStreamXosc->writeStartElement("Condition");
    fileStreamXosc->writeAttribute("delay", "0");
    fileStreamXosc->writeAttribute("edge", "rising");
    fileStreamXosc->writeAttribute("name", "");
    fileStreamXosc->writeStartElement("ByValue");
    fileStreamXosc->writeStartElement("SimulationTime");
    fileStreamXosc->writeAttribute("rule", "equal-to");
    fileStreamXosc->writeAttribute("value", "500.0");
    fileStreamXosc->writeEndElement(); //End SimulationTimeTag
    fileStreamXosc->writeEndElement(); //End ByValueTag
    fileStreamXosc->writeEndElement(); //End ConditionTag
    fileStreamXosc->writeEndElement(); //End ConditionGroupTag
    fileStreamXosc->writeEndElement(); //End EndTag
    fileStreamXosc->writeEndElement(); //End StoryboardTag
    fileStreamXosc->writeEndElement(); //End OpenScenarioTag

    fileStreamXosc->writeEndDocument();
    xoscfile->flush();
    xoscfile->close();

    xoscfile->rename(QString::fromStdString(finalXoscPath));

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

    VehicleState AgentX(agentXPos, agentYPos, agentYAwangle, time);
    if((agentId+1) > AgentLists.size()){
        AgentLists.resize(agentId+1);
    }
    AgentLists[agentId].push_back(AgentX);

}

QString Observation_Osc_Implementation::getStrDate(){
    // create Date
    auto datenow = std::chrono::system_clock::now();
    std::time_t date = std::chrono::system_clock::to_time_t(datenow);
    auto ds = std::localtime(&date);

    //2016-10-18T10:00:00
    QString dateAttributeValue_temp = QString::number(ds->tm_year+1900) + "-" + QString::number(ds->tm_mon+1) + "-" + QString::number(ds->tm_mday) + "T" + QString::number(ds->tm_hour) + ":" +QString::number(ds->tm_min) + ":" + QString::number(ds->tm_sec);
    return dateAttributeValue_temp;
}

QString Observation_Osc_Implementation::getCarModel(){
    // pick random car from CarModelList
    randomCarModelIndex = rand() % CarModelList.size();
    CarModel = CarModelList[randomCarModelIndex];
    return CarModel;
}



