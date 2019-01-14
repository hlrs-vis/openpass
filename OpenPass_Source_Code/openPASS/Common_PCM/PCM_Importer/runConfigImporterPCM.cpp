/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <QtWidgets/QApplication>
#include <memory>
#include <iostream>
#include <QDomDocument>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "runConfigImporterPCM.h"
#include "globalDefinitions.h"
#include "xmlParser.h"
#include "parameterParser.h"

// macros to check and return false if result is false
#define CHECKRF(element) \
    if(!(element)) \
    { \
       return false; \
    }

using namespace SimulationCommon;

RunConfigImporterPCM::RunConfigImporterPCM() {}

RunConfigImporterPCM::~RunConfigImporterPCM()
{
    delete observationParams;

    initials.clear();
    participants.clear();
    trajectories.clear();
}

std::vector<PCM_InitialValues> RunConfigImporterPCM::GetInitials()
{
    return initials;
}

std::vector<PCM_ParticipantData> RunConfigImporterPCM::GetParticipants()
{
    return participants;
}

std::vector<PCM_Trajectory *> RunConfigImporterPCM::GetTrajectories()
{
    return trajectories;
}

QString RunConfigImporterPCM::GetCaseNumber()
{
    return pcmCase;
}

bool RunConfigImporterPCM::GetDocumentRoot(const QString &filename, QDomElement &documentRoot)
{
    std::locale::global(std::locale("C"));

    QString absoluteFilePath = "";

    if (QFileInfo(filename).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        absoluteFilePath = baseDir.absoluteFilePath(filename);
        absoluteFilePath = baseDir.cleanPath(absoluteFilePath);
    }
    else
    {
        absoluteFilePath = filename;
    }

    if (!QFileInfo(absoluteFilePath).exists())
    {
        qDebug("Run Config does not exist.");
        return false;
    }

    QFile xmlFile(absoluteFilePath); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug("an error occurred during run configuration import");
        return false;
    }

    QDomDocument document;
    if (!document.setContent(&xmlFile))
    {
        qDebug("invalid xml file format of file %s", qPrintable(filename));
        return false;
    }

    documentRoot = document.documentElement();
    CHECKRF(!documentRoot.isNull())
    return true;
}

bool RunConfigImporterPCM::ImportInitials(QDomElement &documentRoot)
{
    // parse spawn points
    QDomElement spawnPointsElement;
    CHECKRF(GetFirstChildElement(documentRoot, "SpawnPoints", spawnPointsElement))

    QDomElement spawnPointElement;
    CHECKRF(GetFirstChildElement(spawnPointsElement, "SpawnPoint", spawnPointElement))

    while (!spawnPointElement.isNull())
    {
        // parse parameters
        QDomElement parametersElement;
        CHECKRF(GetFirstChildElement(spawnPointElement, "SpawnPointParameters", parametersElement))

        QStringList initParams;
        QDomElement doubleParamElement;
        CHECKRF(GetFirstChildElement(parametersElement, "Double", doubleParamElement))

        while (!doubleParamElement.isNull())
        {
            std::string strValue;
            CHECKRF(ParseAttributeString(doubleParamElement, "value", strValue))
            initParams.append(strValue.c_str());

            doubleParamElement = doubleParamElement.nextSiblingElement("Double");
        }
        initials.push_back(PCM_InitialValues(initParams[0], initParams[1], initParams[2], initParams[3], initParams[4], initParams[5], initParams[6]));

        spawnPointElement = spawnPointElement.nextSiblingElement("SpawnPoint");
    } // spawn point loop
    return true;
}

