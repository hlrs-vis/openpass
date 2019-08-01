/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "systemConfigImporter.h"
#include <QCoreApplication>

#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
{ \
    throw std::runtime_error("Checkfalse in scenario importer failed"); \
    } \
    } \
    while(0);


using namespace SimulationCommon;

namespace Importer {

//! From parameterParser.cpp in openPASS_Source_Code/CoreFramework/CoreShare/
bool SystemConfigImporter::ImportSystemParameters(QDomElement& parametersElement, ParameterInterface* parameters)
{
    QDomElement parameterElement = parametersElement.firstChildElement("parameter");
    while (!parameterElement.isNull())
    {
        //int id = parameterElement.firstChildElement("id").text().toInt();
        std::string id = parameterElement.firstChildElement("id").text().toStdString();

        QString type = parameterElement.firstChildElement("type").text();
        QString value = parameterElement.firstChildElement("value").text();

        if (type == "int")
        {
            CHECKFALSE(parameters->AddParameterInt(id, value.toInt()));
        }
        else
            if (type == "double")
            {
                CHECKFALSE(parameters->AddParameterDouble(id, value.toDouble()));
            }
            else
                if (type == "bool")
                {
                    value = value.toLower();
                    bool valueRobust = false;
                    if (value == "true" || (value.size() == 1 && value.toInt() > 0))
                    {
                        valueRobust = true;
                    }
                    CHECKFALSE(parameters->AddParameterBool(id, valueRobust));
                }
                else
                    if (type == "string")
                    {
                        CHECKFALSE(parameters->AddParameterString(id, value.toStdString()));
                    }
                    else
                        if (type == "intVector")
                        {
                            std::vector<int> vector{};
                            try
                            {
                                std::stringstream valueStream(value.toStdString());

                                int item;
                                while (valueStream >> item)
                                {
                                    vector.push_back(item);

                                    if (valueStream.peek() == ',')
                                    {
                                        valueStream.ignore();
                                    }
                                }
                            }
                            catch (...)
                            {
                                return false;
                            }
                            if (!parameters->AddParameterIntVector(id, vector))
                            {
                                LOG_INTERN(LogLevel::Error) << "an error occurred during import of parameters";
                                return false;
                            }
                        }
                        else
                            if (type == "doubleVector")
                            {
                                std::vector<double>vector {};
                                try
                                {
                                    std::stringstream valueStream(value.toStdString());

                                    double item;
                                    while (valueStream >> item)
                                    {
                                        vector.push_back(item);

                                        if (valueStream.peek() == ',')
                                        {
                                            valueStream.ignore();
                                        }
                                    }
                                }
                                catch (...)
                                {
                                    return false;
                                }
                                if (!parameters->AddParameterDoubleVector(id, vector))
                                {
                                    LOG_INTERN(LogLevel::Error) << "an error occurred during import of parameters";
                                    return false;
                                }
                            }
                            else
                                if (type == "boolVector")
                                {
                                    std::vector<bool> vector{};
                                    try
                                    {
                                        std::stringstream valueStream(value.toStdString());

                                        bool item;
                                        while (valueStream >> item)
                                        {
                                            vector.push_back(item);

                                            if (valueStream.peek() == ',')
                                            {
                                                valueStream.ignore();
                                            }
                                        }
                                    }
                                    catch (...)
                                    {
                                        return false;
                                    }
                                    if (!parameters->AddParameterBoolVector(id, vector))
                                    {
                                        LOG_INTERN(LogLevel::Error) << "an error occurred during import of parameters";
                                        return false;
                                    }
                                }
        parameterElement = parameterElement.nextSiblingElement("parameter");
    }
    return true;
}


bool SystemConfigImporter::ImportSystemConfigContent(const std::string& filename, QDomDocument& document)
{
    std::locale::global(std::locale("C"));

    if (!QFileInfo(QString::fromStdString(filename)).exists())
    {
        LOG_INTERN(LogLevel::Error) << "Configuration does not exist: " << filename;
        return false;
    }

    QFile xmlFile(QString::fromStdString(filename)); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during agent type import: " << filename;
        return false;
    }

