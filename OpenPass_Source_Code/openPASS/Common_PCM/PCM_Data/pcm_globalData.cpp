/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_globalData.h"

double PCM_GlobalData::GetOffsetX() const
{
    return offsetX;
}

void PCM_GlobalData::SetOffsetX(double value)
{
    offsetX = value;
}

double PCM_GlobalData::GetOffsetY() const
{
    return offsetY;
}

void PCM_GlobalData::SetOffsetY(double value)
{
    offsetY = value;
}

int PCM_GlobalData::GetParticipants() const
{
    return participants;
}

void PCM_GlobalData::SetParticipants(int value)
{
    participants = value;
}

int PCM_GlobalData::GetSimulationVersion() const
{
    return simulationVersion;
}

void PCM_GlobalData::SetSimulationVersion(int value)
{
    simulationVersion = value;
}
