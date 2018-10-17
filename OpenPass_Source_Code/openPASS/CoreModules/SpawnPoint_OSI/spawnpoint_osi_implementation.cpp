/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* Copyright (c) 2018 in-tech GmbH.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <QtGlobal>
#include "spawnpoint_osi_implementation.h"

SpawnPoint_OSI_Implementation::SpawnPoint_OSI_Implementation(StochasticsInterface *stochastics,
                                                                           WorldInterface *world,
                                                                           const ParameterInterface *parameters,
                                                                           const CallbackInterface *callbacks):
    SpawnPointInterface(stochastics, world, parameters, callbacks)
{
    // read parameters
    try
    {
        Par_NSpawnCars = parameters->GetParametersInt().at(0);
        Par_demandHighway = parameters->GetParametersDouble().at(0);
        Par_vMean = parameters->GetParametersDouble().at(1);
        Par_vMin = parameters->GetParametersDouble().at(2);
        Par_vSd = parameters->GetParametersDouble().at(3);
    }catch(...){
        const std::string msg = COMPONENTNAME + " could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
    _carRate = Par_demandHighway / 3600; //demandHighway - cars per hour, carRate - cars per second
}

void SpawnPoint_OSI_Implementation::SetSpawnItem(SpawnItemParameterInterface &spawnItem, int maxIndex)
{
    Q_UNUSED(maxIndex);

    _iSpawnCar++;
    if(_iSpawnCar == Par_NSpawnCars)
    {
        // enough cars spawned
        spawnItem.SetNextTimeOffset(-1);
        return;
    }
    else
    {
        //spawn next car after tGap seconds
        double tNextTimeOffset = GetStochastics()->GetExponentialDistributed(_carRate); // in ms
        int tGap =  ::round(tNextTimeOffset*1000/_standardCycleTime) * _standardCycleTime; // convert to seconds and round to multiples of standardCycleTime
        spawnItem.SetNextTimeOffset(tGap);
    }

    // set index
    spawnItem.SetIndex(0);

    // NOTE: changes for OSI showcase, may need spawnpoint fork
    // set position X
    spawnItem.SetPositionX(6.);

    // set position Y
    int laneIdx = GetStochastics()->GetUniformDistributed(0.0, 1.0) > 0.5 ? 1 : 0;
    spawnItem.SetPositionY(laneIdx * 3.75 + 2.0);

    // set velocity X
    double velocity = std::max(Par_vMin, GetStochastics()->GetNormalDistributed(Par_vMean, Par_vSd)); //in m/s
    spawnItem.SetVelocityX(velocity);

    // set velocity Y
    spawnItem.SetVelocityY(0);

    // set acceleration X
    spawnItem.SetAccelerationX(0);

    // set acceleration Y
    spawnItem.SetAccelerationY(0);

    // set yaw angle
    int yawSign = GetStochastics()->GetUniformDistributed(0.0, 1.0) > 0.5 ? -1 : 1;
    spawnItem.SetYawAngle(yawSign * 0.1);

    return;
}