    QByteArray xmlData(xmlFile.readAll());
    QString errorMsg;
    int errorLine;
    if (!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        LOG_INTERN(LogLevel::Error) << "invalid xml file format of file " << filename;
        LOG_INTERN(LogLevel::Error) << "in line " << errorLine << " : " << errorMsg.toStdString();
        return false;
    }

    return true;
}

bool SystemConfigImporter::Import(const std::string& filename,
                                  std::shared_ptr<Configuration::SystemConfig> systemConfig)
{
    QDomDocument document;
    if (!ImportSystemConfigContent(filename, document))
    {
        return false;
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        return false;
    }

    // parse agents
    auto& agentTypes = systemConfig->GetSystems();
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

            std::shared_ptr<SimulationSlave::AgentType> agent = std::make_shared<SimulationSlave::AgentType>();
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
                    std::string componentId;
                    CHECKFALSE(SimulationCommon::ParseString(componentElement, "id", componentId));
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

                    auto component = std::make_shared<SimulationSlave::ComponentType>(componentId,
                                     isInitComponent,
                                     componentPriority,
                                     offsetTime,
                                     responseTime,
                                     cycleTime,
                                     library);
                    CHECKFALSE(component);

                    auto parameters = systemConfig->AddModelParameters();
                    component->SetModelParameter(parameters);

                    CHECKFALSE(agent->AddComponent(component));


                    // parse model parameters
                    LOG_INTERN(LogLevel::DebugCore) << "import model parameters...";

                    QDomElement parametersElement;
                    CHECKFALSE(SimulationCommon::GetFirstChildElement(componentElement, "parameters",

                               parametersElement));

                    CHECKFALSE(ImportSystemParameters(parametersElement,
                                                      component->GetModelParameters()));

                    componentElement = componentElement.nextSiblingElement("component");
                } // component loop
            } // if components exist

            // parse connections
            QDomElement connectionsElement;
            CHECKFALSE(SimulationCommon::GetFirstChildElement(systemElement, "connections",
                       connectionsElement));

            std::map<std::pair<std::string, int>, int> channelMap;
            QDomElement connectionElement;
            if (SimulationCommon::GetFirstChildElement(connectionsElement, "connection", connectionElement))
            {
                while (!connectionElement.isNull())
                {
                    QDomElement sourceElement = connectionElement.firstChildElement("source");
                    std::string sourceId = sourceElement.firstChildElement("component").text().toStdString();
                    int sourceOutputId = sourceElement.firstChildElement("output").text().toInt();


                    int channelId = connectionElement.firstChildElement("id").text().toInt();
                    std::pair<std::string, int> componentPair = std::make_pair(sourceId, sourceOutputId);
                    std::map<std::pair<std::string, int>, int>::iterator channelIterator;
                    channelIterator = channelMap.find(componentPair);

                    if (channelIterator == channelMap.end())
                    {
                        //                        channelId = channelMap.size();
                        channelMap.emplace(std::make_pair(componentPair, channelId));
                        agent->AddChannel(channelId);

                        agent->GetComponents().at(sourceId)->AddOutputLink(sourceOutputId, channelId);
                    }
                    else
                    {
                        //                        channelId = channelIterator->second;
                    }

                    QDomElement targetElement = connectionElement.firstChildElement("target");
                    while (!targetElement.isNull())
                    {
                        std::string targetId = targetElement.firstChildElement("component").text().toStdString();
                        int targetInputId = targetElement.firstChildElement("input").text().toInt();
                        agent->GetComponents().at(targetId)->AddInputLink(targetInputId, channelId);
                        targetElement = targetElement.nextSiblingElement("target");
                    }

                    connectionElement = connectionElement.nextSiblingElement("connection");
                }
            }

            systemElement = systemElement.nextSiblingElement("system");
        }
    }

    return true;
}

} //namespace importer
