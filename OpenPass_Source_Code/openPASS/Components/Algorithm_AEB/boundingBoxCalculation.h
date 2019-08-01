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
#include <boost/geometry/geometries/adapted/c_array.hpp>
#include "Common/boostGeometryCommon.h"
#include "Interfaces/agentInterface.h"
#include "osi/osi_sensordata.pb.h"

/** \addtogroup Algorithm_AEB
 * @{
 * \brief Utility class for AEB to predict the bounding box of objects in future timesteps
* @} */
class BoundingBoxCalculation
{
public:
    BoundingBoxCalculation(AgentInterface *agent, double collisionDetectionLongitudinalBoundary, double collisionDetectionLateralBoundary);

    /*!
     * \brief Predicts the bounding box of a moving object in local coordinates
     * \param timeStepInSeconds relative time since current timeStep
     * \param baseMoving        object to predict
     * \return predicted bounding box
     */
    polygon_t CalculateBoundingBox(double timeStepInSeconds, const osi3::BaseMoving &baseMoving);

    /*!
     * \brief Calculates the bounding box of a stationary object in local coordinates
     * \param baseMoving        object to calculate
     * \return calculated bounding box
     */
    polygon_t CalculateBoundingBox(const osi3::BaseStationary &baseStationary);

    /*!
     * \brief Predicts the bounding box of the own agent in local coordinates (coordinate system does not move along)
     * \param timeStepInSeconds relative time since current timeStep
     * \return predicted bounding box
     */
    polygon_t CalculateOwnBoundingBox(double timeStepInSeconds);

private:
    AgentInterface* agent; ///!< The agent for which the boundingbox should be calculated
    double collisionDetectionLongitudinalBoundary; ///!< Additional length added to the vehicle boundary when checking for collision detection
    double collisionDetectionLateralBoundary; ///!< Additional width added to the vehicle boundary when checking for collision detection
};
