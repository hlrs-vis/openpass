/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SceneryModel.h"

#include <QIODevice>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

QString const SceneryModel::EmptyValue = QStringLiteral("");

QString const SceneryModel::PCM = QStringLiteral("PCM");
QString const SceneryModel::OpenDRIVE = QStringLiteral("OpenDRIVE");

SceneryModel::SceneryModel(ProjectInterface * const project, QObject *parent)
    : QObject(parent)
    , _project(project)
    , _source(EmptyValue)
    , _agents(EmptyValue)
{
    // update actions from the Browse Buttons
    connect(_project, &ProjectInterface::update,
            this, &SceneryModel::update);
}

bool SceneryModel::setSceneryConfigFile(ProjectInterface::SceneryConfig const &sceneryPath)
{
    // Update the path to the directory containing the Scenery Configuration File
    if (_project->setSceneryConfigFile(sceneryPath)){

        // Load the File to get information about the source and number of agents
        if (loadFile(sceneryPath)){
            Q_EMIT update();
            return true;
        }
    }
    return false;
}

ProjectInterface::SceneryConfig SceneryModel::getSceneryConfigFile() const
{
    // Get the path of the directory containing the Scenery Configuration File
    return _project->getSceneryConfigFile();
}

SceneryModel::Source SceneryModel::getScenerySource() const
{
    // Get the source type of the Scenery configuration {openDrive; PCM}
    return _source;
}

SceneryModel::Agents SceneryModel::getSceneryAgents() const
{
    // Get the number of agents in the scenery configuration
    return _agents;
}

bool SceneryModel::loadFile(QString const & filepath)
{
    // Does the file exist and can we open it?
    QFile file(filepath);
    if ((file.exists()) && (file.open(QIODevice::ReadOnly | QIODevice::Text)))
    {
        return readFile(&file);
    }
    return false;
}

bool SceneryModel::readFile(QIODevice * const device)
{
    // Initialize xml stream reader
    QXmlStreamReader xml(device);

    // Initialize the values of the variables
    initialize();

    // Verify xml header to get the source of the Scenery configuration {openDrive; PCM}
    if ((xml.readNext() == QXmlStreamReader::TokenType::StartDocument) &&
            (xml.readNext() == QXmlStreamReader::TokenType::StartElement))
    {
        if (xml.name() == PCM)
            _source = PCM;
        else if (xml.name() == OpenDRIVE)
            _source = OpenDRIVE;
        else
            _source = EmptyValue;
    }

    // Searching the number of agents (Participants) in the scenery configuration
    while (xml.readNext() != QXmlStreamReader::TokenType::EndDocument)
    {
        if (xml.name() == "Participants"){
            _agents = xml.readElementText();
            break;
        }
    }
    if (_agents.isEmpty())
        _agents = "1";
    return true;
}

bool SceneryModel::initialize()
{
    _source = EmptyValue;
    _agents = EmptyValue;
    return true;
}
