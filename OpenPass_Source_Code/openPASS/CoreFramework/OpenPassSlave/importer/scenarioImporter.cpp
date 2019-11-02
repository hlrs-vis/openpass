/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <memory>
#include <iostream>
#include <queue>
#include <QDomDocument>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "CoreFramework/CoreShare/log.h"
#include "CoreFramework/CoreShare/parameters.h"
#include "scenario.h"
#include "scenarioImporter.h"
#include "xmlParser.h"
#include "manipulatorImporter.h"

// local helper macros
#define CHECKFALSE(element) \
    do { \
        if (!(element)) \
        { \
            throw std::runtime_error("Checkfalse in scenario importer failed"); \
        } \
    } \
    while (0);

namespace Importer {

bool ScenarioImporter::Import(const std::string& filename, ScenarioInterface* scenario)
{
    std::locale::global(std::locale("C"));

    QFile xmlFile(filename.c_str()); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Warning) << "an error occurred during scenario import";
        throw std::runtime_error("Could not open scenario (" + filename + ")");
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    QString errorMsg;
    int errorLine;
    if (!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml format: " << filename;
        LOG_INTERN(LogLevel::Warning) << "in line " << errorLine << ": " << errorMsg.toStdString();
        throw std::runtime_error("Invalid xml format (" + filename + ")");
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        throw std::runtime_error("Scenario xml has no document root");
    }

    if (!ImportAndValidateVersion(documentRoot))
    {
        return false;
    }

    std::string sceneryPath;
    ImportRoadNetwork(documentRoot, sceneryPath);
    scenario->SetSceneryPath(sceneryPath);

    ImportCatalogs(documentRoot, scenario);

    std::vector<ScenarioEntity> entities;
    std::map<std::string, std::list<std::string>> groups;
    if (!ImportEntities(documentRoot, entities, groups))
    {
        LOG_INTERN(LogLevel::Error) << "Unable to import entities from scenario file";
        return false;
    }

    ImportStoryboard(documentRoot, entities, scenario);
    CategorizeEntities(entities, groups, scenario);

    return true;
}

bool ScenarioImporter::ImportAndValidateVersion(QDomElement& documentRoot)
{
    SimulationCommon::Parameters scenarioParameters;

    QDomElement parameterDeclarationElement;

    if (SimulationCommon::GetFirstChildElement(documentRoot, "ParameterDeclaration", parameterDeclarationElement))
    {
        ImportParameterDeclarationElement(parameterDeclarationElement, &scenarioParameters);
    }

    const auto version = scenarioParameters.GetParametersString().find("OP_OSC_SchemaVersion");

    if (version == scenarioParameters.GetParametersString().cend())
    {
        LOG_INTERN(LogLevel::Error) << "Cannot determine scenario version";
        return false;
    }
    else
        if (version->second.compare(supportedScenarioVersion) != 0)
        {
            LOG_INTERN(LogLevel::Error) << "Scenario version not supported (" << version->second << "). Supported version is " <<
                                        supportedScenarioVersion;
            return false;
        }

    return true;
}

void ScenarioImporter::ImportCatalogs(QDomElement& documentRoot, ScenarioInterface* scenario)
{
    std::string catalogPath;

    QDomElement catalogsElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(documentRoot, "Catalogs", catalogsElement));

    if (ImportCatalog(catalogPath, "VehicleCatalog", catalogsElement))
    {
        scenario->SetVehicleCatalogPath(catalogPath);
    }

    if (ImportCatalog(catalogPath, "PedestrianCatalog", catalogsElement))
    {
        scenario->SetPedestrianCatalogPath(catalogPath);
    }
}

void ScenarioImporter::ImportRoadNetwork(QDomElement& documentRoot, std::string& sceneryPath)
{
    QDomElement roadNetworkElement;
    QDomElement logicsElement;

    CHECKFALSE(SimulationCommon::GetFirstChildElement(documentRoot, "RoadNetwork", roadNetworkElement))
    CHECKFALSE(SimulationCommon::GetFirstChildElement(roadNetworkElement, "Logics", logicsElement))
    CHECKFALSE(SimulationCommon::ParseAttributeString(logicsElement, "filepath", sceneryPath));
}

void ScenarioImporter::ImportStoryboard(QDomElement& documentRoot, std::vector<ScenarioEntity>& entities,
                                        ScenarioInterface* scenario)
{
    QDomElement storyboardElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(documentRoot, "Storyboard", storyboardElement))

    //Import Init
    QDomElement initElement;
    // for initial entitiy parameters we just use first child "Init" --> others will be ignore
    CHECKFALSE(SimulationCommon::GetFirstChildElement(storyboardElement, "Init", initElement))
    ImportInitElement(initElement, entities, scenario);

    // Import Story
    QDomElement storyElement;
    if (SimulationCommon::GetFirstChildElement(storyboardElement, "Story", storyElement))
    {
        ImportStoryElement(storyElement, entities, scenario);
    }

    // Import EndCondition
    ImportEndConditionsFromStoryboard(storyboardElement, scenario);
}

