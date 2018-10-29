/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef POSTCRASHDYNAMIC_H
#define POSTCRASHDYNAMIC_H

#include "vector2d.h"

#define POSTCRASHDYNAMICID 0

/*!
 * \brief Class to store post-crash dynamics data
 * Used to store information about the dynamic after a collision.
 */
class PostCrashDynamic
{
public:
    //! Container Class to store all relevant data for a post crash dynamic
    //!
    //! \param velocity
    //! \param velocityChange
    //! \param velocityDirection
    //! \param yawVelocity
    //! \param pulse
    //! \param pulseDirection
    //! \param pulseLocal
    //! \param pointOfContactLocal
    //! \param collisionVelocity
    //! \param sliding
    //!
    PostCrashDynamic(double velocity = 0,
                     double velocityChange = 0,
                     double velocityDirection = 0,
                     double yawVelocity = 0,
                     Common::Vector2d pulse = Common::Vector2d(),
                     double pulseDirection = 0,
                     Common::Vector2d pulseLocal = Common::Vector2d(),
                     Common::Vector2d pointOfContactLocal = Common::Vector2d(),
                     double collisionVelocity = 0,
                     bool sliding = false)
        : velocity(velocity),
          velocityChange(velocityChange),
          velocityDirection(velocityDirection),
          yawVelocity(yawVelocity),
          pulse(pulse),
          pulseDirection(pulseDirection),
          pulseLocal(pulseLocal),
          pointOfContactLocal(pointOfContactLocal),
          collisionVelocity(collisionVelocity),
          sliding(sliding)
    {}
    virtual ~PostCrashDynamic() = default;

    double GetVelocity() const
    {
        return velocity;
    }

    void SetVelocity(double value)
    {
        velocity = value;
    }

    double GetVelocityChange() const
    {
        return velocityChange;
    }

    void SetVelocityChange(double value)
    {
        velocityChange = value;
    }

    double GetVelocityDirection() const
    {
        return velocityDirection;
    }

    void SetVelocityDirection(double value)
    {
        velocityDirection = value;
    }

    double GetYawVelocity() const
    {
        return yawVelocity;
    }

    void SetYawVelocity(double value)
    {
        yawVelocity = value;
    }

    Common::Vector2d GetPulse() const
    {
        return pulse;
    }

    void SetPulse(const Common::Vector2d &value)
    {
        pulse = value;
    }

    double GetPulseDirection() const
    {
        return pulseDirection;
    }

    void SetPulseDirection(double value)
    {
        pulseDirection = value;
    }

    Common::Vector2d GetPulseLocal() const
    {
        return pulseLocal;
    }

    void SetPulseLocal(Common::Vector2d value)
    {
        pulseLocal = value;
    }

    Common::Vector2d GetPointOfContactLocal() const
    {
        return pointOfContactLocal;
    }

    void SetPointOfContactLocal(const Common::Vector2d &value)
    {
        pointOfContactLocal = value;
    }

    double GetCollisionVelocity() const
    {
        return collisionVelocity;
    }

    void SetCollisionVelocity(double value)
    {
        collisionVelocity = value;
    }

    bool GetSliding() const
    {
        return sliding;
    }

    void SetSliding(bool value)
    {
        sliding = value;
    }

private:
    //! post crash velocity, absolute [m/s]
    double velocity = 0;
    //! delta-V: collision induced velocity change [m/s]
    double velocityChange = 0;
    //! post crash velocity direction [rad]
    double velocityDirection = 0;
    //! post crash yaw velocity [rad/s]
    double yawVelocity = 0;
    //! pulse vector [two-dimensional vector: kg*m/s,kg*m/s]
    Common::Vector2d pulse = Common::Vector2d(0, 0);
    //! pulse direction [rad]
    double pulseDirection = 0;
    //! crash pulse in local vehicle coordinate system
    Common::Vector2d pulseLocal = Common::Vector2d(0, 0);
    //! point of impact in local vehicle coordinate system
    Common::Vector2d pointOfContactLocal = Common::Vector2d(0, 0);
    //! collision velocity
    double collisionVelocity = 0;
    /*!
     * \brief flag for collision type
     *
     * true: sliding collision, false: no sliding collision
     *
     * Note that the collision model is not valid for sliding collisions.
     */
    bool sliding = false;
};

#endif // POSTCRASHDYNAMIC_H
