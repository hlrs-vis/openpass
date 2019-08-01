/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "vehicleModelsImporter.h"
#include "math.h"

namespace Importer {

#define CHECKFALSE(element) \
if(!element) \
{   throw std::runtime_error("Checkfalse failed."); \
} \

bool VehicleModelsImporter::Import(const std::string& vehicleCatalogPath,
                                   const std::string& pedestrianCatalogPath,
                                   Configuration::VehicleModels& vehicleModels)
{
    try
    {
        QDomElement catalogElement;

        if (!vehicleCatalogPath.empty())
        {
            ImportCatalog(vehicleCatalogPath, catalogElement);
            ImportVehicleCatalog(catalogElement, vehicleModels);
        }

        if (!pedestrianCatalogPath.empty())
        {
            ImportCatalog(pedestrianCatalogPath, catalogElement);
            ImportPedestrianCatalog(catalogElement, vehicleModels);
        }

        return true;
    }
    catch (const std::runtime_error& e)
    {
        LOG_INTERN(LogLevel::Error) << "VehicleModels import failed: " << e.what();
        return false;
    }
}

void VehicleModelsImporter::ImportVehicleCatalog(QDomElement& catalogElement,
        Configuration::VehicleModels& vehicleModels)
{
    QDomElement vehicleElement;
    const std::string vehicleTag { "Vehicle" };

    if (SimulationCommon::GetFirstChildElement(catalogElement, vehicleTag, vehicleElement))
    {
        //iterates over all models
        while (!vehicleElement.isNull())
        {
            ImportVehicleModel(vehicleElement, vehicleModels.GetVehicleModelMap());
            vehicleElement = vehicleElement.nextSiblingElement(vehicleTag.c_str());
        }
    }
}

void VehicleModelsImporter::ImportPedestrianCatalog(QDomElement& catalogElement,
        Configuration::VehicleModels& vehicleModels)
{
    QDomElement pedestrianElement;
    const std::string pedestrianTag { "Pedestrian" };

    if (SimulationCommon::GetFirstChildElement(catalogElement, pedestrianTag, pedestrianElement))
    {
        //iterates over all models
        while (!pedestrianElement.isNull())
        {
            ImportPedestrianModel(pedestrianElement, vehicleModels.GetVehicleModelMap());
            pedestrianElement = pedestrianElement.nextSiblingElement(pedestrianTag.c_str());
        }
    }
}

void VehicleModelsImporter::ImportCatalog(const std::string& catalogPath, QDomElement& catalogElement)
{
    LOG_INTERN(LogLevel::Info) << "Importing catalog from " << catalogPath;

    QFile xmlFile(catalogPath.c_str()); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Could not open " + catalogPath);
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;

    if (!document.setContent(xmlData))
    {
        throw std::runtime_error("Invalid xml format of " + catalogPath);
    }

    const QDomElement documentRoot = document.documentElement();

    if (documentRoot.isNull())
    {
        throw std::runtime_error("Root xml element not found in " + catalogPath);
    }

    CHECKFALSE(SimulationCommon::GetFirstChildElement(documentRoot, "Catalog", catalogElement));
}

void VehicleModelsImporter::CheckModelParameters(const VehicleModelParameters& model)
{
    assert(model.length > 0);
    assert(model.width > 0);
    assert(model.height > 0);
    assert(model.trackwidth > 0);
    assert(model.wheelbase > 0);
    assert(model.maxVelocity > 0);
    assert(model.maxCurvature > 0);
    assert(model.vehicleType != AgentVehicleType::Undefined);
}

void VehicleModelsImporter::ImportAgentVehicleType(const std::string& vehicleTypeString,
        AgentVehicleType& agentVehicleType)
{
    if (vehicleTypeString == "car" || vehicleTypeString == "van")
    {
        agentVehicleType = AgentVehicleType::Car;
    }
    else
        if (vehicleTypeString == "truck" || vehicleTypeString == "bus")
        {
            agentVehicleType = AgentVehicleType::Truck;
        }
        else
            if (vehicleTypeString == "motorbike")
            {
                agentVehicleType = AgentVehicleType::Motorbike;
            }
            else
                if (vehicleTypeString == "bicycle")
                {
                    agentVehicleType = AgentVehicleType::Bicycle;
                }
                else
                {
                    LOG_INTERN(LogLevel::Warning) << "Vehicle type '" + vehicleTypeString + "' not supported.";
                    agentVehicleType = AgentVehicleType::Undefined;
                }
}

void VehicleModelsImporter::ImportVehicleModelAxles(QDomElement& vehicleElement,
        VehicleModelParameters& modelParameters)
{
    QDomElement axlesElement;
    if (!SimulationCommon::GetFirstChildElement(vehicleElement, "Axles", axlesElement))
    {
        throw std::runtime_error("Vehicle model is missing axles tag");
    }

    QDomElement frontAxleElement;
    if (!SimulationCommon::GetFirstChildElement(axlesElement, "Front", frontAxleElement))
    {
        throw std::runtime_error("Vehicle model is missing front axle tag");
    }

    QDomElement rearAxleElement;
    if (!SimulationCommon::GetFirstChildElement(axlesElement, "Rear", rearAxleElement))
    {
        throw std::runtime_error("Vehicle model is missing rear axle tag");
    }

    VehicleAxle frontAxleParameters;
    VehicleAxle rearAxleParameters;

    ImportVehicleModelAxle(frontAxleElement, frontAxleParameters);
    ImportVehicleModelAxle(rearAxleElement, rearAxleParameters);
    ValidateAxles(frontAxleParameters, rearAxleParameters);
    AssignModelParameters(frontAxleParameters, rearAxleParameters, modelParameters);
}

void VehicleModelsImporter::AssignModelParameters(const VehicleAxle& frontAxle, const VehicleAxle& rearAxle,
        VehicleModelParameters& modelParameters)
{
    modelParameters.staticWheelRadius = rearAxle.wheelDiameter / 2.0;
    modelParameters.distanceReferencePointToFrontAxle = frontAxle.positionX;
    modelParameters.wheelbase = std::abs(frontAxle.positionX - rearAxle.positionX);
    modelParameters.trackwidth = rearAxle.trackWidth;
    modelParameters.maxCurvature = std::sin(frontAxle.maxSteering) / modelParameters.wheelbase;
    modelParameters.maximumSteeringWheelAngleAmplitude = frontAxle.maxSteering * modelParameters.steeringRatio * 180.0 /
            M_PI;
}

void VehicleModelsImporter::ImportVehicleModelAxle(QDomElement& axleElement, VehicleAxle& axleParameters)
{
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(axleElement, "wheelDiameter", axleParameters.wheelDiameter));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(axleElement, "positionX", axleParameters.positionX));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(axleElement, "trackWidth", axleParameters.trackWidth));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(axleElement, "maxSteering", axleParameters.maxSteering));
}

