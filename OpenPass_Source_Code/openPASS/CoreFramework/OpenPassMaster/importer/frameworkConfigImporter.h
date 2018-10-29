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
