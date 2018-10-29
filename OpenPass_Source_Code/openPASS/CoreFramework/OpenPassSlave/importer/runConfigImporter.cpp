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
#include <memory>
#include <iostream>
#include <QDomDocument>
#include <QDir>
#include <QFile>
#include "log.h"
#include "runConfigImporter.h"
#include "xmlParser.h"
#include "parameters.h"
#include "parameterParser.h"
#include "worldInterface.h"

// local helper macros
#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
{ \
    LOG_INTERN(LogLevel::Warning) << "an error occurred during run configuration import"; \
    return nullptr; \
    } \
    } \
    while(0);

namespace {

bool ImportObservations(QDomElement &observationsElement,
                        SimulationCommon::RunConfig &configuration)
{
    QDomElement observationElement;
    if (SimulationCommon::GetFirstChildElement(observationsElement, "Observation", observationElement))
    {
        while (!observationElement.isNull())
        {
            int observationId;
            if (!SimulationCommon::ParseAttributeInt(observationElement, "id", observationId))
            {
                return false;
            }

            std::string libraryName;
            if (!SimulationCommon::ParseAttributeString(observationElement, "library", libraryName))
            {
                return false;
            }

            LOG_INTERN(LogLevel::DebugCore) << "import observation: library " << libraryName;

            SimulationCommon::RunConfig::ObservationInstance *observationInstance = new (
                std::nothrow) SimulationCommon::RunConfig::ObservationInstance(observationId,
                                                                               libraryName);
            if (!observationInstance)
            {
                return false;
            }

            if (!configuration.AddObservationInstance(observationId, observationInstance))
            {
                return false;
            }

            // parse parameters
            LOG_INTERN(LogLevel::DebugCore) << "import observation parameters...";

            QDomElement parametersElement;
            if (!SimulationCommon::GetFirstChildElement(observationElement, "ObservationParameters",
                                                        parametersElement))
            {
                return false;
            }

            if (!ImportParameters(parametersElement, observationInstance->GetObservationParameters()))
            {
                return false;
            }

            observationElement = observationElement.nextSiblingElement("Observation");
        } // observation loop
    } // if observations exist

    return true;
}

} // namespace

