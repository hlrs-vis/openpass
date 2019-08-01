/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "profilesImporter.h"
#include "CoreFramework/CoreShare/log.h"

using namespace Importer;
using namespace SimulationCommon;

bool ProfilesImporter::ImportAgentProfiles(QDomElement agentProfilesElement,
        std::unordered_map<std::string, AgentProfile>& agentProfiles)
{
    QDomElement agentProfileElement;
    if (!GetFirstChildElement(agentProfilesElement, "AgentProfile", agentProfileElement))
    {
        LOG_INTERN(LogLevel::Error) << "At least one agent profile is required.";
        return false;
    }

    while (!agentProfileElement.isNull())
    {
        std::string agentProfileName;
        if (!ParseAttributeString(agentProfileElement, "Name", agentProfileName))
        {
            LOG_INTERN(LogLevel::Error) << "AgentProfile name is invalid.";
            return false;
        }

        std::string profileType;
        if (!ParseAttributeString(agentProfileElement, "Type", profileType))
        {
            LOG_INTERN(LogLevel::Error) << "AgentProfile type is invalid.";
            return false;
        }

        AgentProfile agentProfile;

        if (profileType == "Dynamic")
        {
            agentProfile.type = AgentProfileType::Dynamic;

            //Parses all driver profiles
            QDomElement driverProfilesElement;
            if (!GetFirstChildElement(agentProfileElement, "DriverProfiles", driverProfilesElement)
                    || !ImportProbabilityMap(driverProfilesElement, "Name", "DriverProfile", agentProfile.driverProfiles))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import DriverProfiles.";
                return false;
            }

            //Parses all vehicle profiles
            QDomElement vehicleProfilesElement;
            if (!GetFirstChildElement(agentProfileElement, "VehicleProfiles", vehicleProfilesElement)
                    || !ImportProbabilityMap(vehicleProfilesElement, "Name", "VehicleProfile", agentProfile.vehicleProfiles))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import VehicleProfiles.";
                return false;
            }
        }

        else
            if (profileType == "Static")
            {
                agentProfile.type = AgentProfileType::Static;
                QDomElement systemElement;
                if (!GetFirstChildElement(agentProfileElement, "System", systemElement))
                {
                    LOG_INTERN(LogLevel::Error) << "Could not import System.";
                    return false;
                }

                std::string systemConfigFile;
                if (!ParseString(systemElement, "File", systemConfigFile))
                {
                    LOG_INTERN(LogLevel::Error) << "Could not import SystemConfig file.";
                    return false;
                }
                agentProfile.systemConfigFile = systemConfigFile;

                int systemId;
                if (!ParseInt(systemElement, "Id", systemId))
                {
                    LOG_INTERN(LogLevel::Error) << "Could not import System id.";
                    return false;
                }
                agentProfile.systemId = systemId;

                std::string vehicleModel;
                if (!ParseString(agentProfileElement, "VehicleModel", vehicleModel))
                {
                    LOG_INTERN(LogLevel::Error) << "Could not import VehicleModel.";
                    return false;
                }
                agentProfile.vehicleModel = vehicleModel;
            }

            else
            {
                LOG_INTERN(LogLevel::Error) << "Invalid agent profile type.";
                return false;
            }

        auto insertReturn = agentProfiles.insert({agentProfileName, agentProfile});
        if (!insertReturn.second)
        {
            LOG_INTERN(LogLevel::Error) << "AgentProfile names need to be unique.";
            return false;
        }

        agentProfileElement = agentProfileElement.nextSiblingElement("AgentProfile");
    }

    return true;
}

bool ProfilesImporter::ImportDriverProfiles(QDomElement driverProfilesElement, DriverProfiles& driverProfiles)
{
    QDomElement driverProfileElement;
    if (!GetFirstChildElement(driverProfilesElement, "DriverProfile", driverProfileElement))
    {
        LOG_INTERN(LogLevel::Error) << "At least one driver profile is required.";
        return false;
    }

    while (!driverProfileElement.isNull())
    {
        std::string profileName;
        auto parameters = std::make_shared<ModelParameters>();

        if (!ParseAttributeString(driverProfileElement, "Name", profileName)
                || !ParameterImporter::ImportParameters(driverProfileElement, *parameters))
        {
            LOG_INTERN(LogLevel::Error) << "Could not import driver profile.";
            return false;
        }

        if (parameters->GetParametersString().find("Type") == parameters->GetParametersString().end())
        {
            LOG_INTERN(LogLevel::Error) << "Driver profile needs a type.";
            return false;
        }

        auto insertReturn = driverProfiles.emplace(profileName, parameters);
        if (!insertReturn.second)
        {
            LOG_INTERN(LogLevel::Error) << "Driver profile names need to be unique.";
            return false;
        }

        driverProfileElement = driverProfileElement.nextSiblingElement("DriverProfile");
    }

    return true;
}