void VehicleModelsImporter::ValidateAxles(const VehicleAxle& frontAxle, const VehicleAxle& rearAxle)
{
    if (std::abs(frontAxle.wheelDiameter - rearAxle.wheelDiameter) > 1e-6)
    {
        LOG_INTERN(LogLevel::Warning) <<
                                      "Different wheel diameters for front and rear axle not supported. Using rear axle value.";
    }

    if (rearAxle.positionX != 0.0)
    {
        throw std::runtime_error("Reference point not on rear axle.");
    }

    if (rearAxle.positionX > frontAxle.positionX)
    {
        LOG_INTERN(LogLevel::Warning) << "Front axle is located behind rear axle.";
    }

    if (std::abs(frontAxle.trackWidth - rearAxle.trackWidth) > 1e-6)
    {
        LOG_INTERN(LogLevel::Warning) << "Different track width for front and rear axle not supported. Using rear axle value.";
    }

    if (rearAxle.maxSteering != 0.0)
    {
        throw std::runtime_error("Rear axle steering not supported.");
    }
}

void VehicleModelsImporter::ImportModelBoundingBox(QDomElement& modelElement, VehicleModelParameters& modelParameters)
{
    QDomElement boundingBoxElement;
    if (!SimulationCommon::GetFirstChildElement(modelElement, "BoundingBox", boundingBoxElement))
    {
        throw std::runtime_error("Model bounding box is missing");
    }

    QDomElement boundingBoxCenterElement;
    if (!SimulationCommon::GetFirstChildElement(boundingBoxElement, "Center", boundingBoxCenterElement))
    {
        throw std::runtime_error("Model bounding box center is missing");
    }

    QDomElement boundingBoxDimensionElement;
    if (!SimulationCommon::GetFirstChildElement(boundingBoxElement, "Dimension", boundingBoxDimensionElement))
    {
        throw std::runtime_error("Model bounding box dimension is missing");
    }

    double bbCenterX, bbCenterY, bbCenterZ;
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(boundingBoxCenterElement, "x", bbCenterX));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(boundingBoxCenterElement, "y", bbCenterY));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(boundingBoxCenterElement, "z", bbCenterZ));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(boundingBoxDimensionElement, "width", modelParameters.width));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(boundingBoxDimensionElement, "length", modelParameters.length));
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(boundingBoxDimensionElement, "height", modelParameters.height));

    if (bbCenterY != 0.0)
    {
        LOG_INTERN(LogLevel::Warning) << "Model bounding box center y-coordinate != 0.0";
    }

    if (std::abs(bbCenterZ - modelParameters.height / 2.0) > 1e-6)
    {
        LOG_INTERN(LogLevel::Warning) << "Model bounding box center z-coordinate is not half height";
    }

    modelParameters.distanceReferencePointToLeadingEdge = bbCenterX + modelParameters.length / 2.0;
}

