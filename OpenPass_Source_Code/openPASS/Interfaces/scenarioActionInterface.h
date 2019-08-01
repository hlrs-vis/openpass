/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <vector>
#include <string>

class ScenarioActionInterface
{
public:
    ScenarioActionInterface() = default;
    ScenarioActionInterface(const ScenarioActionInterface&) = delete;
    ScenarioActionInterface(ScenarioActionInterface&&) = delete;
    ScenarioActionInterface& operator=(const ScenarioActionInterface&) = delete;
    ScenarioActionInterface& operator=(ScenarioActionInterface&&) = delete;
    virtual ~ScenarioActionInterface() = default;

    virtual const std::vector<std::string>& GetEventDetectorNames() const = 0;
    virtual const std::vector<std::string>& GetActors() const = 0;
    virtual const std::string& GetSequenceName() const = 0;
private:
};