bool ProfilesImporter::ImportAllVehicleComponentProfiles(QDomElement vehicleComponentProfilesElement,
        std::unordered_map<std::string, VehicleComponentProfiles>& vehicleComponentProfilesMap)
{
    QDomElement vehicleComponentProfileElement;

    if (GetFirstChildElement(vehicleComponentProfilesElement, "VehicleComponentProfile", vehicleComponentProfileElement))
    {
        while (!vehicleComponentProfileElement.isNull())
        {
            std::string componentType;
            std::string profileName;
            auto parameters = std::make_shared<SimulationCommon::ModelParameters>();

            if (!ParseAttributeString(vehicleComponentProfileElement, "Type", componentType))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import component type.";
                return false;
            }

            if (!ParseAttributeString(vehicleComponentProfileElement, "Name", profileName))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import profile name.";
                return false;
            }

            if (!ParameterImporter::ImportParameters(vehicleComponentProfileElement, *parameters))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import component parameters.";
                return false;
            }

            if (vehicleComponentProfilesMap.count(componentType) == 0)
            {
                VehicleComponentProfiles vehicleComponentProfiles;
                vehicleComponentProfilesMap.insert({componentType, vehicleComponentProfiles});
            }

            auto insertReturn = vehicleComponentProfilesMap.at(componentType).emplace(profileName, parameters);
            if (!insertReturn.second)
            {
                LOG_INTERN(LogLevel::Error) << "Component profile names need to be unique.";
                return false;
            }
            vehicleComponentProfileElement = vehicleComponentProfileElement.nextSiblingElement(
                                                 QString::fromStdString("VehicleComponentProfile"));
        }
    }

    return true;
}

bool ProfilesImporter::ImportSensorProfiles(QDomElement sensorProfilesElement, std::list<SensorProfile>& sensorProfiles)
{
    QDomElement sensorProfileElement;
    GetFirstChildElement(sensorProfilesElement, "SensorProfile", sensorProfileElement);

    while (!sensorProfileElement.isNull())
    {
        std::string profileName;
        std::string sensorType;
        auto parameters = std::make_shared<SimulationCommon::Parameters>();

        if (!ParseAttributeString(sensorProfileElement, "Name", profileName))
        {
            LOG_INTERN(LogLevel::Error) << "Sensor profile needs a name.";
            return false;
        }

        if (!ParseAttributeString(sensorProfileElement, "Type", sensorType))
        {
            LOG_INTERN(LogLevel::Error) << "Sensor profile needs a type.";
            return false;
        }

        if (!ParameterImporter::ImportParameters(sensorProfileElement, *parameters))
        {
            LOG_INTERN(LogLevel::Error) << "Could not import sensor parameters.";
            return false;
        }

        sensorProfiles.emplace_back(SensorProfile{profileName, sensorType, parameters});
        sensorProfileElement = sensorProfileElement.nextSiblingElement("SensorProfile");
    }

    return true;
}

bool ProfilesImporter::ImportSensorLinksOfComponent(QDomElement sensorLinksElement, std::list<SensorLink>& sensorLinks)
{
    QDomElement sensorLinkElement;
    GetFirstChildElement(sensorLinksElement, "SensorLink", sensorLinkElement);

    while (!sensorLinkElement.isNull())
    {
        int sensorId;
        std::string inputId;

        if (!ParseAttributeInt(sensorLinkElement, "SensorId", sensorId))
        {
            LOG_INTERN(LogLevel::Error) << "Sensor link needs a SensorId.";
            return false;
        }

        if (!ParseAttributeString(sensorLinkElement, "InputId", inputId))
        {
            LOG_INTERN(LogLevel::Error) << "Sensor link needs a InputId.";
            return false;
        }

        SensorLink sensorLink{};
        sensorLink.sensorId = sensorId;
        sensorLink.inputId = inputId;
        sensorLinks.push_back(sensorLink);

        sensorLinkElement = sensorLinkElement.nextSiblingElement("SensorLink");
    }

    return true;
}

bool ProfilesImporter::ImportVehicleComponent(QDomElement vehicleComponentElement, VehicleComponent& vehicleComponent)
{
    ParseAttributeString(vehicleComponentElement, "Type", vehicleComponent.type);

    QDomElement profilesElement;
    GetFirstChildElement(vehicleComponentElement, "Profiles", profilesElement);
    if (!ImportProbabilityMap(profilesElement, "Name", "Profile", vehicleComponent.componentProfiles, false))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import ComponentProfile.";
        return false;
    }

    QDomElement sensorLinksElement;
    GetFirstChildElement(vehicleComponentElement, "SensorLinks", sensorLinksElement);
    if (!ImportSensorLinksOfComponent(sensorLinksElement, vehicleComponent.sensorLinks))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import SensorLinks.";
        return false;
    }

    return true;
}

