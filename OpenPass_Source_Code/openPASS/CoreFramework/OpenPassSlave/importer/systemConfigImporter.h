/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

////-----------------------------------------------------------------------------
////! @file  agentSystemImporter.h
////! @brief This file contains the importer of the agent system configuration.
////-----------------------------------------------------------------------------

#pragma once

#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <string>
#include <memory>
#include <map>
#include <iostream>

#include "CoreFramework/CoreShare/log.h"
#include "componentType.h"
#include "systemConfig.h"
#include "agentType.h"
#include "parameters.h"
#include "parameterImporter.h"
#include "xmlParser.h"

using namespace Configuration;

namespace Importer {
//-----------------------------------------------------------------------------
//! Class handles importing an agent system configuration.
//-----------------------------------------------------------------------------
class SystemConfigImporter
{
public:
    SystemConfigImporter(const SystemConfigImporter&) = delete;
    SystemConfigImporter(SystemConfigImporter&&) = delete;
    SystemConfigImporter& operator=(const SystemConfigImporter&) = delete;
    SystemConfigImporter& operator=(SystemConfigImporter&&) = delete;

    //-----------------------------------------------------------------------------
    //! Imports the XML agent system configuration from the provided file path and
    //! stores the results in the provided mapping.
    //!
    //! @param[in]  filename            Path to the file with the XML agent system config
    //! @param[out] agentTypes          Mapping from IDs to agents to store the results
    //!                                 of the import
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool Import(const std::string &filename, std::shared_ptr<SystemConfig> systemConfig);


private:
    static bool ImportSystemParameters(QDomElement &parametersElement, ParameterInterface* parameters);
    static bool ImportSystemConfigContent(const std::string &filename, QDomDocument& document);

};

} //namespace Importer
