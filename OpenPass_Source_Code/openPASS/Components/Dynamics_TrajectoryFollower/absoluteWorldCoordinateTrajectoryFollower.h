/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/*!
 * \addtogroup Dynamics_TrajectoryFollower
 * @{
 * \brief makes an agent strictly follow a predefined path
 *
 * The path is defined in a file that is loaded in the importer into a map.
 * The module simply reads the coordinates defined in the map every step and forces the agent to move to that position.
 * The vehicle is in this mode very simplified (there is no driver, no logic or any intelligence). It's basically a "playback" of the trajectory.
 * @} */

#pragma once

#include "trajectoryFollowerCommonBase.h"

/*!
 * \brief Makes an agent strictly follow a predefined path.
 *
 * \ingroup Dynamics_TrajectoryFollower
 */
class AbsoluteWorldCoordinateTrajectoryFollower : public TrajectoryFollowerCommonBase
{
public:
    AbsoluteWorldCoordinateTrajectoryFollower(
            std::string componentName,
            bool isInit,
            int priority,
            int offsetTime,
            int responseTime,
            int cycleTime,
            StochasticsInterface *stochastics,
            WorldInterface *world,
            const ParameterInterface *parameters,
            const std::map<int, ObservationInterface*> *observations,
            const CallbackInterface *callbacks,
            AgentInterface *agent,
            TrajectoryInterface *trajectory);

    AbsoluteWorldCoordinateTrajectoryFollower(const AbsoluteWorldCoordinateTrajectoryFollower&) = delete;
    AbsoluteWorldCoordinateTrajectoryFollower(AbsoluteWorldCoordinateTrajectoryFollower&&) = delete;
    AbsoluteWorldCoordinateTrajectoryFollower& operator=(const AbsoluteWorldCoordinateTrajectoryFollower&) = delete;
    AbsoluteWorldCoordinateTrajectoryFollower& operator=(AbsoluteWorldCoordinateTrajectoryFollower&&) = delete;
    virtual ~AbsoluteWorldCoordinateTrajectoryFollower() = default;

    /*!
    * \brief Process data within component.
    *
    * Function is called by framework when the scheduler calls the trigger task
    * of this component
    *
    * @param[in]     time           Current scheduling time
    */
    virtual void Trigger(int time);

private:
    Common::Vector2d CalculateScaledVector(const Position &previousPosition, const Position &nextPosition, const double &factor);
    double CalculateScaledDeltaYawAngle(const Position &previousPosition, const Position &nextPosition, const double &factor);

    Position CalculateStartPosition(const Position &previousPosition, const Position &nextPosition);
    std::pair<int, Position> CalculateStartCoordinate(const std::pair<int, Position> &previousPosition, const std::pair<int, Position> &nextPosition);
    double CalculateDistanceBetweenWorldCoordinates(Position previousPosition, Position nextPosition);

    void TriggerWithActiveAccelerationInput();
    void TriggerWithInactiveAccelerationInput();

    void UpdateDynamics(const Position &previousPosition,
                        const Common::Vector2d &direction,
                        const double &deltaYawAngle,
                        const double &velocity,
                        const double &acceleration);

    WorldCoordinateTrajectory absoluteWorldTrajectory {};
    WorldCoordinateTrajectory::iterator previousTrajectoryIterator {};
    WorldCoordinateTrajectory::iterator nextTrajectoryIterator {};

    double percentageTraveledBetweenCoordinates {0};
};


