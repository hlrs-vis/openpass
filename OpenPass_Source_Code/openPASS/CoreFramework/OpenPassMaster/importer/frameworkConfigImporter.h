/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  frameworkConfigImporter.h
//! @brief This file contains the importer of the framework configuration.
//-----------------------------------------------------------------------------

#ifndef FRAMEWORDCONFIGIMPORTER_H
#define FRAMEWORDCONFIGIMPORTER_H

namespace SimulationMaster
{

class FrameworkConfig;

class FrameworkConfigImporter
{
public:    
    FrameworkConfigImporter() = delete;
    FrameworkConfigImporter(const FrameworkConfigImporter&) = delete;
    FrameworkConfigImporter(FrameworkConfigImporter&&) = delete;
    FrameworkConfigImporter& operator=(const FrameworkConfigImporter&) = delete;
    FrameworkConfigImporter& operator=(FrameworkConfigImporter&&) = delete;
    virtual ~FrameworkConfigImporter() = delete;

    static FrameworkConfig *Import(const std::string &filename);
};

} // namespace SimulationMaster

#endif // FRAMEWORDCONFIGIMPORTER_H