void VehicleModelsImporter::ImportVehicleModelPerformance(QDomElement& vehicleElement,
        VehicleModelParameters& modelParameters)
{
    QDomElement performanceElement;
    if (!SimulationCommon::GetFirstChildElement(vehicleElement, "Performance", performanceElement))
    {
        throw std::runtime_error("Model performance tag is missing");
    }

    if (!SimulationCommon::ParseAttributeDouble(performanceElement, "maxSpeed", modelParameters.maxVelocity))
    {
        throw std::runtime_error("'maxSpeed' performance attribute missing");
    }

    if (!SimulationCommon::ParseAttributeDouble(performanceElement, "mass", modelParameters.weight))
    {
        throw std::runtime_error("'mass' performance attribute missing");
    }
}

void VehicleModelsImporter::ImportVehicleModelGears(QDomElement& parametersElement,
        VehicleModelParameters& modelParameters)
{
    double gearRatio;

    modelParameters.gearRatios.push_back(0.0);

    ImportModelParameter(parametersElement, "NumberOfGears", modelParameters.numberOfGears);

    for (int currentGear = 1; currentGear <= modelParameters.numberOfGears; ++currentGear)
    {
        ImportModelParameter(parametersElement, "GearRatio" + std::to_string(currentGear), gearRatio);
        modelParameters.gearRatios.push_back(gearRatio);
    }
}

