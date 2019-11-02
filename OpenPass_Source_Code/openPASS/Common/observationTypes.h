/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#define OBSERVATION_LOG_VALUE_UNDEFINED ""
#define NFOLLOWERS 10

#include <string>

enum class LoggingGroup
{
    Trace,
    Visualization,
    RoadPosition,
    Vehicle,
    Sensor,
    Driver
};

const std::string AgentCategoryStrings[] =
{
    "Ego",
    "Scenario",
    "Common"
};
