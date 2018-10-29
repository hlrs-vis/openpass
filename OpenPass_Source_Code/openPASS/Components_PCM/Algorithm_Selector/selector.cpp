/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "selector.h"


void Selector::Perform(double driverThrottle, double driverBrake, double driverSteering,
                       bool brakeAssistActive, double brakeAssistBrake, bool laneAssistActive,
                       std::vector<double> laneAssistBrakeSuperpose, bool evasiveSteeringActive,
                       double evasiveSteeringAngle, bool collisionOccured)
{
    resultingThrottle = driverThrottle;
    resultingBrake = driverBrake;
    resultingSteering = driverSteering;
    resultingBrakeSuperpose = {0.0, 0.0, 0.0, 0.0};

    if (collisionState == false)
    {
        if (collisionOccured == true)
        {
            collisionState = true;
            collisionSteering = driverSteering;
        }
    }

    if (collisionState == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = 1.0;
        resultingSteering = collisionSteering;
        resultingBrakeSuperpose = std::vector<double> {0.0, 0.0, 0.0, 0.0};
    }
    else if (evasiveSteeringActive == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = 0.0;
        resultingSteering = evasiveSteeringAngle;
        resultingBrakeSuperpose = std::vector<double> {0.0, 0.0, 0.0, 0.0};
        assistantActivation = true;
    }
    else if (brakeAssistActive == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = brakeAssistBrake;
        resultingSteering = driverSteering;
        resultingBrakeSuperpose = std::vector<double> {0.0, 0.0, 0.0, 0.0};
        assistantActivation = true;
    }
    else if (laneAssistActive == true)
    {
        resultingBrakeSuperpose = laneAssistBrakeSuperpose;
        assistantActivation = true;
    }
}

double Selector::GetResultingThrottle() const
{
    return resultingThrottle;
}

double Selector::GetResultingBrake() const
{
    return resultingBrake;
}

double Selector::GetResultingSteering() const
{
    return resultingSteering;
}

std::vector<double> Selector::GetResultingBrakeSuperpose() const
{
    return resultingBrakeSuperpose;
}
