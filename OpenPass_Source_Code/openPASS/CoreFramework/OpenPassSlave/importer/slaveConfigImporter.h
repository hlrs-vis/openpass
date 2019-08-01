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
//! @file  SlaveConfigImporter.h
//! @brief This file contains the importer of the slave configuration.
//-----------------------------------------------------------------------------
#pragma once

#include <QDomDocument>
#include <unordered_map>

#include "configurationFiles.h"
#include "slaveConfig.h"
#include "parameters.h"
#include "parameterImporter.h"
#include "xmlParser.h"

namespace Importer {

//-----------------------------------------------------------------------------
/** \brief This class povides the import functionality
*   \details All methods in this class are static
*
*   \ingroup Importers
*/
//-----------------------------------------------------------------------------
class SlaveConfigImporter
{
public:
    /*!
    * \brief Imports the logging groups for the observer
    *
    * @param[in]     loggingGroupsElement   Element containing the logging groups information
    * @param[out]    loggingGroups          Vector where the logging groups get saved
    * @return        true, if successful
    */
    static bool ImportLoggingGroups(QDomElement loggingGroupsElement, std::vector<std::string>& loggingGroups);

    /*!
    * \brief Imports the libraries used by the simulator
    *
    * @param[in]     librariesElement   Element containing the libraries information
    * @return        Libraries (defaulted if element or single elements are missing)
    * @see           defaultLibraryMapping
    */
    static ExperimentConfig::Libraries ImportLibraries(QDomElement rootElement);

    //Importing specific sections of the SlaveConfig
    /*!
    * \brief Imports the ExperimentConfig section of the slave configuration
    *
    * @param[in]     experimentConfigElement    Element containing the information
    * @param[out]    experimentConfig           Struct into which the values get saved
    * @return        true, if successful
    */
    static bool ImportExperimentConfig(QDomElement experimentConfigElement,
                                       ExperimentConfig& experimentConfig);

    /*!
    * \brief Imports the ScenarioConfig section of the slave configuration
    *
    * @param[in]     scenarioConfigElement    Element containing the information
    * @param[out]    scenarioConfig           Struct into which the values get saved
    * @return        true, if successful
    */
    static bool ImportScenarioConfig(QDomElement scenarioConfigElement,
                                     const std::string configurationDir,
                                     ScenarioConfig& scenarioConfig);

    /*!
    * \brief Imports the EnvironmentConfig section of the slave configuration
    *
    * @param[in]     environmentConfigElement    Element containing the information
    * @param[out]    environmentConfig           Struct into which the values get saved
    * @return        true, if successful
    */
    static bool ImportEnvironmentConfig(QDomElement environmentConfigElement,
                                        EnvironmentConfig& environmentConfig);

    /*!
    * \brief Imports the TrafficParameter subsection of trafficConfig the slave configuration
    *
    * @param[in]     trafficConfigElement    Element containing the information
    * @param[out]    trafficConfig           Struct into which the values get saved
    * @return        true, if successful
    */
    static bool ImportTrafficParameter(QDomElement trafficConfigElement,
                                       TrafficConfig& trafficConfig);

    /*!
    * \brief Imports the LaneParameter subsection of trafficConfig the slave configuration
    *
    * @param[in]     trafficConfigElement    Element containing the information
    * @param[out]    trafficConfig           Struct into which the values get saved
    * @return        true, if successful
    */
    static bool ImportLaneParameter(QDomElement trafficConfigElement,
                                    TrafficConfig& trafficConfig);

    /*!
    * \brief Imports the TrafficConfig section of the slave configuration
    *
    * @param[in]     trafficConfigElement    Element containing the information
    * @param[out]    trafficConfig           Struct into which the values get saved
    * @return        true, if successful
    */
    static bool ImportTrafficConfig(QDomElement trafficConfigElement,
                                    TrafficConfig& trafficConfig);

    //Overall import function
    /*!
    * \brief Imports the entire slave configuration
    * \details Calls all sections specific methods and saves the result in the CombiantionConfig
    *
    * @param[in]     configurationDir   Path of the configuration files
    * @param[in]     slaveConfigFile    Name of the SlaveConfig file
    * @param[out]    slaveConfig        Class into which the values get saved
    * @return        true, if successful
    */
    static bool Import(const std::string& configurationDir,
                       const std::string& slaveConfigFile,
                       Configuration::SlaveConfig& slaveConfig);

private:
    static std::string GetLibrary(const QDomElement& root, std::string key, std::string tag);
    static constexpr auto supportedConfigVersion = "0.6.0";

    //! \brief Identifier with correspondiong default values for mandatory libraries
    //! \note: The identifier is identical to the XML tag
    inline static const ExperimentConfig::Libraries defaultLibraryMapping =
    {
        {"EventDetectorLibrary", "EventDetector"},
        {"ManipulatorLibrary", "Manipulator"},
        {"SpawnPointLibrary", "SpawnPoint"},
        {"WorldLibrary", "World"},
        {"ObservationLibrary", "Observation"},
        {"StochasticsLibrary", "Stochastics"}
    };
};

} //namespace Importer