void ScenarioImporter::ImportStoryElement(QDomElement& storyElement, const std::vector<ScenarioEntity>& entities,
        ScenarioInterface* scenario)
{
    // none of these tags are urgently required
    QDomElement actElement;
    if (SimulationCommon::GetFirstChildElement(storyElement, "Act", actElement))
    {
        while (!actElement.isNull())
        {
            QDomElement seqElement;
            if (SimulationCommon::GetFirstChildElement(actElement, "Sequence", seqElement))
            {
                while (!seqElement.isNull())
                {
                    std::string sequenceName;
                    SimulationCommon::ParseAttributeString(seqElement, "name", sequenceName);

                    QDomElement actorsElement;
                    std::vector<std::string> actors;
                    if (SimulationCommon::GetFirstChildElement(seqElement, "Actors", actorsElement)) // just one actors per sequence
                    {
                        actors = ImportActors(actorsElement, entities);
                    }

                    QDomElement maneuverElement;
                    if (SimulationCommon::GetFirstChildElement(seqElement, "Maneuver", maneuverElement)) // just one maneuver per sequence
                    {
                        ImportManeuverElement(maneuverElement, entities, scenario, sequenceName, actors);
                    }

                    seqElement = seqElement.nextSiblingElement("Sequence");
                }
            }
            actElement = actElement.nextSiblingElement("Act");
        }
    }
}

void ScenarioImporter::ImportManeuverElement(QDomElement& maneuverElement, const std::vector<ScenarioEntity>& entities,
        ScenarioInterface* scenario, std::string& sequenceName, std::vector<std::string>& actors)
{
    // handle parameterdeclaration element(s) for event(s)
    QDomElement parameterDeclarationElement;
    SimulationCommon::GetFirstChildElement(maneuverElement, "ParameterDeclaration", parameterDeclarationElement);

    std::queue<SimulationCommon::EventDetectorParameters> eventDetectorParametersList;
    while (!parameterDeclarationElement.isNull())
    {
        SimulationCommon::EventDetectorParameters eventDetectorParameters;
        ImportParameterDeclarationElement(parameterDeclarationElement, &eventDetectorParameters);
        eventDetectorParametersList.push(eventDetectorParameters);

        parameterDeclarationElement = parameterDeclarationElement.nextSiblingElement("ParameterDeclaration");
    }

    // handle event element
    QDomElement eventElement;
    SimulationCommon::GetFirstChildElement(maneuverElement, "Event", eventElement);
    while (!eventElement.isNull())
    {
        SimulationCommon::EventDetectorParameters eventDetectorParameters;
        //EventDetector
        if (!eventDetectorParametersList.empty())
        {
            eventDetectorParameters = eventDetectorParametersList.front();
            eventDetectorParametersList.pop();
        }
        else
        {
            eventDetectorParameters = SimulationCommon::EventDetectorParameters();
        }
        eventDetectorParameters.AddParameterStringVector("Actors", actors);
        std::vector<std::string> eventDetectorNames;
        ParseEventDetectorData(eventElement,
                             entities,
                             scenario,
                             eventDetectorNames,
                             sequenceName,
                             &eventDetectorParameters);

        std::shared_ptr<ScenarioActionInterface> action = ManipulatorImporter::ImportManipulator(eventElement,
                                                                                         eventDetectorNames,
                                                                                         actors,
                                                                                         sequenceName);
        if (action.get() != nullptr)
        {
            scenario->AddAction(action);
        }
        else
        {
            ParseManipulatorData(eventElement, scenario, eventDetectorNames, sequenceName, actors);
        }

        eventElement = eventElement.nextSiblingElement("Event");
    }
}

