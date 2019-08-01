/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "profiles.h"
#include "parameters.h"
#include "parameterImporter.h"
#include "xmlParser.h"

//using namespace Configuration;

namespace Importer {

class ProfilesImporter
{
public:
    /*!
    * \brief Imports the AgentProfiles section of the profiles catalog
    *
    * @param[in]     agentProfilesElement    Element containing the information
    * @param[out]    agentProfiles           Map into which the values get saved
    * @return	     true, if successful
    */
    static bool ImportAgentProfiles(QDomElement agentProfilesElement,
                                    std::unordered_map<std::string, AgentProfile> &agentProfiles);

    /*!
    * \brief Imports the DriverProfiles section of the profiles catalog
    *
    * @param[in]     driverProfilesElement    Element containing the information
    * @param[out]    driverProfiles           Map into which the values get saved
    * @return	     true, if successful
    */
    static bool ImportDriverProfiles(QDomElement driverProfilesElement,
                                     DriverProfiles &driverProfiles);


    /*!
    * \brief Imports the VehicleComponentProfiles section of the profiles catalog
    *
    * @param[in]     vehicleComponentProfilesElement    Element containing the information
    * @param[out]    vehicleComponentProfilesMap        Map into which the values get saved
    * @return	     true, if successful
    */
    static bool ImportAllVehicleComponentProfiles(QDomElement vehicleComponentProfilesElement,
                                      std::unordered_map<std::string, VehicleComponentProfiles> &vehicleComponentProfilesMap);

    /*!
    * \brief Imports the SensorProfiles section of the profiles catalog
    *
    * @param[in]     sensorProfilesElement    Element containing the information
    * @param[out]    sensorProfiles           Map into which the values get saved
    * @return	     true, if successful
    */
    static bool ImportSensorProfiles(QDomElement sensorProfilesElement,
                                     std::list<SensorProfile> &sensorProfiles);

    /*!
    * \brief Imports the AgentProfiles section of the profiles catalog
    *
    * @param[in]     agentProfilesElement    Element containing the information
    * @param[out]    agentProfiles           Map into which the values get saved
    * @return	     true, if successful
    */
    static bool ImportVehicleProfiles(QDomElement vehicleProfilesElement,
                                         std::unordered_map<std::string, VehicleProfile> &vehicleProfiles);

    /*!
     * \brief Imports a single VehicleProfile
     * \param vehicleProfileElement  Element containing the information
     * \param vehicleProfile         VehicleProfile to fill
     * \return
     */
    static bool ImportVehicleProfile(QDomElement vehicleProfileElement,
                                     VehicleProfile &vehicleProfile);

    /*!
     * \brief Imports all VehicleComponentes contained in one VehicleProfile
     * \param vehicleProfileElement  Element containing the information
     * \param vehicleProfile         VehicleProfile to fill
     * \return
     */
    static bool ImportAllVehicleComponentsOfVehicleProfile(QDomElement vehicleProfileElement,
                                                           VehicleProfile &vehicleProfile);

    /*!
     * \brief Imports a single VehicleComponentes contained in one VehicleProfile
     * \param vehicleComponentElement  Element containing the information
     * \param vehicleComponent         VehicleComponent to fill
     * \return
     */
    static bool ImportVehicleComponent(QDomElement vehicleComponentElement,
                                       VehicleComponent &vehicleComponent);


    /*!
     * \brief Imports all SensorLinks of a VehicleComponents contained in one VehicleProfile
     * \param sensorLinksElement      Element containing the information
     * \param sensorLinksElement      Map into which SensorLinks are saved
     * \return
     */
    static bool ImportSensorLinksOfComponent(QDomElement sensorLinksElement,
                                             std::list<SensorLink> &sensorLinks);

    /*!
     * \brief Imports all Sensor contained in one VehicleProfiles
     * \param vehicleProfileElement  Element containing the information
     * \param vehicleProfile         VehicleProfile to fill
     * \return
     */
    static bool ImportAllSensorsOfVehicleProfile(QDomElement vehicleProfileElement,
                                                 VehicleProfile &vehicleProfile);

    /*!
     * \brief Imports a single Sensor contained in one VehicleProfile
     * \param sensorElement           Element containing the information
     * \param sensorParameter         SensorParameter to fill
     * \return
     */
    static bool ImportSensorParameters(QDomElement sensorElement,
                                       SensorParameter &sensorParameter);

    //Overall import function
    /*!
    * \brief Imports the entire profiles catalog
    * \details Calls all sections specific methods and saves the result in the CombiantionConfig
    *
    * @param[in]     filename                Name of the SlaveConfig file
    * @param[out]    slaveConfig       Calss into which the values get saved
    * @return	     true, if successful
    */
    static bool Import(const std::string &filename, Profiles &profiles);

private:
    static constexpr auto profilesCatalogFile = "ProfilesCatalog.xml";
    static constexpr auto supportedConfigVersion = "0.2.1";

};
} //namespace Importer
