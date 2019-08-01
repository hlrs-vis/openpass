/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <string>
#include "directories.h"

/// Container for configuration file related paths
/// \see Directories
struct ConfigurationFiles
{
    ConfigurationFiles(std::string configurationDir,
                       std::string systemConfigBlueprintFile,
                       std::string slaveConfigFile) :
        configurationDir{configurationDir},
        systemConfigBlueprintFile{Directories::Concat(configurationDir, systemConfigBlueprintFile)},
        slaveConfigFile{Directories::Concat(configurationDir, slaveConfigFile)}
    {}

    // This class should not be moved or assigned,
    // as there should be only one instance throughout the system
    ConfigurationFiles() = delete;
    ConfigurationFiles(const Directories&) = delete;
    ConfigurationFiles(Directories&&) = delete;
    ConfigurationFiles& operator=(Directories&&) = delete;

    const std::string configurationDir;            //!< directory of the configuration files
    const std::string systemConfigBlueprintFile;   //!< absolute path of the systemConfigBlueprintFile
    const std::string slaveConfigFile;             //!< absolute path of the slaveConfigFile
};
