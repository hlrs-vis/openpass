/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  agentConfigImporter.h
//! @brief This file contains the importer of the agent configuration.
//-----------------------------------------------------------------------------

#ifndef AGENTCONFIGIMPORTER_H
#define AGENTCONFIGIMPORTER_H

#include <string>
#include <map>

namespace SimulationSlave
{

class AgentType;

//-----------------------------------------------------------------------------
//! Class handles importing an agent configuration.
//-----------------------------------------------------------------------------
class AgentConfigImporter
{
public:
    AgentConfigImporter() = delete;
    AgentConfigImporter(const AgentConfigImporter&) = delete;
    AgentConfigImporter(AgentConfigImporter&&) = delete;
    AgentConfigImporter& operator=(const AgentConfigImporter&) = delete;
    AgentConfigImporter& operator=(AgentConfigImporter&&) = delete;
    virtual ~AgentConfigImporter() = delete;

    //-----------------------------------------------------------------------------
    //! Imports the XML agent configuration from the provided file path and stores
    //! the results in the provided mapping.
    //!
    //! @param[in]  filename            Path to the file with the XML agent config
    //! @param[out] agentTypes          Mapping from IDs to agents to store the results
    //!                                 of the import
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool Import(const std::string &filename, std::map<int, const AgentType*> &agentTypes);
};

} // namespace SimulationSlave

#endif // AGENTCONFIGIMPORTER_H
