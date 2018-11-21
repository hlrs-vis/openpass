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
#include "agentConfigImporter.h"
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

bool AgentConfigImporter::Import(const std::string &filename,
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
        LOG_INTERN(LogLevel::Error) << "Agent Config does not exist.";
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
    QDomElement agentElement;
    if (SimulationCommon::GetFirstChildElement(documentRoot, "Agent", agentElement))
    {
        while (!agentElement.isNull())
        {
            // retrieve agent id
            int agentId;
            CHECKFALSE(SimulationCommon::ParseAttributeInt(agentElement, "id", agentId));
            LOG_INTERN(LogLevel::DebugCore) << "agent type id: " << agentId <<
                                            " *********************************************************";

            // retrieve agent priority
            int agentPriority;
            CHECKFALSE(SimulationCommon::ParseAttributeInt(agentElement, "priority", agentPriority));
            CHECKFALSE(0 <= agentPriority);

            LOG_INTERN(LogLevel::DebugCore) << "agent type priority: " << agentPriority;

            // create agent
            CHECKFALSE(0 == agentTypes.count(agentId)); // avoid duplicate types

            AgentType *agent = new (std::nothrow) AgentType(agentId, agentPriority);
            CHECKFALSE(agent);

            CHECKFALSE(agentTypes.insert({agentId, agent}).second);

            // parse channels
            QDomElement channelsElement;
            CHECKFALSE(SimulationCommon::GetFirstChildElement(agentElement, "Channels", channelsElement));

            QDomElement channelElement;
            if (SimulationCommon::GetFirstChildElement(channelsElement, "Channel", channelElement))
            {
                while (!channelElement.isNull())
                {
                    // retrieve channel id
                    int channelId;
                    CHECKFALSE(SimulationCommon::ParseAttributeInt(channelElement, "id", channelId));

                    LOG_INTERN(LogLevel::DebugCore) << "channel type id: " << channelId;

                    CHECKFALSE(agent->AddChannel(channelId));

                    channelElement = channelElement.nextSiblingElement("Channel");
                } // channel loop
            } // if channels exist

            // parse components
            QDomElement componentsElement;
            CHECKFALSE(SimulationCommon::GetFirstChildElement(agentElement, "Components", componentsElement));

            QDomElement componentElement;
            if (SimulationCommon::GetFirstChildElement(componentsElement, "Component", componentElement))
            {
                while (!componentElement.isNull())
                {
                    // retrieve component id
                    int componentId;
                    CHECKFALSE(SimulationCommon::ParseAttributeInt(componentElement, "id", componentId));
                    LOG_INTERN(LogLevel::DebugCore) << "component type id: " << componentId <<
                                                    " ---------------------------------------------------------";

                    // check for init component
                    bool isInitComponent;
                    CHECKFALSE(SimulationCommon::ParseAttributeBool(componentElement, "init", isInitComponent));
                    LOG_INTERN(LogLevel::DebugCore) << "is init: " << isInitComponent;

                    // retrieve component library
                    std::string library;
                    CHECKFALSE(SimulationCommon::ParseAttributeString(componentElement, "library", library));
                    CHECKFALSE(!library.empty());
                    LOG_INTERN(LogLevel::DebugCore) << "library: " << library;

                    // retrieve component priority
                    int componentPriority;
                    CHECKFALSE(SimulationCommon::ParseAttributeInt(componentElement, "priority", componentPriority));
                    CHECKFALSE(0 <= componentPriority);
                    LOG_INTERN(LogLevel::DebugCore) << "component priority: " << componentPriority;

                    // retrieve component offset time
                    int offsetTime = 0; // must be set to 0 for init tasks for scheduler
                    if (!isInitComponent)
                    {
                        CHECKFALSE(SimulationCommon::ParseAttributeInt(componentElement, "offsetTime", offsetTime));
                        CHECKFALSE(0 <= offsetTime);
                        LOG_INTERN(LogLevel::DebugCore) << "offset time: " << offsetTime;
                    }

                    // retrieve component response time
                    int responseTime = 0; // must be set to 0 for init tasks for scheduler
                    if (!isInitComponent)
                    {
                        CHECKFALSE(SimulationCommon::ParseAttributeInt(componentElement, "responseTime", responseTime));
                        CHECKFALSE(0 <= responseTime);
                        LOG_INTERN(LogLevel::DebugCore) << "response time: " << responseTime;
                    }

                    // retrieve component cycle time
                    int cycleTime = 0; // must be set to 0 for init tasks for scheduler
                    if (!isInitComponent)
                    {
                        CHECKFALSE(SimulationCommon::ParseAttributeInt(componentElement, "cycleTime", cycleTime));
                        CHECKFALSE(0 < cycleTime);
                        LOG_INTERN(LogLevel::DebugCore) << "cycle time: " << cycleTime;
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

                    // parse inputs
                    QDomElement componentInputsElement;
                    CHECKFALSE(SimulationCommon::GetFirstChildElement(componentElement, "ComponentInputs",
                                                                      componentInputsElement));

                    QDomElement componentInputElement;
                    if (SimulationCommon::GetFirstChildElement(componentInputsElement, "ComponentInput",
                                                               componentInputElement))
                    {
                        while (!componentInputElement.isNull())
                        {
                            // retrieve input link id
                            int linkId;
                            CHECKFALSE(SimulationCommon::ParseAttributeInt(componentInputElement, "id", linkId));
                            LOG_INTERN(LogLevel::DebugCore) << "- input link id: " << linkId;

                            // retrieve input channel reference
                            int channelRef;
                            CHECKFALSE(SimulationCommon::ParseAttributeInt(componentInputElement, "channelRef", channelRef));
                            LOG_INTERN(LogLevel::DebugCore) << "  input channel ref: " << channelRef;

                            component->AddInputLink(channelRef, linkId);

                            componentInputElement = componentInputElement.nextSiblingElement("ComponentInput");
                        } // component input loop
                    } // if component inputs exist

                    // parse outputs
                    QDomElement componentOutputsElement;
                    CHECKFALSE(SimulationCommon::GetFirstChildElement(componentElement, "ComponentOutputs",
                                                                      componentOutputsElement));

                    QDomElement componentOutputElement;
                    if (SimulationCommon::GetFirstChildElement(componentOutputsElement, "ComponentOutput",
                                                               componentOutputElement))
                    {
                        while (!componentOutputElement.isNull())
                        {
                            // retrieve output link id
                            int linkId;
                            CHECKFALSE(SimulationCommon::ParseAttributeInt(componentOutputElement, "id", linkId));
                            LOG_INTERN(LogLevel::DebugCore) << "- output link id: " << linkId;

                            // retrieve output channel reference
                            int channelRef;
                            CHECKFALSE(SimulationCommon::ParseAttributeInt(componentOutputElement, "channelRef", channelRef));
                            LOG_INTERN(LogLevel::DebugCore) << "  output channel ref: " << channelRef;

                            component->AddOutputLink(channelRef, linkId);

                            componentOutputElement = componentOutputElement.nextSiblingElement("ComponentOutput");
                        } // component output loop
                    } // if component outputs exist

                    // parse observations
                    QDomElement componentObservationsElement;
                    CHECKFALSE(SimulationCommon::GetFirstChildElement(componentElement, "ComponentObservations",
                                                                      componentObservationsElement));

                    QDomElement componentObservationElement;
                    if (SimulationCommon::GetFirstChildElement(componentObservationsElement, "ComponentObservation",
                                                               componentObservationElement))
                    {
                        while (!componentObservationElement.isNull())
                        {
                            // retrieve id
                            int linkId;
                            CHECKFALSE(SimulationCommon::ParseAttributeInt(componentObservationElement, "id", linkId));
                            LOG_INTERN(LogLevel::DebugCore) << "- observation link id: " << linkId;

                            // retrieve reference
                            int observationRef;
                            CHECKFALSE(SimulationCommon::ParseAttributeInt(componentObservationElement, "observationRef",
                                                                           observationRef));
                            LOG_INTERN(LogLevel::DebugCore) << "  observation ref: " << observationRef;

                            component->AddObservationLink(observationRef, linkId);

                            componentObservationElement =
                                componentObservationElement.nextSiblingElement("ComponentObservation");
                        } // observation loop
                    } // if observations exist

                    // parse model parameters
                    LOG_INTERN(LogLevel::DebugCore) << "import model parameters...";

                    QDomElement parametersElement;
                    CHECKFALSE(SimulationCommon::GetFirstChildElement(componentElement, "ComponentParameters",
                                                                      parametersElement));

                    CHECKFALSE(SimulationCommon::ImportParameters(parametersElement, component->GetModelParameters()));

                    componentElement = componentElement.nextSiblingElement("Component");
                } // component loop
            } // if components exist

            agentElement = agentElement.nextSiblingElement("Agent");
        } // agent loop
    } // if agents exist

    return true;
}

} // namespace SimulationSlave
