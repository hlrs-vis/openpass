/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#include <QtGlobal>
#include "spawnpoint_start_implementation.h"

SpawnPoint_Start_Implementation::SpawnPoint_Start_Implementation(StochasticsInterface *stochastics,
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

void SpawnPoint_Start_Implementation::SetSpawnItem(SpawnItemParameterInterface &spawnItem, int maxIndex)
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

    // set position X
    spawnItem.SetPositionX(0);

    // set position Y
    spawnItem.SetPositionY(0);

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
    spawnItem.SetYawAngle(0);

    return;
}
