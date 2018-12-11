/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <QApplication>
#include <climits>
#include <cfloat>
#include <iostream>
#include <sstream>
#include <string>
#include <QDomDocument>
#include <QDir>
#include <QFile>
#include "agentSystemImporter.h"
#include "componentType.h"
#include "agentType.h"
#include "component.h"
#include "parameters.h"
#include "log.h"
#include "xmlParser.h"
#include "parameterParser.h"

// local helper macros
#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
{ \
    LOG_INTERN(LogLevel::Warning) << "an error occurred during agent type import"; \
    return false; \
    } \
    } \
    while(0);

namespace SimulationSlave {

bool AgentSystemImporter::Import(const std::string &filename,
                                 std::map<int, const AgentType *> &agentTypes)
{
    std::locale::global(std::locale("C"));

    QString absoluteFilePath = "";

    if (QFileInfo(QString::fromStdString(filename)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        absoluteFilePath = baseDir.absoluteFilePath(QString::fromStdString(filename));
        absoluteFilePath = baseDir.cleanPath(absoluteFilePath);
    }
    else
    {
        absoluteFilePath = QString::fromStdString(filename);
    }

    if (!QFileInfo(absoluteFilePath).exists())
    {
        LOG_INTERN(LogLevel::Error) << "System Config does not exist.";
        return false;
    }

    QFile xmlFile(absoluteFilePath); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Warning) << "an error occurred during agent type import";
        return false;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    QString errorMsg;
    int errorLine;
    if (!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml file format of file " << filename;
        LOG_INTERN(LogLevel::Warning) << "in line " << errorLine << " : " << errorMsg.toStdString();
        return false;
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        return false;
    }

    // parse agents
    QDomElement systemElement;
    if (SimulationCommon::GetFirstChildElement(documentRoot, "system", systemElement))
    {
        while (!systemElement.isNull())
        {
            // retrieve agent id
            int agentId;
            CHECKFALSE(SimulationCommon::ParseInt(systemElement, "id", agentId));
            LOG_INTERN(LogLevel::DebugCore) << "agent type id: " << agentId <<
                                            " *********************************************************";

            // retrieve agent priority
            int agentPriority;
            CHECKFALSE(SimulationCommon::ParseInt(systemElement, "priority", agentPriority));
            CHECKFALSE(0 <= agentPriority);

            LOG_INTERN(LogLevel::DebugCore) << "agent type priority: " << agentPriority;

            // create agent
            CHECKFALSE(0 == agentTypes.count(agentId)); // avoid duplicate types

            AgentType *agent = new (std::nothrow) AgentType(agentId, agentPriority);
            CHECKFALSE(agent);

            CHECKFALSE(agentTypes.insert({agentId, agent}).second);

            // parse components
            QDomElement componentsElement;
            CHECKFALSE(SimulationCommon::GetFirstChildElement(systemElement, "components", componentsElement));

            QDomElement componentElement;
            if (SimulationCommon::GetFirstChildElement(componentsElement, "component", componentElement))
            {
                while (!componentElement.isNull())
                {
                    // retrieve component id
                    int componentId;
                    CHECKFALSE(SimulationCommon::ParseInt(componentElement, "id", componentId));
                    LOG_INTERN(LogLevel::DebugCore) << "component type id: " << componentId <<
                                                    " ---------------------------------------------------------";

                    // retrieve component library
                    std::string library;
                    CHECKFALSE(SimulationCommon::ParseString(componentElement, "library", library));
                    CHECKFALSE(!library.empty());
                    LOG_INTERN(LogLevel::DebugCore) << "library: " << library;

                    QDomElement scheduleElement = componentElement.firstChildElement("schedule");

                    // retrieve component priority
                    int componentPriority;
                    CHECKFALSE(SimulationCommon::ParseInt(scheduleElement, "priority", componentPriority));
                    CHECKFALSE(0 <= componentPriority);
                    LOG_INTERN(LogLevel::DebugCore) << "component priority: " << componentPriority;

                    // retrieve component offset time
                    int offsetTime = 0; // must be set to 0 for init tasks for scheduler
                    CHECKFALSE(SimulationCommon::ParseInt(scheduleElement, "offset", offsetTime));
                    CHECKFALSE(0 <= offsetTime);
                    LOG_INTERN(LogLevel::DebugCore) << "offset time: " << offsetTime;

                    // retrieve component response time
                    int responseTime = 0; // must be set to 0 for init tasks for scheduler
                    CHECKFALSE(SimulationCommon::ParseInt(scheduleElement, "response", responseTime));
                    CHECKFALSE(0 <= responseTime);
                    LOG_INTERN(LogLevel::DebugCore) << "response time: " << responseTime;

                    // retrieve component cycle time
                    int cycleTime = 0; // must be set to 0 for init tasks for scheduler
                    CHECKFALSE(SimulationCommon::ParseInt(scheduleElement, "cycle", cycleTime));
                    CHECKFALSE(0 <= cycleTime);
                    LOG_INTERN(LogLevel::DebugCore) << "cycle time: " << cycleTime;

                    bool isInitComponent = false;
                    if (cycleTime == 0)
                    {
                        isInitComponent = true;
                    }

                    // create type
                    ComponentType *component = new (std::nothrow) ComponentType(componentId,
                                                                                isInitComponent,
                                                                                componentPriority,
                                                                                offsetTime,
                                                                                responseTime,
                                                                                cycleTime,
                                                                                library);
                    CHECKFALSE(component);

                    CHECKFALSE(agent->AddComponent(componentId, component));


                    // parse model parameters
                    LOG_INTERN(LogLevel::DebugCore) << "import model parameters...";

                    QDomElement parametersElement;
                    CHECKFALSE(SimulationCommon::GetFirstChildElement(componentElement, "parameters",
                                                                      parametersElement));

                    CHECKFALSE(SimulationCommon::ImportSystemParameters(parametersElement,
                                                                        component->GetModelParameters()));

                    componentElement = componentElement.nextSiblingElement("component");
                } // component loop
            } // if components exist

            // parse connections
            QDomElement connectionsElement;
            CHECKFALSE(SimulationCommon::GetFirstChildElement(systemElement, "connections",
                                                              connectionsElement));

            std::map<std::pair<int, int>, int> channelMap;
            QDomElement connectionElement;
            if (SimulationCommon::GetFirstChildElement(connectionsElement, "connection", connectionElement))
            {
                while (!connectionElement.isNull())
                {
                    QDomElement targetElement = connectionElement.firstChildElement("target");
                    int targetId = targetElement.firstChildElement("component").text().toInt();
                    int targetInputId = targetElement.firstChildElement("input").text().toInt();

                    QDomElement sourceElement = connectionElement.firstChildElement("source");
                    int sourceId = sourceElement.firstChildElement("component").text().toInt();
                    int sourceOutputId = sourceElement.firstChildElement("output").text().toInt();

                    int channelId;
                    std::pair<int, int> componentPair = std::make_pair(sourceId, sourceOutputId);
                    std::map<std::pair<int, int>, int>::iterator channelIterator;
                    channelIterator = channelMap.find(componentPair);

                    if (channelIterator == channelMap.end())
                    {
                        channelId = (int)channelMap.size();
                        channelMap.emplace(std::make_pair(componentPair, channelId));
                        agent->AddChannel(channelId);

                        agent->GetComponents().at(sourceId)->AddOutputLink(channelId, sourceOutputId);
                    }
                    else
                    {
                        channelId = channelIterator->second;
                    }
                    agent->GetComponents().at(targetId)->AddInputLink(channelId, targetInputId);

                    connectionElement = connectionElement.nextSiblingElement("connection");
                }
            }

            systemElement = systemElement.nextSiblingElement("system");
        } // system loop
    } // if systems exist

    return true;
}

} // namespace SimulationSlave
