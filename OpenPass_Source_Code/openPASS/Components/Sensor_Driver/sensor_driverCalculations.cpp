/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "sensor_driverCalculations.h"

double SensorDriverCalculations::GetLateralDistanceToObject(const AgentInterface *otherObject)
{
    if (otherObject == nullptr)
    {
        return INFINITY;
    }

    double lateralPositionOfOtherObject = otherObject->GetPositionLateral();
    if (otherObject->GetMainLaneId(MeasurementPoint::Reference) == agent->GetMainLaneId(MeasurementPoint::Reference))
    {
        return lateralPositionOfOtherObject - agent->GetPositionLateral();
    }
    if (otherObject->GetMainLaneId(MeasurementPoint::Reference) == agent->GetLaneIdLeft())
    {
        return lateralPositionOfOtherObject - agent->GetPositionLateral() + 0.5 * agent->GetLaneWidth() + 0.5 * agent->GetLaneWidth(1);
    }
    if (otherObject->GetMainLaneId(MeasurementPoint::Reference) == agent->GetLaneIdRight())
    {
        return lateralPositionOfOtherObject - agent->GetPositionLateral() - 0.5 * agent->GetLaneWidth() - 0.5 * agent->GetLaneWidth(-1);
    }
    return INFINITY;
}
