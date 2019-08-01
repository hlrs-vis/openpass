/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <list>
namespace SimulationSlave
{
class Agent;

enum class SpawnControlError{
    NoError,
    IncompleteScenario,
    AgentGenerationError,
};

class SpawnControlInterface
{
public:
    virtual bool Execute(int) = 0;
    virtual std::list<const Agent*> PullNewAgents() = 0;
    virtual SpawnControlError GetError() const = 0;
};

} // namespace SimulationSlave