void ScenarioImporter::ImportLongitudinalElement(ScenarioEntity& scenarioEntity, QDomElement firstChildOfActionElement)
{
    QDomElement speedElement;
    if (SimulationCommon::GetFirstChildElement(firstChildOfActionElement, "Speed", speedElement))
    {
        QDomElement dynamicsElement;
        if (SimulationCommon::GetFirstChildElement(speedElement, "Dynamics", dynamicsElement))
        {
            double rate;
            if (SimulationCommon::ParseAttributeDouble(dynamicsElement, "rate", rate))
            {
                scenarioEntity.spawnInfo.acceleration.value = rate;
            }
        }

        // Handle <Target> attributes
        QDomElement targetElement;
        if (SimulationCommon::GetFirstChildElement(speedElement, "Target", targetElement))
        {
            // Handle <Target> internal tags - currently ignoring <Relative> tags
            QDomElement absoluteElement;
            if (SimulationCommon::GetFirstChildElement(targetElement, "Absolute", absoluteElement))
            {
                double velocity;
                if (SimulationCommon::ParseAttributeDouble(absoluteElement, "value", velocity))
                {
                    scenarioEntity.spawnInfo.velocity.value = velocity;
                }
            }
        }

        // Parse stochastics if available
        QDomElement stochasticElement;
        SimulationCommon::GetFirstChildElement(speedElement, "Stochastics", stochasticElement);
        while (!stochasticElement.isNull())
        {
           SetStochasticsData(scenarioEntity, stochasticElement);
           stochasticElement = stochasticElement.nextSiblingElement("Stochastics");

        }
    }
}

void ScenarioImporter::ImportPositionElement(ScenarioEntity& scenarioEntity, QDomElement firstChildOfActionElement)
{
    std::string roadId = "";
    int laneId;
    double s;
    double offset;

    QDomElement laneElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(firstChildOfActionElement, "Lane", laneElement))
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(laneElement, "s", s));
    CHECKFALSE(SimulationCommon::ParseAttributeInt(laneElement, "laneId", laneId));
    CHECKFALSE(SimulationCommon::ParseAttributeString(laneElement, "roadId", roadId));

    SpawnInfo& spawnInfo = scenarioEntity.spawnInfo;
    spawnInfo.s.value = s;
    spawnInfo.TStart = 0; // always presimulation spawning
    spawnInfo.ILane = laneId;
    spawnInfo.roadId = roadId;

    bool offsetAvailable = SimulationCommon::ParseAttributeDouble(laneElement, "offset", offset);
    if (offsetAvailable)
    {
        spawnInfo.offset.value = offset;
    }

    QDomElement stochasticElement;
    SimulationCommon::GetFirstChildElement(laneElement, "Stochastics", stochasticElement);
    while (!stochasticElement.isNull())
    {
        SetStochasticsData(scenarioEntity, stochasticElement);
        stochasticElement = stochasticElement.nextSiblingElement("Stochastics");
    }

    QDomElement orientationElement;
    if (SimulationCommon::GetFirstChildElement(laneElement, "Orientation", orientationElement))
    {
        SetOrientationData(scenarioEntity, orientationElement);
    }
}

void ScenarioImporter::SetStochasticsDataHelper(SpawnAttribute& attribute, QDomElement& stochasticsElement)
{
    double stdDeviation;
    double lowerBound;
    double upperBound;

    CHECKFALSE(SimulationCommon::ParseAttributeDouble(stochasticsElement, "stdDeviation", stdDeviation));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(stochasticsElement, "lowerBound", lowerBound));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(stochasticsElement, "upperBound", upperBound));

    attribute.mean = attribute.value;
    attribute.isStochastic = true;
    attribute.stdDeviation = stdDeviation;
    attribute.lowerBoundary = lowerBound;
    attribute.upperBoundary = upperBound;
}

void ScenarioImporter::SetStochasticsData(ScenarioEntity& scenarioEntity, QDomElement& stochasticsElement)
{
    SpawnInfo& spawnInfo = scenarioEntity.spawnInfo;

    std::string type;
    CHECKFALSE(SimulationCommon::ParseAttributeString(stochasticsElement, "value", type));

    if (type == "offset")
    {
        SetStochasticsDataHelper(spawnInfo.offset, stochasticsElement);
    }
    else
        if (type == "s")
        {
            SetStochasticsDataHelper(spawnInfo.s, stochasticsElement);
        }
        else
            if (type == "velocity")
            {
                SetStochasticsDataHelper(spawnInfo.velocity, stochasticsElement);
            }
            else
                if (type == "rate")
                {
                    SetStochasticsDataHelper(spawnInfo.acceleration, stochasticsElement);
                }
}

void ScenarioImporter::SetOrientationData(ScenarioEntity& scenarioEntity, QDomElement& orientationElement)
{
    std::string type;
    double heading;

    CHECKFALSE(SimulationCommon::ParseAttributeString(orientationElement, "type", type));
    if (type != "relative")
    {
        LOG_INTERN(LogLevel::Warning) << "Scenario Importer: only relative orientation is allowed.";
        throw std::runtime_error("Scenario Importer: only relative orientation is allowed.");
    }

    CHECKFALSE(SimulationCommon::ParseAttributeDouble(orientationElement, "h", heading));

    scenarioEntity.spawnInfo.heading = heading;
}