bool ProfilesImporter::ImportAllVehicleComponentsOfVehicleProfile(QDomElement vehicleProfileElement,
        VehicleProfile& vehicleProfile)
{
    QDomElement vehicleComponentsElement;
    if (!GetFirstChildElement(vehicleProfileElement, "Components", vehicleComponentsElement))
    {
        LOG_INTERN(LogLevel::Error) << "Missing Components tag.";
        return false;
    }


    QDomElement componentElement;
    GetFirstChildElement(vehicleComponentsElement, "Component", componentElement);
    while (!componentElement.isNull())
    {
        VehicleComponent vehicleComponent;
        if (!ImportVehicleComponent(componentElement, vehicleComponent))
        {
            LOG_INTERN(LogLevel::Error) << "Failed to import Component";
            return false;
        }

        vehicleProfile.vehicleComponents.push_back(vehicleComponent);
        componentElement = componentElement.nextSiblingElement("Component");
    }

    return true;
}

bool ProfilesImporter::ImportSensorParameters(QDomElement sensorElement, SensorParameter& sensor)
{
    if (!ParseAttributeInt(sensorElement, "Id", sensor.id))
    {
        LOG_INTERN(LogLevel::Error) << "Sensor needs an Id.";
        return false;
    }

    QDomElement positionElement;
    if (!GetFirstChildElement(sensorElement, "Position", positionElement))
    {
        LOG_INTERN(LogLevel::Error) << "Sensor needs a Position.";
        return false;
    }

    if (!ParseAttributeString(positionElement, "Name", sensor.sensorPosition.name))
    {
        LOG_INTERN(LogLevel::Error) << "Sensorposition needs a Name.";
        return false;
    }

    if (!ParseAttributeDouble(positionElement, "Longitudinal", sensor.sensorPosition.longitudinal))
    {
        LOG_INTERN(LogLevel::Error) << "Sensorposition needs a Longitudinal.";
        return false;
    }

    if (!ParseAttributeDouble(positionElement, "Lateral", sensor.sensorPosition.lateral))
    {
        LOG_INTERN(LogLevel::Error) << "Sensorposition needs a Lateral.";
        return false;
    }

    if (!ParseAttributeDouble(positionElement, "Height", sensor.sensorPosition.height))
    {
        LOG_INTERN(LogLevel::Error) << "Sensorposition needs a Height.";
        return false;
    }

    if (!ParseAttributeDouble(positionElement, "Pitch", sensor.sensorPosition.pitch))
    {
        LOG_INTERN(LogLevel::Error) << "Sensorposition needs a Pitch.";
        return false;
    }

    if (!ParseAttributeDouble(positionElement, "Yaw", sensor.sensorPosition.yaw))
    {
        LOG_INTERN(LogLevel::Error) << "Sensorposition needs a Yaw.";
        return false;
    }

    if (!ParseAttributeDouble(positionElement, "Roll", sensor.sensorPosition.roll))
    {
        LOG_INTERN(LogLevel::Error) << "Sensorposition needs a Roll.";
        return false;
    }

    QDomElement profileElement;
    if (!GetFirstChildElement(sensorElement, "Profile", profileElement))
    {
        LOG_INTERN(LogLevel::Error) << "Sensor needs a Profile.";
        return false;
    }

    if (!ParseAttributeString(profileElement, "Type", sensor.sensorProfile.type))
    {
        LOG_INTERN(LogLevel::Error) << "SensorProfile needs a Type.";
        return false;
    }

    if (!ParseAttributeString(profileElement, "Name", sensor.sensorProfile.name))
    {
        LOG_INTERN(LogLevel::Error) << "SensorProfile needs a Name.";
        return false;
    }

    return true;
}

bool ProfilesImporter::ImportAllSensorsOfVehicleProfile(QDomElement vehicleProfileElement,
        VehicleProfile& vehicleProfile)
{
    QDomElement sensorsElement;
    if (!GetFirstChildElement(vehicleProfileElement, "Sensors", sensorsElement))
    {
        LOG_INTERN(LogLevel::Error) << "VehicleProfile has no Sensors Element";
        return false;
    }

    QDomElement sensorElement;
    GetFirstChildElement(sensorsElement, "Sensor", sensorElement);
    while (!sensorElement.isNull())
    {
        SensorParameter sensor;
        if (!ImportSensorParameters(sensorElement, sensor))
        {
            LOG_INTERN(LogLevel::Error) << "Failed to import Sensor";
            return false;
        }

        vehicleProfile.sensors.push_back(sensor);
        sensorElement = sensorElement.nextSiblingElement("Sensor");
    }

    return true;
}

