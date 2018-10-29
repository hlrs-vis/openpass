/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#ifndef SPAWNPOINT_START_IMPLEMENTATION_H
#define SPAWNPOINT_START_IMPLEMENTATION_H

#include <math.h>
#include "spawnPointInterface.h"

/**
* \addtogroup CoreModules_Basic openPASS CoreModules basic
* @{
* \addtogroup SpawnPoint_Start
* @{
*
* \brief Spawn agents at the beginning of road.
*
* This SpawnPoint module implements a basic SpawnPointInterface which is used
* by the framework.
* It used to spawn several agents at the beginning of the road with random position and
* velocity.
*
* \section SpawnPoint_Start_ExternalParameters External parameters
* name | value | meaning
* -----|-------|-------------
* Par_NSpawnCars | 10 | The maximum number of vehicles that will be spawned on the road. However, the simulation of each situation might stop before this amount is reached. The amount of spawned vehicles might therefore vary between different situations of the same simulation.
* Par_demandHighway | 900 | The traffic demand of the simulated road. The number states how many vehicles pass a point on the road per hour.
* Par_vMean | 25 | mean velocity in m/s
* Par_vSd | 3 | velocity standard deviation
*
* \section spawnpoint_start_Parameters Parameters to be specified in runConfiguration.xml
*
* --------------------------------------------------
* |type | id | meaning | corresponding external paramter
* |-----|----|---------|----------------------------------
* int | 0 | Number of spawned cars | Par_NSpawnCars
* double | 0 | demand of highway in cars/hour | Par_demandHighway
* double | 1 | mean velocity in m/s | Par_vMean
* double | 2 | minimal velocity in m/s | Par_vMin
* double | 3 | velocity standard deviation | Par_vSd
*
* \section SpawnPoint_Start_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|-------------
* standardCycleTime | 100 | standard time for one cycle in ms
*
* \see SpawnPointInterface
*   @}
*   @} */

/*!
 * \brief Spawn agents at the beginning of road.
 *
 * This SpawnPoint module implements a basic SpawnPointInterface which is used
 * by the framework.
 * It used to spawn several agents at the beginning of the road with random position and
 * velocity.
 *
 * \ingroup SpawnPoint_Start
 */
class SpawnPoint_Start_Implementation : public SpawnPointInterface
{
public:
    const std::string COMPONENTNAME = "SpawnPoint_Start";

    SpawnPoint_Start_Implementation(
        StochasticsInterface *stochastics,
        WorldInterface *world,
        const ParameterInterface *parameters,
        const CallbackInterface *callbacks);
    SpawnPoint_Start_Implementation(const SpawnPoint_Start_Implementation &) = delete;
    SpawnPoint_Start_Implementation(SpawnPoint_Start_Implementation &&) = delete;
    SpawnPoint_Start_Implementation &operator=(const SpawnPoint_Start_Implementation &) = delete;
    SpawnPoint_Start_Implementation &operator=(SpawnPoint_Start_Implementation &&) = delete;
    virtual ~SpawnPoint_Start_Implementation() = default;

    virtual void SetSpawnItem(SpawnItemParameterInterface &spawnItem, int maxIndex);

private:

    /**
        * \addtogroup SpawnPoint_Start
        * @{
        *
        *    @name External Parameters
        *    @{*/
    //! Number of spawned cars
    int Par_NSpawnCars = 100;
    //! demand of highway in cars/hour
    double Par_demandHighway = 900;
    //! mean velocity in m/s
    double Par_vMean = 25;
    //! minimal velocity in m/s
    double Par_vMin;
    //! velocity standard deviation
    double Par_vSd = 3;

    /**    @}
           *    @name Internal Parameters
           *    @{*/
    //! standard cycle time for spawning, update, trigger tasks
    int _standardCycleTime = 100;
    /**@}*/
    /**@}*/

    //! spawning car rate in cars/second
    double _carRate;
    //! number of cars already spawned
    int _iSpawnCar = 0;
};

#endif // SPAWNPOINT_START_IMPLEMENTATION_H