void ScenarioImporter::ImportPrivateElement(QDomElement& privateElement,
        std::vector<ScenarioEntity>& entities,
        ScenarioInterface* scenario)
{
    std::string object;
    CHECKFALSE(SimulationCommon::ParseAttributeString(privateElement, "object", object));

    ScenarioEntity* scenarioEntity = GetEntityByName(entities, object);

    if (scenarioEntity == nullptr)
    {
        throw std::runtime_error(std::string("Action object '") + object + "' not declared in 'Entities'");
    }

    QDomElement actionElement;
    SimulationCommon::GetFirstChildElement(privateElement, "Action", actionElement);

    while (!actionElement.isNull())
    {
        QDomElement firstChildOfActionElement;
        if (SimulationCommon::GetFirstChildElement(actionElement, "Longitudinal", firstChildOfActionElement))
        {
            ImportLongitudinalElement(*scenarioEntity, firstChildOfActionElement);
        }
        else
            if (SimulationCommon::GetFirstChildElement(actionElement, "Position", firstChildOfActionElement))
            {
                ImportPositionElement(*scenarioEntity, firstChildOfActionElement);
            }

        actionElement = actionElement.nextSiblingElement("Action");
    }

    try
    {
        ValidityCheckForSpawnParameters(*scenarioEntity);
    }
    catch (const std::runtime_error& error)
    {
        LOG_INTERN(LogLevel::Warning) << "error in importInitElement: " << error.what();
        throw std::runtime_error("Import scenario xml failed.");
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Warning) << "An unexpected error occured.";
        throw std::runtime_error("Import scenario xml failed.");
    }
}

void ScenarioImporter::ImportPrivateElements(QDomElement& actionsElement, std::vector<ScenarioEntity>& entities,
        ScenarioInterface* scenario)
{
    QDomElement privateElement;
    SimulationCommon::GetFirstChildElement(actionsElement, "Private", privateElement);

    while (!privateElement.isNull())
    {
        ImportPrivateElement(privateElement, entities, scenario);
        privateElement = privateElement.nextSiblingElement("Private");
    }
}

void ScenarioImporter::ImportInitElement(QDomElement& initElement, std::vector<ScenarioEntity>& entities,
        ScenarioInterface* scenario)
{
    // for initial entitiy parameters we just use first child "Actions" --> others will be ignore
    QDomElement actionsElement;
    SimulationCommon::GetFirstChildElement(initElement, "Actions", actionsElement);

    ImportPrivateElements(actionsElement, entities, scenario);
}

void ScenarioImporter::ImportEndConditionsFromStoryboard(const QDomElement& storyboardElement, ScenarioInterface* scenario)
{
    bool endConditionProvided = false;

    QDomElement endConditionsElement;
    if (SimulationCommon::GetFirstChildElement(storyboardElement, "EndConditions", endConditionsElement))
    {
        QDomElement conditionGroupElement;
        if (SimulationCommon::GetFirstChildElement(endConditionsElement, "ConditionGroup", conditionGroupElement))
        {
            QDomElement conditionElement;
            if (SimulationCommon::GetFirstChildElement(conditionGroupElement, "Condition", conditionElement))
            {
                // these attributes are required by OpenSCENARIO standard, but have no impact on behaviour
                std::string conditionName;
                double conditionDelay;
                std::string conditionEdge;

                ParseConditionAttributes(conditionElement, conditionName, conditionDelay, conditionEdge);
                if (conditionDelay > 0.0)
                {
                    LOG_INTERN(LogLevel::Warning) << "Condition delay attribute not equal to zero not currently supported";
                    throw std::runtime_error("End Condition specifies unsupported delay value");
                }

                if (conditionEdge != "rising")
                {
                    LOG_INTERN(LogLevel::Warning) << "Condition edge attribute not equal to 'rising' not currently supported";
                    throw std::runtime_error("End Condition specifies unsupported edge value");
                }

                QDomElement byValueElement;
                if (SimulationCommon::GetFirstChildElement(conditionElement, "ByValue", byValueElement))
                {
                    double endTime;
                    std::string rule;

                    ParseSimulationTime(byValueElement, endTime, rule);
                    if (rule == "less_than" || rule == "equal_to")
                    {
                        LOG_INTERN(LogLevel::Warning) << "SimulationTime rule attribute value '" << rule << "' not supported; defaulting to 'greater_than'";
                        throw std::runtime_error("End Condition specifies unsupported rule");
                    }
                    scenario->SetEndTime(endTime);
                    endConditionProvided = true;
                }
            }
        }
    }

    if (!endConditionProvided)
    {
        // error message
        throw std::runtime_error("Scenario provides no EndConditions for the simulation");
    }
}

