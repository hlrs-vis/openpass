/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "spawnPointSingle_0_implementation.h"

void SpawnPointSingle_0_Implementation::SetSpawnItem(SpawnItemParameterInterface &spawnItem, int maxIndex)
{
    double result;

    // set index
    spawnItem.SetIndex(maxIndex);

    // set next spawn time
    spawnItem.SetNextTimeOffset(-1);

    // set position X
    try
    {
        result = GetParameters()->GetParametersDouble().at(0);
    }
    catch(const std::out_of_range&)
    {
        result = -1.0;
    }

    spawnItem.SetPositionX(result);

    // set position Y
    try
    {
        result = GetParameters()->GetParametersDouble().at(1);
    }
    catch(const std::out_of_range&)
    {
        result = -1.0;
    }

    spawnItem.SetPositionY(result);

    // set velocity X
    try
    {
        result = GetParameters()->GetParametersDouble().at(2);
    }
    catch(const std::out_of_range&)
    {
        result = 0.0;
    }

    spawnItem.SetVelocityX(result);

    // set velocity Y
    try
    {
        result = GetParameters()->GetParametersDouble().at(3);
    }
    catch(const std::out_of_range&)
    {
        result = 0.0;
    }

    spawnItem.SetVelocityY(result);

    // set acceleration X
    try
    {
        result = GetParameters()->GetParametersDouble().at(4);
    }
    catch(const std::out_of_range&)
    {
        result = 0.0;
    }

    spawnItem.SetAccelerationX(result);

    // set acceleration Y
    try
    {
        result = GetParameters()->GetParametersDouble().at(5);
    }
    catch(const std::out_of_range&)
    {
        result = 0.0;
    }

    spawnItem.SetAccelerationY(result);

    // set yaw angle
    try
    {
        result = GetParameters()->GetParametersDouble().at(6);
    }
    catch(const std::out_of_range&)
    {
        result = 0.0;
    }

    spawnItem.SetYawAngle(result);

    return;
}
