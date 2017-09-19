/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

/** \addtogroup SpawnPoint_Single_0
* @{
* \brief spawns vehicles into the scenery
*
* This component spawns one vehicle at a given point, with given velocity, acceleration and yaw angle in the scenery.
*
* \section SpawnPoint_Single_0_ExternalParameters External parameters
* name | value | meaning
* -----|-------|-------------
* PositionX | -1 | position of the agent in X directory
* PositionY | -1 | position of the agent in Y directory
* VelocityX | 0 | velocity of the agent in X directory
* VelocityY | 0 | velocity of the agent in Y directory
* AccelerationX | 0 | acceleration of the agent in X directory
* AccelerationY | 0 | acceleration of the agent in Y directory
* YawAngle | 0 | yaw angle of the agent
*
* \section SpawnPoint_Single_0_ConfigParameters Parameters to be specified in runConfiguration.xml
* type | id | meaning | corresponding external parameter
* -----|----|---------|---------------------------------
* Double | 0 | position of the agent in X directory | PositionX
* Double | 1 | position of the agent in Y directory | PositionY
* Double | 2 | velocity of the agent in X directory | VelocityX
* Double | 3 | velocity of the agent in Y directory | VelocityY
* Double | 4 | acceleration of the agent in X directory | AccelerationX
* Double | 5 | acceleration of the agent in Y directory | AccelerationY
* Double | 6 | yaw angle of the agent | YawAngle
*
* \section SpawnPoint_Single_0_InternalParameters Internal paramters
* none
*
*
*   @} */

#ifndef SPAWNPOINTSINGLE_0_IMPLEMENTATION_H
#define SPAWNPOINTSINGLE_0_IMPLEMENTATION_H

#include "spawnPointInterface.h"

/*!
* \brief spawn one agent.
*
* This class implements a SpawnPointInterface which is used by the framework to set the start position,
* velocity, acceleration and yaw angle of one agent in the scenery.
*
* \ingroup SpawnPoint_Single_0
*/
class SpawnPointSingle_0_Implementation : public SpawnPointInterface
{
public:
    using SpawnPointInterface::SpawnPointInterface;
    SpawnPointSingle_0_Implementation(const SpawnPointSingle_0_Implementation&) = delete;
    SpawnPointSingle_0_Implementation(SpawnPointSingle_0_Implementation&&) = delete;
    SpawnPointSingle_0_Implementation& operator=(const SpawnPointSingle_0_Implementation&) = delete;
    SpawnPointSingle_0_Implementation& operator=(SpawnPointSingle_0_Implementation&&) = delete;
    virtual ~SpawnPointSingle_0_Implementation() = default;

    /*!
     * \brief SetSpawnItem
     * sets the spawnItem with parameters which is used to calculate the spawning position and time.
     * @param[inout] spawnItem spawn item which shall be spawned in the world.
     * @param[in] maxIndex maximum index of agent array configuration
     * (spawnpoint implementation must not exceed this index)
     */
    virtual void SetSpawnItem(SpawnItemParameterInterface &spawnItem, int maxIndex);
};

#endif // SPAWNPOINTSINGLE_0_IMPLEMENTATION_H