void ScenarioImporter::ParseConditionAttributes(const QDomElement& conditionElement, std::string& name, double& delay, std::string& edge)
{
    if (!SimulationCommon::ParseAttributeString(conditionElement, "name", name))
    {
        LOG_INTERN(LogLevel::Error) << "Condition name required";
        throw std::runtime_error("No name specified for condition");
    }

    if (!SimulationCommon::ParseAttributeDouble(conditionElement, "delay", delay))
    {
        LOG_INTERN(LogLevel::Error) << "Condition delay required";
        throw std::runtime_error("No delay attribute specified for condition " + name);
    }
    else if (delay < 0.0)
    {
        LOG_INTERN(LogLevel::Error) << "Negative condition delay not valid";
        throw std::runtime_error("Invalid delay value specified for condition " + name);
    }

    if (!SimulationCommon::ParseAttributeString(conditionElement, "edge", edge))
    {
        LOG_INTERN(LogLevel::Error) << "Condition edge required";
        throw std::runtime_error("No edge specified for condition " + name);
    }
}

void ScenarioImporter::ParseSimulationTime(const QDomElement& byValueElement, double& value, std::string& rule)
{
    QDomElement simulationTimeElement;
    if(SimulationCommon::GetFirstChildElement(byValueElement, "SimulationTime", simulationTimeElement))
    {
        if(SimulationCommon::ParseAttributeDouble(simulationTimeElement, "value", value))
        {
            if(SimulationCommon::ParseAttributeString(simulationTimeElement, "rule", rule))
            {
                if(!(rule == "greater_than" || rule == "less_than" || rule == "equal_to"))
                {
                    LOG_INTERN(LogLevel::Error) << "SimulationTime rule attribute value '" << rule << "' not valid";
                    throw std::runtime_error("Invalid rule specified for SimulationTime element");
                }
            }
        }
        else
        {
            LOG_INTERN(LogLevel::Error) << "SimulationTime value attribute not valid";
            throw std::runtime_error("Invalid SimulationTime element");
        }
    }
}

void ScenarioImporter::AddToEntities(const ScenarioEntity& scenarioEntity, ScenarioInterface* scenario)
{
    if (scenarioEntity.name == "Ego")
    {
        scenario->SetEgoEntity(scenarioEntity);
    }
    else
    {
        scenario->AddScenarioEntity(scenarioEntity);
    }
}

void ScenarioImporter::ValidityCheckForSpawnParameters(const ScenarioEntity& scenarioEntity)
{
    if (scenarioEntity.spawnInfo.s.value == -999 || scenarioEntity.spawnInfo.TStart == -999)
    {
        throw std::runtime_error("no position information available");
    }
}

void ScenarioImporter::ImportByEntityElement(QDomElement& byEntityElement, const std::vector<ScenarioEntity>& entities,
        ParameterInterface* eventDetectorParameters)
{
    QDomElement triggeringEntitiesElement;
    if (SimulationCommon::GetFirstChildElement(byEntityElement, "TriggeringEntities", triggeringEntitiesElement))
    {
        std::string rule;
        if (SimulationCommon::ParseAttributeString(triggeringEntitiesElement, "rule", rule))
        {
            eventDetectorParameters->AddParameterString("AgentGroup", rule);
        }

        QDomElement entityElement;
        SimulationCommon::GetFirstChildElement(triggeringEntitiesElement, "Entity", entityElement);

        std::vector<std::string> triggeringAgents;
        while (!entityElement.isNull())
        {
            std::string entityName;
            CHECKFALSE(SimulationCommon::ParseAttributeString(entityElement, "name", entityName));

            if (!ContainsEntity(entities, entityName))
            {
                throw std::runtime_error(std::string("TriggeringEntity '") + entityName + "' not declared in 'Entities'");
            }

            triggeringAgents.push_back(entityName);
            entityElement = entityElement.nextSiblingElement("Entity");
        }

        eventDetectorParameters->AddParameterStringVector("TriggeringAgents", triggeringAgents);
    }
}

void ScenarioImporter::ImportConditionByValueElement(QDomElement& byValueElement, ParameterInterface &conditionParameters)
{
    QDomElement simulationTimeElement;
    if (SimulationCommon::GetFirstChildElement(byValueElement, "SimulationTime", simulationTimeElement))
    {
        double value;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(simulationTimeElement, "value", value));

        std::string rule;
        CHECKFALSE(SimulationCommon::ParseAttributeString(simulationTimeElement, "rule", rule));

        conditionParameters.AddParameterDouble("value", value);
        conditionParameters.AddParameterString("rule", rule);
        conditionParameters.AddParameterString("type", "SimulationTimeCondition");
    }
}