void VehicleModelsImporter::ImportVehicleModel(QDomElement& vehicleElement, VehicleModelMap& vehicleModelsMap)
{
    VehicleModelParameters modelParameters;

    std::string vehicleModelCategory;

    if (!SimulationCommon::ParseAttributeString(vehicleElement, "category", vehicleModelCategory))
    {
        LOG_INTERN(LogLevel::Warning) << "Unable to parse vehicle model category";
        return;
    }

    ImportAgentVehicleType(vehicleModelCategory, modelParameters.vehicleType);

    std::string vehicleModelName;

    if (!SimulationCommon::ParseAttributeString(vehicleElement, "name", vehicleModelName))
    {
        throw std::runtime_error("Unable to parse vehicle model name");
    }

    if (vehicleModelsMap.find(vehicleModelName) != vehicleModelsMap.end())
    {
        throw std::runtime_error("Vehicle model '" + vehicleModelName + "' already exists");
    }

    QDomElement parametersElement;
    if (!SimulationCommon::GetFirstChildElement(vehicleElement, "ParameterDeclaration", parametersElement))
    {
        return;
    }

    if (modelParameters.vehicleType == AgentVehicleType::Car ||
            modelParameters.vehicleType == AgentVehicleType::Truck ||
            modelParameters.vehicleType == AgentVehicleType::Motorbike ||
            modelParameters.vehicleType == AgentVehicleType::Bicycle)
    {
        ImportModelParameter(parametersElement, "SteeringRatio", modelParameters.steeringRatio);

        ImportModelParameter(parametersElement, "MomentInertiaRoll", modelParameters.momentInertiaRoll);
        ImportModelParameter(parametersElement, "MomentInertiaPitch", modelParameters.momentInertiaPitch);
        ImportModelParameter(parametersElement, "MomentInertiaYaw", modelParameters.momentInertiaYaw);

        ImportModelParameter(parametersElement, "MinimumEngineSpeed", modelParameters.minimumEngineSpeed);
        ImportModelParameter(parametersElement, "MaximumEngineSpeed", modelParameters.maximumEngineSpeed);
        ImportModelParameter(parametersElement, "MaximumEngineTorque", modelParameters.maximumEngineTorque);
        ImportModelParameter(parametersElement, "MinimumEngineTorque", modelParameters.minimumEngineTorque);

        ImportModelParameter(parametersElement, "AirDragCoefficient", modelParameters.airDragCoefficient);
        ImportModelParameter(parametersElement, "AxleRatio", modelParameters.axleRatio);
        ImportModelParameter(parametersElement, "DecelerationFromPowertrainDrag",
                             modelParameters.decelerationFromPowertrainDrag);
        ImportModelParameter(parametersElement, "FrictionCoefficient", modelParameters.frictionCoeff);
        ImportModelParameter(parametersElement, "FrontSurface", modelParameters.frontSurface);

        ImportModelBoundingBox(vehicleElement, modelParameters);
        ImportVehicleModelAxles(vehicleElement, modelParameters);
        ImportVehicleModelPerformance(vehicleElement, modelParameters);
        ImportVehicleModelGears(parametersElement, modelParameters);

        modelParameters.heightCOG = 0.0;    // currently not supported

        CheckModelParameters(modelParameters);
    }
    else
    {
        throw std::runtime_error("VehicleModelCategory '" + vehicleModelCategory + "' currently not supported");
    }

    vehicleModelsMap[vehicleModelName] = modelParameters;
}

void VehicleModelsImporter::ImportPedestrianModel(QDomElement& pedestrianElement, VehicleModelMap& vehicleModelsMap)
{
    VehicleModelParameters modelParameters;

    std::string pedestrianCategory;

    if (!SimulationCommon::ParseAttributeString(pedestrianElement, "category", pedestrianCategory))
    {
        LOG_INTERN(LogLevel::Warning) << "Unable to parse pedestrian category";
        return;
    }

    if (pedestrianCategory != "pedestrian")
    {
        throw std::runtime_error("Unsupported pedestrian category");
    }

    std::string pedestrianModelName;

    if (!SimulationCommon::ParseAttributeString(pedestrianElement, "name", pedestrianModelName))
    {
        throw std::runtime_error("Unable to parse pedestrian model name");
    }

    if (vehicleModelsMap.find(pedestrianModelName) != vehicleModelsMap.end())
    {
        throw std::runtime_error("pedestrian model '" + pedestrianModelName + "' already exists");
    }

    if (!SimulationCommon::ParseAttributeDouble(pedestrianElement, "mass", modelParameters.weight))
    {
        throw std::runtime_error("Unable to parse pedestrian mass");
    }

    QDomElement parametersElement;
    if (!SimulationCommon::GetFirstChildElement(pedestrianElement, "ParameterDeclaration", parametersElement))
    {
        return;
    }

    ImportModelBoundingBox(pedestrianElement, modelParameters);

    // Currently, AgentAdapter and Dynamics cannot handle pedestrians properly
    // setting some required defaults here for now for compatibility with cars
    modelParameters.vehicleType = AgentVehicleType::Car;
    modelParameters.gearRatios.push_back(0.0);
    modelParameters.gearRatios.push_back(1.0);
    modelParameters.numberOfGears = 1;

    vehicleModelsMap[pedestrianModelName] = modelParameters;
}

} //namespace Importer
