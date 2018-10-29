/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef SPAWNPOINT_PCM_IMPLEMENTATION_H
#define SPAWNPOINT_PCM_IMPLEMENTATION_H

#include "spawnPointInterface.h"
#include "componentPorts.h"

/**
* \addtogroup CoreModules_PCM openPASS CoreModules pcm
* @{
* \addtogroup SpawnPoint_PCM
*
* \brief Spawnpoint Module to spawn one agent into the scenery.
*
* This class implements a SpawnPointInterface which is used by the framework to set the start position,
* velocity, acceleration and yaw angle of one agent in the scenery.
*
* @see SpawnPointInterface
* @}
*/

/*!
 * \copydoc SpawnPoint_PCM
 * \ingroup SpawnPoint_PCM
 */
class SpawnPoint_PCM_Implementation : public SpawnPointInterface
{
public:
    SpawnPoint_PCM_Implementation(
        StochasticsInterface *stochastics,
        WorldInterface *world,
        const ParameterInterface *parameters,
        const CallbackInterface *callbacks);

    SpawnPoint_PCM_Implementation(const SpawnPoint_PCM_Implementation &) = delete;
    SpawnPoint_PCM_Implementation(SpawnPoint_PCM_Implementation &&) = delete;
    SpawnPoint_PCM_Implementation &operator=(const SpawnPoint_PCM_Implementation &) = delete;
    SpawnPoint_PCM_Implementation &operator=(SpawnPoint_PCM_Implementation &&) = delete;
    virtual ~SpawnPoint_PCM_Implementation() = default;

    /*!
     * \brief SetSpawnItem
     * sets the spawnItem with parameters which is used to calculate the spawning position and time.
     * @param[inout] spawnItem spawn item which shall be spawned in the world.
     * @param[in] maxIndex maximum index of agent array configuration
     * (spawnpoint implementation must not exceed this index)
     */
    virtual void SetSpawnItem(SpawnItemParameterInterface &spawnItem, int maxIndex);

    std::map<int, externalParameter<double>*> parameterMapDouble;
    /** \addtogroup SpawnPoint_PCM
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in runConfiguration file.
     *      @{
     */
    externalParameter<double> positionX {0, &parameterMapDouble }; //!< position of the agent in X directory
    externalParameter<double> positionY {1, &parameterMapDouble }; //!< position of the agent in Y directory
    externalParameter<double> velocityX {2, &parameterMapDouble }; //!< velocity of the agent in X directory
    externalParameter<double> velocityY {3, &parameterMapDouble }; //!< velocity of the agent in Y directory
    externalParameter<double> accelerationX {4, &parameterMapDouble }; //!< acceleration of the agent in X directory
    externalParameter<double> accelerationY {5, &parameterMapDouble }; //!< acceleration of the agent in Y directory
    externalParameter<double> yawAngle {6, &parameterMapDouble }; //!< yaw angle of the agent
    /**
     *      @}
     *  @}
     */
};

#endif // SPAWNPOINT_PCM_IMPLEMENTATION_H
