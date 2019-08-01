/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  SlaveConfigImporter.cpp */
//-----------------------------------------------------------------------------

#include <cmath>

#include "slaveConfigImporter.h"
#include "CoreFramework/CoreShare/log.h"

using namespace Importer;
using namespace SimulationCommon;

std::string SlaveConfigImporter::GetLibrary(const QDomElement& root, std::string tag, std::string defaultValue)
{
    std::string parsedName;
    if (!Parse(root, tag, parsedName))
    {
        LOG_INTERN(LogLevel::Info) << "Library " + tag << " undefined, falling back to default value " + defaultValue;
        return defaultValue;
    }
    return parsedName;
}

ExperimentConfig::Libraries SlaveConfigImporter::ImportLibraries(QDomElement rootElement)
{
    QDomElement libsRoot;
    if (!SimulationCommon::GetFirstChildElement(rootElement, "Libraries", libsRoot))
    {
        LOG_INTERN(LogLevel::Warning) << "No libraries found. Falling back to default values";
        return defaultLibraryMapping;
    }

    ExperimentConfig::Libraries libs;
    for (auto [tagName, defaultValue] : defaultLibraryMapping)
    {
        libs.try_emplace(tagName, SlaveConfigImporter::GetLibrary(libsRoot, tagName, defaultValue));
    }
    return libs;
}

bool SlaveConfigImporter::ImportLoggingGroups(QDomElement loggingGroupsElement,
        std::vector<std::string>& loggingGroups)
{
    const auto& loggingGroupElements = loggingGroupsElement.elementsByTagName("LoggingGroup");

    for (auto i = loggingGroupElements.length() - 1; i >= 0; --i)
    {
        const auto& loggingGroupElement = loggingGroupElements.at(i).toElement();

        if (loggingGroupElement.isNull())
        {
            LOG_INTERN(LogLevel::Error) << "Error parsing LoggingGroup elements";
            return false;
        }

        std::string groupName = loggingGroupElement.text().toStdString();

        if (groupName.length() == 0)
        {
            LOG_INTERN(LogLevel::Error) << "Invalid LoggingGroup name (empty string)";
            return false;
        }

        loggingGroups.push_back(groupName);
    }

    return true;
}

bool SlaveConfigImporter::ImportExperimentConfig(QDomElement experimentConfigElement,
        ExperimentConfig& experimentConfig)
{
    if (!ParseInt(experimentConfigElement, "ExperimentID", experimentConfig.experimentId))
    {
        LOG_INTERN(LogLevel::Error) << "ExperimentID not valid.";
        return false;
    }

    if (!ParseInt(experimentConfigElement, "NumberOfInvocations", experimentConfig.numberOfInvocations))
    {
        LOG_INTERN(LogLevel::Error) << "NumberOfInvocations not valid.";
        return false;
    }

    unsigned long randomSeed;
    if (!ParseULong(experimentConfigElement, "RandomSeed", randomSeed))
    {
        LOG_INTERN(LogLevel::Error) << "RandomSeed not valid.";
        return false;
    }
    experimentConfig.randomSeed = static_cast<std::uint32_t>(randomSeed);

    // Logging groups
    QDomElement loggingGroupsElement;
    if (!GetFirstChildElement(experimentConfigElement, "LoggingGroups", loggingGroupsElement) ||
            !ImportLoggingGroups(loggingGroupsElement, experimentConfig.loggingGroups))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import LoggingGroups.";
        return false;
    }

    experimentConfig.libraries = ImportLibraries(experimentConfigElement);

    return true;
}

bool SlaveConfigImporter::ImportScenarioConfig(QDomElement scenarioConfigElement,
        const std::string configurationDir,
        ScenarioConfig& scenarioConfig)
{
    std::string scenarioFilename;
    if (!ParseString(scenarioConfigElement, "OpenScenarioFile", scenarioFilename))
    {
        LOG_INTERN(LogLevel::Error) << "OpenScenarioFile not valid.";
        return false;
    }

    scenarioConfig.scenarioPath = Directories::Concat(configurationDir, scenarioFilename);

    return true;
}

bool SlaveConfigImporter::ImportEnvironmentConfig(QDomElement environmentConfigElement,
        EnvironmentConfig& environmentConfig)
{
    //Parse all time of days
    QDomElement timeOfDaysElement;
    if (!GetFirstChildElement(environmentConfigElement, "TimeOfDays", timeOfDaysElement)
            || !ImportProbabilityMap(timeOfDaysElement, "Value", "TimeOfDay", environmentConfig.timeOfDays))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import TimeOfDays.";
        return false;
    }

    //Parse all visibility distances
    QDomElement visibilityDistancesElement;
    if (!GetFirstChildElement(environmentConfigElement, "VisibilityDistances", visibilityDistancesElement)
            || !ImportProbabilityMap(visibilityDistancesElement, "Value", "VisibilityDistance",
                                     environmentConfig.visibilityDistances))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import VisibilityDistances.";
        return false;
    }


    //Parse all frictions
    QDomElement frictionsElement;
    if (!GetFirstChildElement(environmentConfigElement, "Frictions", frictionsElement)
            || !ImportProbabilityMap(frictionsElement, "Value", "Friction", environmentConfig.frictions))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import Frictions.";
        return false;
    }


    //Parse all weathers
    QDomElement weathersElement;
    if (!GetFirstChildElement(environmentConfigElement, "Weathers", weathersElement)
            || !ImportProbabilityMap(weathersElement, "Value", "Weather", environmentConfig.weathers))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import Weathers.";
        return false;
    }

    return true;
}

