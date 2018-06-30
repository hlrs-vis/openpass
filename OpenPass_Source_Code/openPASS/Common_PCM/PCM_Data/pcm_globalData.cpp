/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