bool RunConfigImporterPCM::ImportParticipants(QDomElement &documentRoot)
{
    // parse the XML tag <Agents>
    QDomElement agentsElement;
    CHECKRF(GetFirstChildElement(documentRoot, "Agents", agentsElement))

    QDomElement agentElement;
    CHECKRF(GetFirstChildElement(agentsElement, "Agent", agentElement))

    while (!agentElement.isNull())
    {
        std::string vehicleTypeName;
        std::string vehicleWidth;
        std::string vehicleLength;
        std::string distcgfa;
        std::string vehicleWeight;
        std::string vehicleHeightcg;
        std::string vehicleWheelbase;
        std::string vehicleIxx;
        std::string vehicleIyy;
        std::string vehicleIzz;
        std::string vehicleMue;
        std::string vehicleTrackwidth;
        std::string cogfront;

        CHECKRF(ParseString(agentElement, "Type", vehicleTypeName))
        QString vehicleTypeCode = QString::number(int(GetAgentVehicleType(vehicleTypeName)));

        CHECKRF(ParseString(agentElement, "Width", vehicleWidth))
        CHECKRF(ParseString(agentElement, "Length", vehicleLength))
        CHECKRF(ParseString(agentElement, "DistanceCOGtoFrontAxle", distcgfa))
        CHECKRF(ParseString(agentElement, "Weight", vehicleWeight))
        CHECKRF(ParseString(agentElement, "HeightCOG", vehicleHeightcg))
        CHECKRF(ParseString(agentElement, "Wheelbase", vehicleWheelbase))
        CHECKRF(ParseString(agentElement, "MomentInertiaRoll", vehicleIxx))
        CHECKRF(ParseString(agentElement, "MomentInertiaPitch", vehicleIyy))
        CHECKRF(ParseString(agentElement, "MomentInertiaYaw", vehicleIzz))
        CHECKRF(ParseString(agentElement, "FrictionCoeff", vehicleMue))
        CHECKRF(ParseString(agentElement, "TrackWidth", vehicleTrackwidth))
        CHECKRF(ParseString(agentElement, "DistanceCOGtoLeadingEdge", cogfront))

//        qDebug("import agent vehicle type %s", vehicleTypeName.c_str());

        participants.push_back(PCM_ParticipantData(vehicleTypeCode,
                                                   vehicleWidth.c_str(),
                                                   vehicleLength.c_str(),
                                                   distcgfa.c_str(),
                                                   vehicleWeight.c_str(),
                                                   vehicleHeightcg.c_str(),
                                                   vehicleWheelbase.c_str(),
                                                   vehicleIxx.c_str(),
                                                   vehicleIyy.c_str(),
                                                   vehicleIzz.c_str(),
                                                   vehicleMue.c_str(),
                                                   vehicleTrackwidth.c_str(),
                                                   "",
                                                   cogfront.c_str()));


        agentElement = agentElement.nextSiblingElement("Agent");
    } // agent loop
    return true;
}

bool RunConfigImporterPCM::ImportObservations(QDomElement &documentRoot)
{
    QDomElement observationsElement;
    CHECKRF(GetFirstChildElement(documentRoot, "Observations", observationsElement))

    QDomElement observationElement;
    CHECKRF(GetFirstChildElement(observationsElement, "Observation", observationElement))

    while (!observationElement.isNull())
    {
        std::string libraryName;
        CHECKRF(ParseAttributeString(observationElement, "library", libraryName))

        if(libraryName.compare("Evaluation_Pcm") == 0) // parse trajectories
        {
            QDomElement parametersElement;
            CHECKRF(GetFirstChildElement(observationElement, "ObservationParameters", parametersElement))

            observationParams = new Parameters();
            CHECKRF(ImportParameters(parametersElement, *observationParams))

            const std::vector<int> *times = observationParams->GetParametersIntVector().at(1);

            const std::vector<double>* x0 = observationParams->GetParametersDoubleVector().at(2);
            const std::vector<double>* y0 = observationParams->GetParametersDoubleVector().at(3);
            const std::vector<double>* uVel0 = observationParams->GetParametersDoubleVector().at(4);
            const std::vector<double>* vVel0 = observationParams->GetParametersDoubleVector().at(5);
            const std::vector<double>* yaw0 = observationParams->GetParametersDoubleVector().at(6);
            trajectories.push_back(new PCM_Trajectory(times, x0, y0, uVel0, vVel0, yaw0));

            const std::vector<double>* x1 = observationParams->GetParametersDoubleVector().at(7);
            const std::vector<double>* y1 = observationParams->GetParametersDoubleVector().at(8);
            const std::vector<double>* uVel1 = observationParams->GetParametersDoubleVector().at(9);
            const std::vector<double>* vVel1 = observationParams->GetParametersDoubleVector().at(10);
            const std::vector<double>* yaw1 = observationParams->GetParametersDoubleVector().at(11);
            trajectories.push_back(new PCM_Trajectory(times, x1, y1, uVel1, vVel1, yaw1));

            pcmCase = QString::fromUtf8(observationParams->GetParametersString().at(13).c_str());
            return true;
        }
        observationElement = observationElement.nextSiblingElement("Observation");
    } // observation loop

    return true;
}

// import the run config file to create participant data.
// Since the data is only used for generating systemConfig, only the info of participant type is parsed.
//bool RunConfigImporterPCM::ImportParticipantData(const QString &filename, std::vector<PCM_ParticipantData> &participants)
bool RunConfigImporterPCM::Import(const QString &filename)
{
    QDomElement documentRoot;
    CHECKRF(GetDocumentRoot(filename, documentRoot))

    // parse initials from the tag <SpawnPoints>
    CHECKRF(ImportInitials(documentRoot))

    // parse participants from the tag <Agents>
    CHECKRF(ImportParticipants(documentRoot))

    // parse trajectories and pcmCase number from the tag <Observations>
    CHECKRF(ImportObservations(documentRoot))

    return true;
}
