/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
