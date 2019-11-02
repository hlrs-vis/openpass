/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "Interfaces/agentInterface.h"

//! Utility class for the sensor driver
class SensorDriverCalculations
{
public:
    SensorDriverCalculations(AgentInterface* agent) :
        agent(agent)
    {}

    //!
    //! \brief Calculates the lateral distance (i.e. distance in t) between the own
    //! agent and another agent
    //!
    double GetLateralDistanceToObject(const AgentInterface *otherObject);

private:
    AgentInterface* agent;
};
