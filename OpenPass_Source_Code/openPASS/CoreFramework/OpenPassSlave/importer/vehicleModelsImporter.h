/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <QDomDocument>
#include <QFile>
#include <assert.h>
#include <unordered_map>
#include "xmlParser.h"
#include "Common/globalDefinitions.h"
#include "vehicleModels.h"

namespace Importer {
/*!
 * \brief Container for axle parameters in OpenSCENARIO vehicle model
 */
struct VehicleAxle
{
    double maxSteering;     //!< Maximum steering angle
    double wheelDiameter;   //!< Diameter of the wheels
    double trackWidth;      //!< Trackwidth of the axle
    double positionX;       //!< Longitudinal position offset (measured from reference point)
    double positionZ;       //!< Vertical position offset (measured from reference point)
};

/*!
 * \brief Provides methods for importing vehicle models from OpenSCENARIO catalog files
 */
class VehicleModelsImporter
{
public:
    /*!
     * \brief Imports vehicle and pedestrian models from the given paths
     *
     * The `vehicleModelsPath` and the `pedestrianModelsPath` are appended to the configurationDir string, respectively.
     * If one of the paths is an empty string, the import of this type of models is ommited and doesn't result
     * in an error.
     *
     * \param[in]   vehicleModelsPath       File path to the vehicle model catalog
     * \param[in]   pedestrianModelsPath    File path to the pedestrian model catalog
     * \param[out]  vehicleModels           Storage for the imported models
     *
     * \return      true on success, false otherwise
     */
    static bool Import(const std::string& vehicleModelsPath,
                       const std::string& pedestrianModelsPath,
                       Configuration::VehicleModels& vehicleModels);

    /*!
     * \brief Imports a vehicle model from OpenSCENARIO DOM
     *
     * The model is stored in `vehicleModelsMap` with the model attribute `name` as its key.
     * Duplicated model names are not allowed.
     *
     * Supported vehicle model categories are `car`, `van`, `truck`, `bus`, `motorbike` and `bicycle`.
     *
     * \param[in]   modelElement        Vehicle model DOM element
     * \param[out]  vehicleModelsMap    Storage for the imported vehicle model
     *
     * \throw   std::runtime_error  On invalid XML, duplicated model names or unsupported vehicle category
     */
    static void ImportVehicleModel(QDomElement& modelElement, VehicleModelMap& vehicleModelsMap);

    /*!
     * \brief Imports a pedestrian model from OpenSCENARIO DOM
     *
     * The model is stored in `vehicleModelsMap` with the model attribute `name` as its key.
     * Duplicated model names are not allowed.
     *
     * The only supported pedestrian model category is `pedestrian`.
     *
     * \param[in]   modelElement        Pedestrian model DOM element
     * \param[out]  vehicleModelsMap    Storage for the imported pedestrian model (NOTE: internaly shared with vehicles)
     *
     * \throw   std::runtime_error  On invalid XML, duplicated model names or unsupported pedestrian category
     */
    static void ImportPedestrianModel(QDomElement& pedestrianElement, VehicleModelMap& vehicleModelsMap);

private:
    /*!
     * \brief Asserts that the provided model parameters are valid
     *
     * Model parameters are considered valid, if all the following parameters have a value greater than 0.0
     * and the vehicle type is not `AgentVehicleType::Undefined`:
     *     - length
     *     - width
     *     - height
     *     - trackwidth
     *     - wheelbase
     *     - maxVelocity
     *     - maxCurvature
     *
     * \param[in]   model   The model parameters to check
     */
    static void CheckModelParameters(const VehicleModelParameters& model);

    /*!
     * \brief Imports a catalog from OpenSCENARIO DOM
     *
     *
     * \param[in]   catalogPath         Full path to the catalog XML file
     * \param[out]  catalogElement      The imported catalog element
     *
     * \throw   std::runtime_error  On invalid XML or missing catalog tag
     */
    static void ImportCatalog(const std::string& catalogPath, QDomElement& catalogElement);

    /*!
     * \brief Imports all vehicles from a catalog (OpenSCENARIO DOM)
     *
     * \param[in]   catalogElement      The catalog element to import vehicles from
     * \param[out]  vehicleModels       Storage for the imported vehicle models
     */
    static void ImportVehicleCatalog(QDomElement& catalogElement, Configuration::VehicleModels& vehicleModels);

    /*!
     * \brief Imports all pedestrians from a catalog (OpenSCENARIO DOM)
     *
     * \param[in]   catalogElement      The catalog element to import predestrians from
     * \param[out]  vehicleModels       Storage for the imported pedestrian models (NOTE: internaly shared with vehicles)
     */
    static void ImportPedestrianCatalog(QDomElement& catalogElement, Configuration::VehicleModels& vehicleModels);

    /*!
     * \brief Imports the vehicle type for an agent
     *
     * \param[in]   vehicleTypeString   Type string of the vehicle category (from OpenSCENARIO catalog file)
     * \param[out]  agentVehicleType    Mapped type of the vehicle
     *
     * Supported vehicle model categories are `car`, `van`, `truck`, `bus`, `motorbike` and `bicycle`.
     * `van` and `bus` are mapped to `car` and `truck`, respectively.
     * Unsupported model categories are mapped to AgentVehicleType::Undefined and a warning is issued.
     */
    static void ImportAgentVehicleType(const std::string& vehicleTypeString, AgentVehicleType& agentVehicleType);

