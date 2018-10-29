/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