void ScenarioImporter::ImportParameterElement(QDomElement& parameterElement, ParameterInterface *parameters)
{
    std::string parameterName;
    CHECKFALSE(SimulationCommon::ParseAttributeString(parameterElement, "name", parameterName));

    std::string parameterType;
    CHECKFALSE(SimulationCommon::ParseAttributeString(parameterElement, "type", parameterType));

    switch (parameterTypes.at(parameterType))
    {
        case 0:
            bool parameterValueBool;
            CHECKFALSE(SimulationCommon::ParseAttributeBool(parameterElement, "value", parameterValueBool));
            parameters->AddParameterBool(parameterName, parameterValueBool);
            break;

        case 1:
            int parameterValueInt;
            CHECKFALSE(SimulationCommon::ParseAttributeInt(parameterElement, "value", parameterValueInt));
            parameters->AddParameterInt(parameterName, parameterValueInt);
            break;

        case 2:
            double parameterValueDouble;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(parameterElement, "value", parameterValueDouble));
            parameters->AddParameterDouble(parameterName, parameterValueDouble);
            break;

        case 3:
            std::string parameterValueString;
            CHECKFALSE(SimulationCommon::ParseAttributeString(parameterElement, "value", parameterValueString));
            parameters->AddParameterString(parameterName, parameterValueString);
            break;
    }
}

void ScenarioImporter::ImportParameterDeclarationElement(QDomElement& parameterDeclarationElement,
        ParameterInterface* parameters)
{
    QDomElement parameterElement;
    if (SimulationCommon::GetFirstChildElement(parameterDeclarationElement, "Parameter", parameterElement))
    {
        while (!parameterElement.isNull())
        {
            ImportParameterElement(parameterElement, parameters);

            parameterElement = parameterElement.nextSiblingElement("Parameter");
        }
    }
}

void ScenarioImporter::ParseManipulatorData(QDomElement& eventElement, ScenarioInterface* scenario,
        std::vector<std::string>& eventDetectorNames, std::string& seqName, std::vector<std::string>& actors)
{
    QDomElement actionElement;
    if (SimulationCommon::GetFirstChildElement(eventElement, "Action", actionElement))
    {
        std::string manipulatorName;
        CHECKFALSE(SimulationCommon::ParseAttributeString(actionElement, "name", manipulatorName));

        //Parse manipulator parameters
        SimulationCommon::ManipulatorParameters manipulatorParameters;

        const std::vector<std::string> eventDetectorNamesVector = eventDetectorNames;
        manipulatorParameters.AddParameterStringVector("EventDetector", eventDetectorNamesVector);
        manipulatorParameters.AddParameterString("SequenceName", seqName);
        manipulatorParameters.AddParameterStringVector("Actors", actors);

        QDomElement parameterDeclarationElement;
        if (SimulationCommon::GetFirstChildElement(actionElement, "ParameterDeclaration", parameterDeclarationElement))
        {
            ImportParameterDeclarationElement(parameterDeclarationElement, &manipulatorParameters);
        }

        scenario->AddManipulator(manipulatorName, manipulatorParameters);
    }
}

std::string ScenarioImporter::ImportConditionElement(QDomElement& conditionElement, const std::vector<ScenarioEntity>& entities, ScenarioInterface *scenario, std::string &seqName, ParameterInterface *parameters)
{
    //Parse event detector name
    std::string eventDetectorType;
    CHECKFALSE(SimulationCommon::ParseAttributeString(conditionElement, "name", eventDetectorType));

    //Parse event detector parameters
    parameters->AddParameterString("SequenceName", seqName);

    QDomElement byEntityElement;
    //Parse specific entity
    if (SimulationCommon::GetFirstChildElement(conditionElement, "ByEntity", byEntityElement))
    {
        ImportByEntityElement(byEntityElement, entities, parameters);
    }

    QDomElement parameterDeclarationElement;
    if (SimulationCommon::GetFirstChildElement(conditionElement, "ParameterDeclaration", parameterDeclarationElement))
    {
        ImportParameterDeclarationElement(parameterDeclarationElement, parameters);
    }

    // handle children elements (conditions)

    QDomElement byValueElement;
    if (SimulationCommon::GetFirstChildElement(conditionElement, "ByValue", byValueElement))
    {
        ParameterInterface &conditionParameters = parameters->InitializeListItem("conditions");
        ImportConditionByValueElement(byValueElement, conditionParameters);
    }

    // SimulationCommon::EventDetectorParameters eventDetectorParameters = *(static_cast<SimulationCommon::EventDetectorParameters*>(parameters));
    //Add event detector to the scenario
    scenario->AddEventDetector(eventDetectorType, *dynamic_cast<SimulationCommon::EventDetectorParameters*>(parameters));

    return eventDetectorType;
}