namespace SimulationCommon {

RunConfig *RunConfigImporter::Import(const std::string &filename)
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
        LOG_INTERN(LogLevel::Error) << "Run Config does not exist.";
        return nullptr;
    }

    QFile xmlFile(absoluteFilePath); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Warning) << "an error occurred during run configuration import";
        return nullptr;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    if (!document.setContent(xmlData))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml file format of file " << filename;
        return nullptr;
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        return nullptr;
    }

    // parse start time
    int startTime;
    CHECKFALSE(ParseInt(documentRoot, "StartTime", startTime));
    LOG_INTERN(LogLevel::DebugCore) << "start time: " << startTime;

    // parse end time
    int endTime;
    CHECKFALSE(ParseInt(documentRoot, "EndTime", endTime));
    LOG_INTERN(LogLevel::DebugCore) << "end time: " << endTime;

    // parse number of invocations
    int numberInvocations;
    CHECKFALSE(ParseInt(documentRoot, "NumberInvocations", numberInvocations));
    LOG_INTERN(LogLevel::DebugCore) << "number of invocations: " << numberInvocations;

    // parse stochastics library name
    std::string stochasticsLibrary;
    CHECKFALSE(ParseString(documentRoot, "Stochastics", stochasticsLibrary));
    LOG_INTERN(LogLevel::DebugCore) << "StochasticsLibrary: " << stochasticsLibrary;

    // parse stochastics library name
    std::string collisionDetectionLibrary;
    CHECKFALSE(ParseString(documentRoot, "CollisionDetection", collisionDetectionLibrary));
    LOG_INTERN(LogLevel::DebugCore) << "collisionDetectionLibrary: " << collisionDetectionLibrary;

    // parse random seed
    unsigned long randomSeed;
    CHECKFALSE(ParseULong(documentRoot, "RandomSeed", randomSeed));
    LOG_INTERN(LogLevel::DebugCore) << "random seed: " << randomSeed;

    // parse weekday
    std::string weekdayStr;
    CHECKFALSE(ParseString(documentRoot, "Weekday", weekdayStr));

    Weekday weekday = Weekday::Undefined;
    if (0 == weekdayStr.compare("Monday"))
    {
        weekday = Weekday::Monday;
    }
    else if (0 == weekdayStr.compare("Tuesday"))
    {
        weekday = Weekday::Tuesday;
    }
    else if (0 == weekdayStr.compare("Wednesday"))
    {
        weekday = Weekday::Wednesday;
    }
    else if (0 == weekdayStr.compare("Thursday"))
    {
        weekday = Weekday::Thursday;
    }
    else if (0 == weekdayStr.compare("Friday"))
    {
        weekday = Weekday::Friday;
    }
    else if (0 == weekdayStr.compare("Saturday"))
    {
        weekday = Weekday::Saturday;
    }
    else if (0 == weekdayStr.compare("Sunday"))
    {
        weekday = Weekday::Sunday;
    }
    else
    {
        weekdayStr = "Undefined";
    }

    LOG_INTERN(LogLevel::DebugCore) << "weekday: " << weekdayStr;

    // parse time of day
    int timeOfDay;
    CHECKFALSE(ParseInt(documentRoot, "TimeOfDay", timeOfDay));
    LOG_INTERN(LogLevel::DebugCore) << "time of day: " << timeOfDay;

    std::string worldLibraryName;
    CHECKFALSE(ParseString(documentRoot, "WorldLibrary", worldLibraryName));
    LOG_INTERN(LogLevel::DebugCore) << "WorldLibraryName: " << worldLibraryName;

    // create configuration
    WorldParameter *worldParameter = new (std::nothrow) WorldParameter(weekday, timeOfDay,
                                                                       worldLibraryName);
    CHECKFALSE(worldParameter);

    RunConfig::StochasticsInstance *stochasticInstance = new (std::nothrow)
    RunConfig::StochasticsInstance(stochasticsLibrary);
    CHECKFALSE(stochasticInstance);

    RunConfig::CollisionDetectionInstance *collisionDetectionInstance = new (
        std::nothrow) RunConfig::CollisionDetectionInstance(collisionDetectionLibrary);
    CHECKFALSE(collisionDetectionInstance);

    std::unique_ptr<RunConfig> configuration(new (std::nothrow) RunConfig(startTime,
                                                                          endTime,
                                                                          numberInvocations,
                                                                          (std::uint32_t)randomSeed,
                                                                          worldParameter,
                                                                          stochasticInstance,
                                                                          collisionDetectionInstance));
    if (!configuration)
    {
        delete worldParameter;
        delete stochasticInstance;
        delete collisionDetectionInstance;
        return nullptr;
    }

    // worldParameter is now released by destructor of RunConfig (no explicite delete necessary)

    // parse spawn points
    QDomElement spawnPointsElement;
    CHECKFALSE(GetFirstChildElement(documentRoot, "SpawnPoints", spawnPointsElement));

    QDomElement spawnPointElement;
    if (GetFirstChildElement(spawnPointsElement, "SpawnPoint", spawnPointElement))
    {
        while (!spawnPointElement.isNull())
        {
            int spawnPointId;
            CHECKFALSE(ParseAttributeInt(spawnPointElement, "id", spawnPointId));

            std::string libraryName;
            CHECKFALSE(ParseAttributeString(spawnPointElement, "library", libraryName));

            LOG_INTERN(LogLevel::DebugCore) << "import spawn point: library " << libraryName;

            RunConfig::SpawnPointInstance *spawnPointInstance = new (std::nothrow)
            RunConfig::SpawnPointInstance(spawnPointId,
                                          libraryName);
            CHECKFALSE(spawnPointInstance);
            CHECKFALSE(configuration->AddSpawnPointInstance(spawnPointInstance));

            // parse agent references
            QDomElement agentRefsElement;
            CHECKFALSE(GetFirstChildElement(spawnPointElement, "AgentRefs", agentRefsElement));

            QDomElement agentRefElement;
            if (GetFirstChildElement(agentRefsElement, "AgentRef", agentRefElement))
            {
                while (!agentRefElement.isNull())
                {
                    int agentRef;
                    CHECKFALSE(ParseCurrentInt(agentRefElement, agentRef));

                    LOG_INTERN(LogLevel::DebugCore) << "   - agent ref " << agentRef;
                    spawnPointInstance->AddAgentRef(agentRef);

                    agentRefElement = agentRefElement.nextSiblingElement("AgentRef");
                } // agent ref loop
            } // if agent refs exist

            // parse parameters
            LOG_INTERN(LogLevel::DebugCore) << "import spawn point parameters...";

            QDomElement parametersElement;
            CHECKFALSE(GetFirstChildElement(spawnPointElement, "SpawnPointParameters", parametersElement));

            CHECKFALSE(ImportParameters(parametersElement, spawnPointInstance->GetSpawnPointParameters()));

            spawnPointElement = spawnPointElement.nextSiblingElement("SpawnPoint");
        } // spawn point loop
    } // if spawn points exist

    // parse agents
    QDomElement agentsElement;
    CHECKFALSE(GetFirstChildElement(documentRoot, "Agents", agentsElement));

    QDomElement agentElement;
    if (GetFirstChildElement(agentsElement, "Agent", agentElement))
    {
        while (!agentElement.isNull())
        {
            int agentId;
            CHECKFALSE(ParseAttributeInt(agentElement, "id", agentId));

            int agentRef;
            CHECKFALSE(ParseInt(agentElement, "AgentTypeRef", agentRef));

            std::string vehicleTypeStr;
            CHECKFALSE(ParseString(agentElement, "Type", vehicleTypeStr));

            AgentVehicleType vehicleType = AgentVehicleType::Undefined;
            if (0 == vehicleTypeStr.compare("Car"))
            {
                vehicleType = AgentVehicleType::Car;
            }
            else if (0 == vehicleTypeStr.compare("Pedestrian"))
            {
                vehicleType = AgentVehicleType::Pedestrian;
            }
            else if (0 == vehicleTypeStr.compare("Motorbike"))
            {
                vehicleType = AgentVehicleType::Motorbike;
            }
            else if (0 == vehicleTypeStr.compare("Bicycle"))
            {
                vehicleType = AgentVehicleType::Bicycle;
            }
            else if (0 == vehicleTypeStr.compare("Truck"))
            {
                vehicleType = AgentVehicleType::Truck;
            }
            else
            {
                vehicleTypeStr = "Undefined";
            }

            double width;
            CHECKFALSE(ParseDouble(agentElement, "Width", width));

            double length;
            CHECKFALSE(ParseDouble(agentElement, "Length", length));

            double distanceCOGtoFrontAxle;
            CHECKFALSE(ParseDouble(agentElement, "DistanceCOGtoFrontAxle", distanceCOGtoFrontAxle));

            double weight;
            CHECKFALSE(ParseDouble(agentElement, "Weight", weight));

            double heightCOG;
            CHECKFALSE(ParseDouble(agentElement, "HeightCOG", heightCOG));

            double wheelbase;
            CHECKFALSE(ParseDouble(agentElement, "Wheelbase", wheelbase));

            double momentInertiaRoll;
            CHECKFALSE(ParseDouble(agentElement, "MomentInertiaRoll", momentInertiaRoll));

            double momentInertiaPitch;
            CHECKFALSE(ParseDouble(agentElement, "MomentInertiaPitch", momentInertiaPitch));

            double momentInertiaYaw;
            CHECKFALSE(ParseDouble(agentElement, "MomentInertiaYaw", momentInertiaYaw));

            double frictionCoeff;
            CHECKFALSE(ParseDouble(agentElement, "FrictionCoeff", frictionCoeff));

            double trackWidth;
            CHECKFALSE(ParseDouble(agentElement, "TrackWidth", trackWidth));

            double distanceCOGtoLeadingEdge;
            CHECKFALSE(ParseDouble(agentElement, "DistanceCOGtoLeadingEdge", distanceCOGtoLeadingEdge));

            LOG_INTERN(LogLevel::DebugCore) << "import agent library " << agentRef << ", vehicle type " <<
                                            vehicleTypeStr;

            AgentSpawnItem *agentSpawnItem = new (std::nothrow) AgentSpawnItem(agentId, agentRef);
            CHECKFALSE(agentSpawnItem);

            agentSpawnItem->SetVehicleType(vehicleType);
            agentSpawnItem->SetWidth(width);
            agentSpawnItem->SetLength(length);
            agentSpawnItem->SetDistanceCOGtoFrontAxle(distanceCOGtoFrontAxle);
            agentSpawnItem->SetWeight(weight);
            agentSpawnItem->SetHeightCOG(heightCOG);
            agentSpawnItem->SetWheelbase(wheelbase);
            agentSpawnItem->SetMomentInertiaRoll(momentInertiaRoll);
            agentSpawnItem->SetMomentInertiaPitch(momentInertiaPitch);
            agentSpawnItem->SetMomentInertiaYaw(momentInertiaYaw);
            agentSpawnItem->SetFrictionCoeff(frictionCoeff);
            agentSpawnItem->SetTrackWidth(trackWidth);
            agentSpawnItem->SetDistanceCOGtoLeadingEdge(distanceCOGtoLeadingEdge);

            CHECKFALSE(configuration->AddAgentSpawnItem(agentSpawnItem));

            agentElement = agentElement.nextSiblingElement("Agent");
        } // agent loop
    } // if agents exist

    // parse observation modules
    QDomElement observationsElement;
    CHECKFALSE(GetFirstChildElement(documentRoot, "Observations", observationsElement));

    CHECKFALSE(ImportObservations(observationsElement, *configuration.get()));

    return configuration.release();
}

} // namespace SimulationCommon