    /*!
     * \brief Imports the model's bounding box from OpenSCENARIO DOM
     *
     * \param[in]   modelElement        The DOM element of the model
     * \param[out]  modelParameters     Storage for the imported values
     *
     * A warning is issued, if the bounding box center is not 0.0 in y-direction and half the model's height in z-direction,
     * as offsets in these directions are not supported by the simulator.
     *
     * \throw   std::runtime_error  On missing `BoundingBox`, `Center` or `Dimension` tag
     */
    static void ImportModelBoundingBox(QDomElement& modelElement, VehicleModelParameters& modelParameters);

    /*!
     * \brief Imports the models' axles parameters
     *
     * The following parameters are derived from axle configuration:
     *     - maxCurvature
     *     - maximumSteeringWheelAngleAmplitude
     *     - trackWidth
     *     - wheelBase
     *     - distanceReferencePointToFrontAxle
     *     - staticWheelRadius
     *
     * Warnings are issued for unsupported configurations (different track width for axles, different radius for front and
     * rear wheels or front axle located behind rear axle).
     *
     * \param[in]   vehicleElement      The DOM element of the vehicle model
     * \param[out]  modelParameters     Storage for the imported values
     *
     * \throw   std::runtime_error  On missing `Axles`, `Front` or `Rear` tag, reference point not being on rear axle or
     *                              steering defined for rear axle
     */
    static void ImportVehicleModelAxles(QDomElement& vehicleElement, VehicleModelParameters& modelParameters);


    /*!
     * \brief Imports the parameters of a single axle
     *
     * \param[in]   axleElement         The DOM element ot the axle
     * \param[out]  axleParameters      Storage for the imported values
     *
     * \throw   std::runtime_error  On missing `Axles`, `Front` or `Rear` tag, reference point not being on rear axle or
     *                              steering defined for rear axle
     */
    static void ImportVehicleModelAxle(QDomElement& axleElement, VehicleAxle& axleParameters);

    /*!
     * \brief Assigns the axle-specific model parameters
     *
     * \param[in]   frontAxle           Parameters of the front axle
     * \param[in]   rearAxle            Parameters of the rear axle
     * \param[out]  modelParameters     Storage for the model parameters
     */
    static void AssignModelParameters(const VehicleAxle& frontAxle, const VehicleAxle& rearAxle,
                                      VehicleModelParameters& modelParameters);

    /*!
     * \brief Validates the axle parameters
     *
     * \param[in]   frontAxle   The parameters of the front axle
     * \param[in]   rearAxle    The parameters of the rear axle
     *
     * \throw std::runtime_error    When reference point not on rear axle or rear axle steering != 0.0
     */
    static void ValidateAxles(const VehicleAxle& frontAxle, const VehicleAxle& rearAxle);

    /*!
     * \brief Imports the models' performance tag from OpenSCENARIO DOM
     *
     * The values for `maxSpeed` and `mass` are imported to their corresponding model parameters.
     *
     * \note  Currently, `maxDeceleration` is ignored.
     *
     * \param[in]   vehicleElement      The DOM element of the vehicle model
     * \param[out]  modelParameters     Storage for the imported values
     *
     * \throw   std::runtime_error  On missing `Performance` tag or missing or invalid `maxSpeed` or `mass` attribute
     */
    static void ImportVehicleModelPerformance(QDomElement& vehicleElement, VehicleModelParameters& modelParameters);

    /*!
     * \brief Imports the models' gears from its `ParameterDeclaration` in OpenSCENARIO DOM
     *
     * The paramters `NumberOfGears` has to specifiy the number of gears in a model. The first entry in VehicleModelParameters
     * `gearRatios` will always be 0.0. For every gear, a `Parameter` tag `GearRatioN` has to exist, where N represents the
     * number of a gear. Ratios are imported in ascending order of the gear number.
     *
     * \param[in]   vehicleElement      The DOM element of the vehicle model
     * \param[out]  modelParameters     Storage for the imported values
     */
    static void ImportVehicleModelGears(QDomElement& parametersElement, VehicleModelParameters& modelParameters);

    /*!
     * \brief Imports the value from an model parameter tag
     *
     * \param[in]   parametersElement   Parameters DOM element
     * \param[in]   parameterName       Name of the parameter to import
     * \param[out]  parameterValue      Value of the parsed parameter
     *
     * \throw   std::runtime_error  On missing model `Parameter` tag or invalid `name` or `value` attribute.
     */
    template<typename T>
    static void ImportModelParameter(QDomElement& parametersElement, const std::string& parameterName, T& parameterValue)
    {
        QDomElement parameterElement;
        if (!SimulationCommon::GetFirstChildElement(parametersElement, "Parameter", parameterElement))
        {
            throw std::runtime_error("Model parameter '" + parameterName + "' is missing");
        }

        do
        {
            if (parameterElement.attribute("name").toStdString() == parameterName)
            {
                std::string parameterType;

                if (!SimulationCommon::ParseAttribute(parameterElement, "type", parameterType))
                {
                    throw std::runtime_error("Error parsing type of '" + parameterName + "'");
                }

                if (!SimulationCommon::ParseAttribute(parameterElement, "value", parameterValue))
                {
                    throw std::runtime_error("Error parsing '" + parameterName + "'");
                }

                return;
            }

            parameterElement = parameterElement.nextSiblingElement("Parameter");

        } while (!parameterElement.isNull());

        throw std::runtime_error("Model parameter '" + parameterName + "' is missing");
    }
};

} //namespace Importer