void ScenarioImporter::ImportStartConditionsElement(QDomElement& startConditionsElement, const std::vector<ScenarioEntity>& entities, ScenarioInterface *scenario, std::vector<std::string> &eventDetecorNames, std::string &seqName, ParameterInterface *parameters)
{
    QDomElement conditionGroupElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(startConditionsElement, "ConditionGroup", conditionGroupElement));

    QDomElement conditionElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(conditionGroupElement, "Condition", conditionElement));

    while (!conditionElement.isNull())
    {
        std::string eventDetectorName = ImportConditionElement(conditionElement, entities, scenario, seqName, parameters);
        eventDetecorNames.push_back(eventDetectorName);
        conditionElement = conditionElement.nextSiblingElement("Condition");
    }
}

void ScenarioImporter::ParseEventDetectorData(QDomElement& eventElement, const std::vector<ScenarioEntity>& entities, ScenarioInterface *scenario, std::vector<std::string> &eventDetectorNames, std::string &seqName, ParameterInterface *parameters)
{
    //once per event
    QDomElement startConditionsElement;
    if (SimulationCommon::GetFirstChildElement(eventElement, "StartConditions", startConditionsElement))
    {
        while (!startConditionsElement.isNull())
        {
            ImportStartConditionsElement(startConditionsElement, entities, scenario, eventDetectorNames, seqName, parameters);

            startConditionsElement = startConditionsElement.nextSiblingElement("StartConditions");
        }
    }
}

std::vector<std::string> ScenarioImporter::ImportActors(QDomElement& actorsElement,
        const std::vector<ScenarioEntity>& entities)
{
    std::vector<std::string> actors;

    QDomElement entityElement;
    SimulationCommon::GetFirstChildElement(actorsElement, "Entity", entityElement);

    while (!entityElement.isNull())
    {
        std::string entityName;
        if (SimulationCommon::ParseAttributeString(entityElement, "name", entityName))
        {
            if (!ContainsEntity(entities, entityName))
            {
                throw std::runtime_error(std::string("Actor element references entity '") + entityName +
                                         "' which isn't declared in 'Entities'");
            }

            actors.push_back(entityName);
            entityElement = entityElement.nextSiblingElement("Entity");
        }
    }

    return actors;
}

bool ScenarioImporter::ImportCatalogDirectory(std::string& catalogPath, QDomElement& catalogElement)
{
    QDomElement directoryElement;
    SimulationCommon::GetFirstChildElement(catalogElement, "Directory", directoryElement);

    if (directoryElement.isNull())
    {
        LOG_INTERN(LogLevel::Warning) << "Catalog element directory tag missing or invalid";
        return false;
    }

    return SimulationCommon::ParseAttributeString(directoryElement, "path", catalogPath);
}

bool ScenarioImporter::ImportCatalog(std::string& catalogPath, const std::string& catalogName,
                                     QDomElement& catalogsElement)
{
    QDomElement catalogElement;
    SimulationCommon::GetFirstChildElement(catalogsElement, catalogName, catalogElement);

    if (catalogElement.isNull())
    {
        LOG_INTERN(LogLevel::Warning) << "No " << catalogName << " found in scenario file";
        return false;
    }

    return ImportCatalogDirectory(catalogPath, catalogElement);
}

bool ScenarioImporter::ImportEntities(QDomElement& documentRoot, std::vector<ScenarioEntity>& entities, std::map<std::string, std::list<std::string>> &groups)
{
    QDomElement entitiesElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(documentRoot, "Entities", entitiesElement));

    if (entitiesElement.isNull())
    {
        LOG_INTERN(LogLevel::Error) << "'Entities' tag not found in scenario file";
        return false;
    }

    QDomElement entityElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(entitiesElement, "Object", entityElement));

    while (!entityElement.isNull())
    {
        ScenarioEntity entity;

        if (!ImportEntity(entityElement, entity))
        {
            LOG_INTERN(LogLevel::Error) << "Unable to import entity object";
            return false;
        }

        entities.push_back(entity);
        entityElement = entityElement.nextSiblingElement("Object");
    }

    // Parse selection elements (there can be as few as zero and as many as one wants)
    if (!ImportSelectionElements(entitiesElement, groups))
    {
        LOG_INTERN(LogLevel::Error) << "Unable to import selection element";
        return false;
    }

    return true;
}

