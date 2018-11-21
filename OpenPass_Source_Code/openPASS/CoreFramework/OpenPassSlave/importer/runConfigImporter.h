/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  runConfigImporter.h
//! @brief This file contains the importer of the simulation run configuration.
//-----------------------------------------------------------------------------

#ifndef RUNCONFIGIMPORTER_H
#define RUNCONFIGIMPORTER_H

#include <string>
#include "runConfig.h"

namespace SimulationCommon
{

class RunConfigImporter
{
public:    
    RunConfigImporter() = delete;
    RunConfigImporter(const RunConfigImporter&) = delete;
    RunConfigImporter(RunConfigImporter&&) = delete;
    RunConfigImporter& operator=(const RunConfigImporter&) = delete;
    RunConfigImporter& operator=(RunConfigImporter&&) = delete;
    virtual ~RunConfigImporter() = delete;

    static RunConfig *Import(const std::string &filename);
};

} // namespace SimulationCommon

#endif // RUNCONFIGIMPORTER_H
