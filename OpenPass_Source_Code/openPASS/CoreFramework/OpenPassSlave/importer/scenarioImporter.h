/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  scenarioImporter.h
//! @brief This file contains the importer of the scenario configuration.
//-----------------------------------------------------------------------------

#ifndef SCENARIOIMPORTER_H
#define SCENARIOIMPORTER_H

#include <QApplication>
#include <algorithm>
#include <utility>
#include <limits>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <QDir>
#include <QFile>
#include "log.h"
#include "xmlParser.h"
#include <QDomDocument>
#include "worldInterface.h"
#include "scenery.h"

namespace SimulationSlave {

class ScenarioImporter
{
public:
    ScenarioImporter() = default;
    ScenarioImporter(const ScenarioImporter &) = delete;
    ScenarioImporter(ScenarioImporter &&) = delete;
    ScenarioImporter &operator=(const ScenarioImporter &) = delete;
    ScenarioImporter &operator=(ScenarioImporter &&) = delete;
    virtual ~ScenarioImporter() = default;

    //-----------------------------------------------------------------------------
    //! Imports data structures from the openScenario configuration file
    //!
    //! @param[in]  filename       path to openScenario file
    //! @param[in]  world          the destination world
    //! @return                    true on success
    //-----------------------------------------------------------------------------
    static bool Import(const std::string &filename,
                       WorldInterface *world);
};

} // namespace SimulationSlave

#endif // SCENARIOIMPORTER_H