bool ProfilesImporter::ImportVehicleProfile(QDomElement vehicleProfileElement, VehicleProfile& vehicleProfile)
{
    QDomElement vehicleModelElement;
    if (!GetFirstChildElement(vehicleProfileElement, "Model", vehicleModelElement))
    {
        LOG_INTERN(LogLevel::Error) << "VehicleProfile has no VehicleModel Element";
        return false;
    }
    if (!ParseAttributeString(vehicleModelElement, "Name", vehicleProfile.vehicleModel))
    {
        LOG_INTERN(LogLevel::Error) << "Model needs a Name";
        return false;
    }

    if (!ImportAllVehicleComponentsOfVehicleProfile(vehicleProfileElement, vehicleProfile))
    {
        LOG_INTERN(LogLevel::Error) << "Failed to import Vehicle Components";
        return false;
    }

    if (!ImportAllSensorsOfVehicleProfile(vehicleProfileElement, vehicleProfile))
    {
        LOG_INTERN(LogLevel::Error) << "Failed to import Sensors";
        return false;
    }

    return true;
}

bool ProfilesImporter::ImportVehicleProfiles(QDomElement vehicleProfilesElement,
        std::unordered_map<std::string, VehicleProfile>& vehicleProfiles)
{
    QDomElement vehicleProfileElement;
    GetFirstChildElement(vehicleProfilesElement, "VehicleProfile", vehicleProfileElement);

    while (!vehicleProfileElement.isNull())
    {
        std::string profileName;
        VehicleProfile vehicleProfile;

        if (!ParseAttributeString(vehicleProfileElement, "Name", profileName))
        {
            LOG_INTERN(LogLevel::Error) << "Vehicle profile needs a name.";
            return false;
        }

        if (!ImportVehicleProfile(vehicleProfileElement, vehicleProfile))
        {
            LOG_INTERN(LogLevel::Error) << "Failed to import VehicleProfile";
            return false;
        }

        vehicleProfiles.insert(std::make_pair<std::string&, VehicleProfile&>(profileName, vehicleProfile));

        vehicleProfileElement = vehicleProfileElement.nextSiblingElement("VehicleProfile");
    }

    return true;
}

bool ProfilesImporter::Import(const std::string& filename, Profiles& profiles)
{
    std::locale::global(std::locale("C"));

    QFile xmlFile(filename.c_str()); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Warning) << "an error occurred during profilesCatalog import";
        return false;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    if (!document.setContent(xmlData))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml file format of file " << filename;
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
        LOG_INTERN(LogLevel::Error) << "ProfilesCatalog version not supported. Supported version is " <<
                                    supportedConfigVersion;
        return false;
    }

    //Import agent profiles
    QDomElement agentProfilesElement;
    if (!GetFirstChildElement(documentRoot, "AgentProfiles", agentProfilesElement)
            || !ImportAgentProfiles(agentProfilesElement, profiles.GetAgentProfiles()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import AgentProfiles.";
        return false;
    }

    //Import driver profiles
    QDomElement driverProfilesElement;
    if (!GetFirstChildElement(documentRoot, "DriverProfiles", driverProfilesElement)
            || !ImportDriverProfiles(driverProfilesElement, profiles.GetDriverProfiles()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import DriverProfiles.";
        return false;
    }

    //Import all VehicleComponent profiles
    QDomElement vehicleComponentProfilesElement;
    if (!GetFirstChildElement(documentRoot, "VehicleComponentProfiles", vehicleComponentProfilesElement)
            || !ImportAllVehicleComponentProfiles(vehicleComponentProfilesElement, profiles.GetVehicleComponentProfiles()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import VehicleComponentProfiles.";
        return false;
    }

    //Import vehicle profiles
    QDomElement vehicleProfilesElement;
    if (!GetFirstChildElement(documentRoot, "VehicleProfiles", vehicleProfilesElement)
            || !ImportVehicleProfiles(vehicleProfilesElement, profiles.GetVehicleProfiles()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import VehicleProfiles.";
        return false;
    }

    //Import sensor profiles
    QDomElement sensorProfilesElement;
    if (!GetFirstChildElement(documentRoot, "SensorProfiles", sensorProfilesElement)
            || !ImportSensorProfiles(sensorProfilesElement, profiles.GetSensorProfiles()))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import SensorProfiles.";
        return false;
    }

    return true;
}