bool SlaveConfigImporter::ImportTrafficParameter(QDomElement trafficParameterElement,
        TrafficConfig& trafficConfig)
{
    QDomElement trafficVolumesElement;
    if (!GetFirstChildElement(trafficParameterElement, "TrafficVolumes", trafficVolumesElement)
            || !ImportProbabilityMap(trafficVolumesElement, "Value", "TrafficVolume", trafficConfig.trafficVolumes))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import TrafficVolumes.";
        return false;
    }

    QDomElement platoonRatesElement;
    if (!GetFirstChildElement(trafficParameterElement, "PlatoonRates", platoonRatesElement)
            || !ImportProbabilityMap(platoonRatesElement, "Value", "PlatoonRate", trafficConfig.platoonRates))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import PlatoonRates.";
        return false;
    }

    QDomElement velocitiesElement;
    if (!GetFirstChildElement(trafficParameterElement, "Velocities", velocitiesElement)
            || !ImportProbabilityMap(velocitiesElement, "Value", "Velocity", trafficConfig.velocities))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import Velocities.";
        return false;
    }

    QDomElement homogenitiesElement;
    if (!GetFirstChildElement(trafficParameterElement, "Homogenities", homogenitiesElement)
            || !ImportProbabilityMap(homogenitiesElement, "Value", "Homogenity", trafficConfig.homogenities))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import Homogenities.";
        return false;
    }

    return true;
}

bool SlaveConfigImporter::ImportLaneParameter(QDomElement trafficConfigElement, TrafficConfig& trafficConfig)
{
    QDomElement regularLaneElement;
    if (!GetFirstChildElement(trafficConfigElement, "RegularLane", regularLaneElement)
            || !ImportProbabilityMap(regularLaneElement, "Name", "AgentProfile", trafficConfig.regularLaneAgents))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import RegularLane.";
        return false;
    }

    QDomElement rightMostLaneElement;
    if (!GetFirstChildElement(trafficConfigElement, "RightMostLane", rightMostLaneElement)
            || !ImportProbabilityMap(rightMostLaneElement, "Name", "AgentProfile", trafficConfig.rightMostLaneAgents))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import RightMostLane.";
        return false;
    }

    return true;
}

bool SlaveConfigImporter::ImportTrafficConfig(QDomElement trafficConfigElement, TrafficConfig& trafficConfig)
{
    QDomElement trafficParameterElement;
    if (!GetFirstChildElement(trafficConfigElement, "TrafficParameter", trafficParameterElement)
            || !ImportTrafficParameter(trafficParameterElement, trafficConfig))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import TrafficParameter.";
        return false;
    }

    if (!ImportLaneParameter(trafficConfigElement, trafficConfig))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import LaneParameter.";
        return false;
    }

    return true;
}

bool SlaveConfigImporter::Import(const std::string& configurationDir,
                                 const std::string& slaveConfigFile,
                                 Configuration::SlaveConfig& slaveConfig)
{
    std::locale::global(std::locale("C"));

    QFile xmlFile(slaveConfigFile.c_str()); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Warning) << "an error occurred during slave configuration import";
        return false;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    if (!document.setContent(xmlData))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml file format of file " << slaveConfigFile;
        return false;
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        return false;
    }

    std::string configVersion;
    ParseAttributeString(documentRoot, "SchemaVersion", configVersion);
    if (configVersion.compare(supportedConfigVersion) != 0)
    {
        LOG_INTERN(LogLevel::Error) << "SlaveConfig version not supported. Supported version is " <<
                                    supportedConfigVersion;
        return false;
    }

    //Import profiles catalog
    std::string profilesCatalog;
    if (!ParseString(documentRoot, "ProfilesCatalog", profilesCatalog))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import ProfilesCatalog.";
        return false;
    }
    else
    {
        slaveConfig.SetProfilesCatalog(Directories::Concat(configurationDir, profilesCatalog));
    }


    //Import experiment config
    QDomElement experimentConfigElement;
    if (!GetFirstChildElement(documentRoot, "ExperimentConfig", experimentConfigElement)
            || !ImportExperimentConfig(experimentConfigElement, slaveConfig.GetExperimentConfig()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import ExperimentConfig.";
        return false;
    }

    //Import scenario config
    QDomElement scenarioConfigElement;
    if (!GetFirstChildElement(documentRoot, "ScenarioConfig", scenarioConfigElement)
            || !ImportScenarioConfig(scenarioConfigElement, configurationDir, slaveConfig.GetScenarioConfig()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import ScenarioConfig.";
        return false;
    }

    //Import environment config
    QDomElement environmentConfigElement;
    if (!GetFirstChildElement(documentRoot, "EnvironmentConfig", environmentConfigElement)
            || !ImportEnvironmentConfig(environmentConfigElement, slaveConfig.GetEnvironmentConfig()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import EnvironmentConfig.";
        return false;
    }

    //Import traffic config
    QDomElement trafficConfigElement;
    if (!GetFirstChildElement(documentRoot, "TrafficConfig", trafficConfigElement)
            || !ImportTrafficConfig(trafficConfigElement, slaveConfig.GetTrafficConfig()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import TrafficConfig.";
        return false;
    }

    return true;
}
