/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  agentTypeImporter.h
//! @brief This file contains the importer of the agent types.
//-----------------------------------------------------------------------------

#ifndef AGENTTYPEIMPORTER_H
#define AGENTTYPEIMPORTER_H

#include <string>
#include <map>

namespace SimulationSlave
{

class AgentType;

//-----------------------------------------------------------------------------
//! Class handles importing an agent type.
//-----------------------------------------------------------------------------
class AgentTypeImporter
{
public:
    AgentTypeImporter() = delete;
    AgentTypeImporter(const AgentTypeImporter&) = delete;
    AgentTypeImporter(AgentTypeImporter&&) = delete;
    AgentTypeImporter& operator=(const AgentTypeImporter&) = delete;
    AgentTypeImporter& operator=(AgentTypeImporter&&) = delete;
    virtual ~AgentTypeImporter() = delete;

    //-----------------------------------------------------------------------------
    //! Imports the agent types from the provided file path and stores
    //! the results in the provided mapping.
    //!
    //! @param[in]  filename            Path to the file with the agent types
    //! @param[out] agentTypes          Mapping from IDs to agents to store the results
    //!                                 of the import
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool Import(const std::string &filename, std::map<int, const AgentType*> &agentTypes);
};

} // namespace SimulationSlave

#endif // AGENTTYPEIMPORTER_H