bool ScenarioImporter::ImportSelectionElements(QDomElement &entitiesElement, std::map<std::string, std::list<std::string>> &groups)
{
    QDomElement selectionElement;
    if(SimulationCommon::GetFirstChildElement(entitiesElement, "Selection", selectionElement))
    {
        std::string selectionName;
        QDomElement membersElement;
        std::list<std::string> members;
        while (!selectionElement.isNull())
        {
            if (!SimulationCommon::ParseAttributeString(selectionElement, "name", selectionName))
            {
                LOG_INTERN(LogLevel::Error) << "No name provided for selection";
                return false;
            }
            // parse members element (just one)
            if(!SimulationCommon::GetFirstChildElement(selectionElement, "Members", membersElement))
            {
                LOG_INTERN(LogLevel::Error) << "No members provided for selection: " << selectionName;
                return false;
            }

            ImportMembers(membersElement, members);

            groups.insert({selectionName, members});

            selectionElement = selectionElement.nextSiblingElement("Selection");
        }
    }

    if (groups.find("ScenarioAgents") == groups.cend())
    {
        LOG_INTERN(LogLevel::Info) << "ScenarioAgents selection is not defined. Adding empty one.";
        groups.insert({"ScenarioAgents", {}});
    }
    return true;
}

bool ScenarioImporter::ImportEntity(QDomElement& entityElement, ScenarioEntity& entity)
{
    if (!SimulationCommon::ParseAttributeString(entityElement, "name", entity.name))
    {
        LOG_INTERN(LogLevel::Error) << "Unable to parse 'name' tag of entity object";
        return false;
    }

    if (entity.name.size() == 0)
    {
        LOG_INTERN(LogLevel::Error) << "Length of entity object name has to be greater than 0";
        return false;
    }

    QDomElement catalogReferenceElement;

    if (!SimulationCommon::GetFirstChildElement(entityElement, "CatalogReference", catalogReferenceElement))
    {
        LOG_INTERN(LogLevel::Error) << "Unable to import entity catalog reference tag";
        return false;
    }

    if (!ImportEntityCatalogReference(catalogReferenceElement, entity))
    {
        LOG_INTERN(LogLevel::Error) << "Invalid catalog reference for entity '" << entity.name << "'";
        return false;
    }

    return true;
}

bool ScenarioImporter::ImportMembers(const QDomElement &membersElement, std::list<std::string> &members)
{
    QDomElement byEntityElement;
    if(SimulationCommon::GetFirstChildElement(membersElement, "ByEntity", byEntityElement))
    {
        std::string memberName;
        while(!byEntityElement.isNull())
        {
            SimulationCommon::ParseAttributeString(byEntityElement, "name", memberName);
            members.push_back(memberName);

            byEntityElement = byEntityElement.nextSiblingElement("ByEntity");
        }
    }

    return true;
}

bool ScenarioImporter::ImportEntityCatalogReference(QDomElement& catalogReferenceElement, ScenarioEntity& entity)
{
    if (!SimulationCommon::ParseAttributeString(catalogReferenceElement, "catalogName",
            entity.catalogReference.catalogName))
    {
        LOG_INTERN(LogLevel::Error) << "Unable to parse 'catalogName' attribute of catalog reference";
        return false;
    }

    if (entity.catalogReference.catalogName.size() == 0)
    {
        LOG_INTERN(LogLevel::Error) << "Length of 'catalogName' has to be greater than 0";
        return false;
    }

    if (!SimulationCommon::ParseAttributeString(catalogReferenceElement, "entryName", entity.catalogReference.entryName))
    {
        LOG_INTERN(LogLevel::Error) << "Unable to parse 'entryName' attribute of catalog reference";
        return false;
    }

    if (entity.catalogReference.entryName.size() == 0)
    {
        LOG_INTERN(LogLevel::Error) << "Length of 'entryName' has to be greater than 0";
        return false;
    }

    return true;
}

ScenarioEntity* ScenarioImporter::GetEntityByName(std::vector<ScenarioEntity>& entities, const std::string& entityName)
{
    auto entitiesFound = std::find_if(entities.begin(),
                                      entities.end(),
                                      [entityName](ScenarioEntity & elem)
    {
        return elem.name == entityName;
    });

    if (entitiesFound != entities.end())
    {
        return &(*entitiesFound);
    }

    return nullptr;
}

bool ScenarioImporter::ContainsEntity(const std::vector<ScenarioEntity>& entities, const std::string& entityName)
{
    auto entitiesFound = std::find_if(entities.cbegin(),
                                      entities.cend(),
                                      [entityName](const ScenarioEntity & elem)
    {
        return elem.name == entityName;
    });

    if (entitiesFound != entities.cend())
    {
        return true;
    }

    return false;
}

void ScenarioImporter::CategorizeEntities(const std::vector<ScenarioEntity>& entities, const std::map<std::string, std::list<std::string>> &groups, ScenarioInterface* scenario)
{
    for (const auto& entity : entities)
    {
        if (entity.name == "Ego")
        {
            scenario->SetEgoEntity(entity);
        }
        scenario->AddScenarioEntity(entity);
    }

    scenario->AddScenarioGroupsByEntityNames(groups);
}
} //namespace Importer
