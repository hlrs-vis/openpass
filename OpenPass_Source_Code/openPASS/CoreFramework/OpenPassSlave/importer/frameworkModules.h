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

struct FrameworkModules
{
public:
    FrameworkModules(int logLevel,
                     std::string libraryDir,
                     std::string eventDetectorLibrary,
                     std::string manipulatorLibrary,
                     std::string observationLibrary,
                     std::string spawnPointLibrary,
                     std::string stochasticsLibrary,
                     std::string worldLibrary) :
        logLevel{logLevel},
        libraryDir{libraryDir},
        eventDetectorLibrary{Directories::Concat(libraryDir, eventDetectorLibrary)},
        manipulatorLibrary{Directories::Concat(libraryDir, manipulatorLibrary)},
        observationLibrary{Directories::Concat(libraryDir, observationLibrary)},
        spawnPointLibrary{Directories::Concat(libraryDir, spawnPointLibrary)},
        stochasticsLibrary{Directories::Concat(libraryDir, stochasticsLibrary)},
        worldLibrary{Directories::Concat(libraryDir, worldLibrary)}
    {}
    const int logLevel;
    const std::string libraryDir;
    const std::string eventDetectorLibrary;
    const std::string manipulatorLibrary;
    const std::string observationLibrary;
    const std::string spawnPointLibrary;
    const std::string stochasticsLibrary;
    const std::string worldLibrary;
};
