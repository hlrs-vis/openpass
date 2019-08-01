/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "runStatistic.h"

class WorldInterface;

/*!
 * \brief The RunStatisticCalculation class provides helper functions for run statistic analysis
 */
class RunStatisticCalculation
{
public:
    /*!
     * \brief Calculates the total distance traveled of all agents in the simulation
     *
     * The members `EgoDistanceTraveled` and `TotalDistanceTraveled` of the first parameter
     * are used to store the result
     *
     * \param[out] runStatistic      Result is stored here
     * \param[in]  world             Pointer to the world
     */
    static void CalculateTotalDistanceTraveled(RunStatistic& runStatistic, WorldInterface* world);

    /*!
     * \brief Counts the number of collisions in the simulation
     *
     * The members `NCollisionsArbitrary`, `NCollisionsFollower` and `EgoCollision` of the first parameter
     * are used to store the result
     *
     * \param[out] runStatistic      Result is stored here
     * \param[in]  runResult         Structure to retrieve collisions from
     * \param[in]  world             Pointer to the world
     */
    static void CalculateNumberOfCollisions(RunStatistic& runStatistic, const RunResultInterface& runResult, WorldInterface* world);
};
