/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "scenarioImporter.h"

namespace SimulationSlave {

//-----------------------------------------------------------------------------
//! Imports openScenario from a given file
//!
//!
//! @param[in]  filename
//! @param[out] globalObjects
//!
//! @return                         False if an error occurred, true otherwise
//-----------------------------------------------------------------------------
bool ScenarioImporter::Import(const std::string &filename,
                              WorldInterface *world)
{
    std::locale::global(std::locale("C"));

    QString absoluteFilePath = "";

    if (QFileInfo(QString::fromStdString(filename)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        absoluteFilePath = baseDir.absoluteFilePath(QString::fromStdString(filename));
        absoluteFilePath = baseDir.cleanPath(absoluteFilePath);
    }
    else
    {
        absoluteFilePath = QString::fromStdString(filename);
    }

    if (!QFileInfo(absoluteFilePath).exists())
    {
        LOG_INTERN(LogLevel::Warning) << "Scenario Config: "
                                      << absoluteFilePath.toStdString()
                                      << " does not exist.";
        return false;
    }

    return world->CreateWorldScenario(absoluteFilePath.toStdString());
}

} // namespace SimulationSlave
