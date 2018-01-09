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
    // create Date
    auto datenow = std::chrono::system_clock::now();
    std::time_t date = std::chrono::system_clock::to_time_t(datenow);
    auto ds = std::localtime(&date);

    //2016-10-18T10:00:00
    dateAttributeValue = QString::number(ds->tm_year+1900) + "-" + QString::number(ds->tm_mon+1) + "-" + QString::number(ds->tm_mday) + "T" + QString::number(ds->tm_hour) + ":" +QString::number(ds->tm_min) + ":" + QString::number(ds->tm_sec);



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
    fileStream->writeStartElement(OpenSCENARIOTag);

    fileStream->writeStartElement(FileHeaderTag);
    fileStream->writeAttribute(revMajorAttribute, revMajorAttributeValue);
    fileStream->writeAttribute(revMinorAttribute, revMinorAttributeValue);
    fileStream->writeAttribute(dateAttribute, dateAttributeValue);
    fileStream->writeAttribute(descriptionAttribute, descriptionAttributeValue);
    fileStream->writeAttribute(authorAttribute, authorAttributeValue);
    fileStream->writeEndElement();

    fileStream->writeStartElement(CatalogTag);
    CatalogNameAttributeValue = "tCatalog_Run_" + QString::number(runNumber);
    fileStream->writeAttribute(CatalogNameAttribute, CatalogNameAttributeValue);

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

    fileStreamXosc->writeStartElement(OpenSCENARIOTag);
    fileStreamXosc->writeStartElement(FileHeaderTag);
    fileStreamXosc->writeAttribute(authorAttribute, authorAttributeValue);
    fileStreamXosc->writeAttribute(dateAttribute, dateAttributeValue);
    fileStreamXosc->writeAttribute(descriptionAttribute, descriptionAttributeValue);
    fileStreamXosc->writeAttribute(revMajorAttribute, revMajorAttributeValue);
    fileStreamXosc->writeAttribute(revMinorAttribute, revMinorAttributeValue);
    fileStreamXosc->writeEndElement(); // end File Header Tag

    fileStreamXosc->writeStartElement(ParameterDeclarationTag);
    fileStreamXosc->writeEndElement(); // end ParameterDeclaration


    //      Catalogs Definitions

    fileStreamXosc->writeStartElement(CatalogsTag);
    fileStreamXosc->writeStartElement(VehicleCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,VehicleCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(DriverCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,DriverCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(PedestrianCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,PedestrianCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(PedestrianControllerCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,PedestrianControllerCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(MiscObjectCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,MiscObjectCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(EnvironmentCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,EnvironmentCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(ManeuverCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,ManeuverCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(TrajectoryCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,TrajectoryCatalogTagValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(RouteCatalogTag);
    fileStreamXosc->writeStartElement(DirectoryTag);
    fileStreamXosc->writeAttribute(DirectoryAttribute,RouteCatalogValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeEndElement();

    fileStreamXosc->writeStartElement(RoadNetworkTag);
    fileStreamXosc->writeStartElement(LogicsTag);
    fileStreamXosc->writeAttribute(filepathAttribute,LogicsFilePathValue);
    fileStreamXosc->writeEndElement();
    fileStreamXosc->writeStartElement(SceneGraphTag);
    fileStreamXosc->writeAttribute(filepathAttribute,SceneGraphFilepathValue);
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
    fileStreamXosc->writeStartElement(EntitiesTag);
    for(int agentID = 0; agentID<AgentLists.size(); agentID++){

        randomCarModelIndex = rand() % CarModel.size();

        ObjectNameValue = "Agent" + QString::number(agentID);

        fileStreamXosc->writeStartElement(ObjectTag);
        fileStreamXosc->writeAttribute(NameAttribute,ObjectNameValue);
        fileStreamXosc->writeStartElement(CatalogReferenceTag);
        fileStreamXosc->writeAttribute(CatalogReferenceAttribute,VehicleCatalogTag);
        fileStreamXosc->writeAttribute(entryNameAttribute,CarModel[randomCarModelIndex]);
        fileStreamXosc->writeEndElement(); //EndCatalogRefernce
        fileStreamXosc->writeStartElement(ControllerTag);
        fileStreamXosc->writeStartElement(CatalogReferenceTag);
        fileStreamXosc->writeAttribute(CatalogReferenceAttribute,DriverCatalogTag);
        fileStreamXosc->writeAttribute(entryNameAttribute,driverModel);
        fileStreamXosc->writeEndElement(); //End CatalogRefernceTag
        fileStreamXosc->writeEndElement(); //End ControllerTag
        fileStreamXosc->writeEndElement(); //End ObjectTag

    }
    fileStreamXosc->writeEndElement(); //End EntitiesTag

    fileStreamXosc->writeStartElement(StoryboardTag);
    fileStreamXosc->writeStartElement(InitTag);
    fileStreamXosc->writeStartElement(ActionsTag);

    for(int agentID = 0; agentID<AgentLists.size(); agentID++){
        ObjectNameValue = "Agent" + QString::number(agentID);

        fileStreamXosc->writeStartElement(PrivteTag);
        fileStreamXosc->writeAttribute(objectAttribute,ObjectNameValue);
        fileStreamXosc->writeStartElement(ActionTag);
        fileStreamXosc->writeStartElement(LongitudinalTag);
        fileStreamXosc->writeStartElement(SpeedTag);
        fileStreamXosc->writeStartElement(DynamicsTag);
        fileStreamXosc->writeAttribute(rateAttribute,DynamicsRateValue);
        fileStreamXosc->writeAttribute(shapeAttribute,DynamicsShapeValue);
        fileStreamXosc->writeEndElement(); //End DynamicsTag
        fileStreamXosc->writeStartElement(TargetTag);
        fileStreamXosc->writeStartElement(AbsoluteTag);
        fileStreamXosc->writeAttribute(valueAttribute,AbsoluteValueValue);
        fileStreamXosc->writeEndElement(); //End AbsoluteTag
        fileStreamXosc->writeEndElement(); //End TargetTag
        fileStreamXosc->writeEndElement(); //End SpeedTag
        fileStreamXosc->writeEndElement(); //End LongitudinalTag
        fileStreamXosc->writeEndElement(); //End ActionTag
        fileStreamXosc->writeStartElement(ActionTag);
        fileStreamXosc->writeStartElement(PositionTag);
        fileStreamXosc->writeStartElement(LaneTag);
        fileStreamXosc->writeAttribute(laneIDAttribue,laneIdValue);
        fileStreamXosc->writeAttribute(offsetAttribute,offsetValue);
        fileStreamXosc->writeAttribute(roadIdAttribute,roadIdValue);
        fileStreamXosc->writeAttribute(sAttribute,sValue);
        fileStreamXosc->writeEndElement(); //End LaneTag
        fileStreamXosc->writeEndElement(); //End PositionTag
        fileStreamXosc->writeEndElement(); //End ActionTag
        fileStreamXosc->writeEndElement(); //End PrivateTag
    }
    fileStreamXosc->writeEndElement(); //End ActionsTag
    fileStreamXosc->writeEndElement(); //End initTag

    fileStreamXosc->writeStartElement(StoryTag);
    fileStreamXosc->writeAttribute(NameAttribute, StoryNameValue);

    for(int agentID = 0; agentID<AgentLists.size(); agentID++){
        TrajectoryNameAttributeValue = "Agent_" + QString::number(agentID) + "_Trajectory";



        std::list<VehicleState> agentData = AgentLists[agentID];
        std::list<VehicleState>::iterator firstTimeStep = agentData.begin();
        std::list<VehicleState>::iterator lastTimeStep = agentData.end();
        SimulationTimeStart = firstTimeStep->gettime()+5;
        SimulationTimeEnd = 400; //lastTimeStep->gettime();

        // XOSC File
        EntityNameValue = "Agent" + QString::number(agentID);
        ActNameValue = "MyAct" + QString::number(agentID);
        SequenceNameValue = "MySequence" + QString::number(agentID);
        ManeuverNameValue = "laneChange" + QString::number(agentID);
        EventNameValue = "MyLaneChangeEvent" + QString::number(agentID);
        ActionNameValue = "MyLaneChangeAction" + QString::number(agentID);




        fileStreamXosc->writeStartElement(ActTag);
        fileStreamXosc->writeAttribute(NameAttribute, ActNameValue);
        fileStreamXosc->writeStartElement(SequenceTag);
        fileStreamXosc->writeAttribute(NameAttribute, SequenceNameValue);
        fileStreamXosc->writeAttribute(numOfExecAttribute, numOfExecValue);
        fileStreamXosc->writeStartElement(ActorsTag);
        fileStreamXosc->writeStartElement(EntityTag);
        fileStreamXosc->writeAttribute(NameAttribute, EntityNameValue);
        fileStreamXosc->writeEndElement(); //End EntityTag
        fileStreamXosc->writeEndElement(); //End ActorsTag
        fileStreamXosc->writeStartElement(ManeuverTag);
        fileStreamXosc->writeAttribute(NameAttribute, ManeuverNameValue);
        fileStreamXosc->writeStartElement(EventTag);
        fileStreamXosc->writeAttribute(NameAttribute, EventNameValue);
        fileStreamXosc->writeAttribute(EventPrioAttribute, EventPrioValue);
        fileStreamXosc->writeStartElement(ActionTag);
        fileStreamXosc->writeAttribute(NameAttribute, ActionNameValue);
        fileStreamXosc->writeStartElement(PrivteTag);
        fileStreamXosc->writeStartElement(RoutingTag);
        fileStreamXosc->writeStartElement(FollowTrajectoryTag);
        fileStreamXosc->writeStartElement(CatalogReferenceTag);
        fileStreamXosc->writeAttribute(CatalogReferenceAttribute, TrajectoryCatalogTag);
        fileStreamXosc->writeAttribute(entryNameAttribute, TrajectoryNameAttributeValue);
        fileStreamXosc->writeEndElement(); //End CatalogReference
        fileStreamXosc->writeStartElement(LongitudinalTag);
        fileStreamXosc->writeStartElement(Nonetag);
        fileStreamXosc->writeEndElement(); //End NoneTag
        fileStreamXosc->writeEndElement(); //End LongitudinalTag
        fileStreamXosc->writeStartElement(LateralTag);
        fileStreamXosc->writeAttribute(purposeAttribute, LateralPurposeValue);
        fileStreamXosc->writeEndElement(); //End LateralTag
        fileStreamXosc->writeEndElement(); //End FollowTrajectory
        fileStreamXosc->writeEndElement(); //End RoutingTag
        fileStreamXosc->writeEndElement(); //End Private
        fileStreamXosc->writeEndElement(); //End Action
        fileStreamXosc->writeStartElement(ConditionsTag);
        fileStreamXosc->writeStartElement(StartTag);
        fileStreamXosc->writeStartElement(ConditionGroupTag);
        fileStreamXosc->writeStartElement(ConditionTag);
        fileStreamXosc->writeAttribute(delayAttribute, ConditionDelayValue);
        fileStreamXosc->writeAttribute(edgeAttribute, ConditionEdgeValue);
        fileStreamXosc->writeAttribute(NameAttribute, ConditionNameValue);
        fileStreamXosc->writeStartElement(ByValueTag);
        fileStreamXosc->writeStartElement(SimulationTimeTag);
        fileStreamXosc->writeAttribute(ruleAttribute, SimulationTimeRuleValue);
        fileStreamXosc->writeAttribute(valueAttribute, QString::number(SimulationTimeStart));
        fileStreamXosc->writeEndElement(); //End SimulationTimeTag
        fileStreamXosc->writeEndElement(); //End ByValueTag
        fileStreamXosc->writeEndElement(); //End ConditionTag
        fileStreamXosc->writeEndElement(); //End ConditionGroupTag
        fileStreamXosc->writeEndElement(); //End StartTag
        fileStreamXosc->writeEndElement(); //End ConditionsTag
        fileStreamXosc->writeEndElement(); //End EventTag
        fileStreamXosc->writeEndElement(); //End ManeuverTag
        fileStreamXosc->writeEndElement(); //End SequenceTag
        fileStreamXosc->writeStartElement(ConditionsTag);
        fileStreamXosc->writeStartElement(StartTag);
        fileStreamXosc->writeStartElement(ConditionGroupTag);
        fileStreamXosc->writeStartElement(ConditionTag);
        fileStreamXosc->writeAttribute(delayAttribute, ConditionDelayValue);
        fileStreamXosc->writeAttribute(edgeAttribute, ConditionEdgeValue);
        fileStreamXosc->writeAttribute(NameAttribute, ConditionNameValue);
        fileStreamXosc->writeStartElement(ByValueTag);
        fileStreamXosc->writeStartElement(SimulationTimeTag);
        fileStreamXosc->writeAttribute(ruleAttribute, SimulationTimeRuleValue);
        fileStreamXosc->writeAttribute(valueAttribute, SimulationStartDelay);
        fileStreamXosc->writeEndElement(); //End SimulationTimeTag
        fileStreamXosc->writeEndElement(); //End ByValueTag
        fileStreamXosc->writeEndElement(); //End ConditionTag
        fileStreamXosc->writeEndElement(); //End ConditionGroupTag
        fileStreamXosc->writeEndElement(); //End StartTag
        fileStreamXosc->writeStartElement(EndTag);
        fileStreamXosc->writeStartElement(ConditionGroupTag);
        fileStreamXosc->writeStartElement(ConditionTag);
        fileStreamXosc->writeAttribute(delayAttribute, ConditionDelayValue);
        fileStreamXosc->writeAttribute(edgeAttribute, ConditionEdgeValue);
        fileStreamXosc->writeAttribute(NameAttribute, ConditionNameValue);
        fileStreamXosc->writeStartElement(ByValueTag);
        fileStreamXosc->writeStartElement(SimulationTimeTag);
        fileStreamXosc->writeAttribute(ruleAttribute, SimulationTimeRuleValue);
        fileStreamXosc->writeAttribute(valueAttribute, QString::number(SimulationTimeEnd));
        fileStreamXosc->writeEndElement(); //End SimulationTimeTag
        fileStreamXosc->writeEndElement(); //End ByValueTag
        fileStreamXosc->writeEndElement(); //End ConditionTag
        fileStreamXosc->writeEndElement(); //End ConditionGroupTag
        fileStreamXosc->writeEndElement(); //End EndTag
        fileStreamXosc->writeEndElement(); //End ConditionsTag
        fileStreamXosc->writeEndElement(); //End ActTag

        // Trajectory File
        fileStream->writeStartElement(TrajectoryTag);
        fileStream->writeAttribute(NameAttribute, TrajectoryNameAttributeValue);

        fileStream->writeStartElement(ParameterDeclarationTag);
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
    fileStreamXosc->writeEndElement(); //End StoryTag

    fileStreamXosc->writeStartElement(EndTag);
    fileStreamXosc->writeStartElement(ConditionGroupTag);
    fileStreamXosc->writeStartElement(ConditionTag);
    fileStreamXosc->writeAttribute(delayAttribute, ConditionDelayValue);
    fileStreamXosc->writeAttribute(edgeAttribute, ConditionEdgeValue);
    fileStreamXosc->writeAttribute(NameAttribute, ConditionNameValue);
    fileStreamXosc->writeStartElement(ByValueTag);
    fileStreamXosc->writeStartElement(SimulationTimeTag);
    fileStreamXosc->writeAttribute(ruleAttribute, SimulationTimeRuleValue);
    fileStreamXosc->writeAttribute(valueAttribute, "500.0");
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


