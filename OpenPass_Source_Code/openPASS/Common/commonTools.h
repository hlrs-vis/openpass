/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*               2016, 2017, 2018, 2019 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#ifndef COMMONTOOLS
#define COMMONTOOLS

#include "globalDefinitions.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include "math.h"
#include "vector2d.h"

//-----------------------------------------------------------------------------
//! @brief defines common helper functions like conversion from and to enums.
//-----------------------------------------------------------------------------
class CommonHelper
{
public:
    static double ConvertagentViewDirectionToRadian(AgentViewDirection agentViewDirection)
    {
        double viewDirection = INFINITY;

        switch (agentViewDirection)
        {
        case AgentViewDirection::front:
            viewDirection = 0;
            break;
        case AgentViewDirection::left:
            viewDirection = M_PI_2;
            break;
        case AgentViewDirection::back:
            viewDirection = M_PI;
            break;
        case AgentViewDirection::right:
            viewDirection = -M_PI_2;
            break;
        default:
            break;
        }
        return viewDirection;
    }

    static AgentViewDirection ConvertRadianToAgentViewDirection(double radiant)
    {
        if ((radiant <= M_PI_4) && (radiant >= -M_PI_4))
        {
            return AgentViewDirection::front;
        }
        else if ((radiant > M_PI_4) && (radiant <= 3 * M_PI_4))
        {
            return AgentViewDirection::left;
        }
        else if ((radiant < -M_PI_4) && (radiant >= -3 * M_PI))
        {
            return AgentViewDirection::right;
        }
        else if (((radiant < -3 * M_PI_4) && (radiant >= -M_PI))
                 || ((radiant > 3 * M_PI_4) && (radiant <= M_PI)))
        {
            return AgentViewDirection::back;
        }
        else
        {
            return AgentViewDirection::none;
        }
    }

    static double ConvertRadiantToDegree(double radian)
    {
        double degree = radian * 180 / M_PI;
        return degree;
    }

    static double ConvertDegreeToRadian(double degree)
    {
        double radian = degree / 180 * M_PI;
        return radian;
    }

    static double ConvertAngleToPi(double angle)
    {
        double out_angle = angle;
        if (fabs(out_angle) > M_PI)
        {
            if (out_angle > 0)
            {
                out_angle -= 2 * M_PI;
            }
            else
            {
                out_angle += 2 * M_PI;
            }
        }

        return out_angle;
    }

    // check if an angle is in the range of [angleRight, angleLeft]. All 3 inputs should be in the range of [-PI, PI]
    static bool Angle_In_Range(double angel, double angleRight, double angleLeft)
    {
      if (angleRight <= angleLeft)
      {
          return ((angel >= angleRight) && (angel <= angleLeft));
      }
      else // angleRight > angleLeft, meaning that the range passes through the line of PI/-PI
      {
          return ((angel >= angleRight) || (angel <= angleLeft));
      }
    }

    static bool CheckPointValid(const Common::Vector2d *point)
    {
        return ((point != nullptr) && (point->x != INFINITY) && (point->y != INFINITY));
    }

    //! Calculate the absolute angle between two pcm points.
    //!
    //! @param[in]    firstPoint     firstPoint
    //! @param[in]    secondPoint    secondPoint
    //! @return       distance between two pcm points
    //-----------------------------------------------------------------------------
    static double CalcAngleBetweenPoints(const Common::Vector2d& firstPoint, const Common::Vector2d& secondPoint)
    {
        if ((!CheckPointValid(&firstPoint)) || (!CheckPointValid(&secondPoint)))
        {
            return INFINITY;
        }

        double angle = (secondPoint - firstPoint).Angle();

        return angle;
    }

    //! Transform a pcm point to a vector in the coordination system of a
    //! source point in a direction.
    //!
    //! @param[in]    point     point
    //! @return                 vector
    static Common::Vector2d TransformPointToSourcePointCoord(const Common::Vector2d *point,
                                                             const Common::Vector2d *sourcePoint,
                                                             double direction)
    {
        Common::Vector2d newPoint = *point; //(point->x, point->y);
        newPoint.Translate((*sourcePoint) * (-1));
        newPoint.Rotate(direction * (-1));

        return newPoint;
    }
};

//-----------------------------------------------------------------------------
//! @brief Containing general static functions to evaluate traffic situations
//-----------------------------------------------------------------------------
class TrafficHelperFunctions
{
public:
    //-----------------------------------------------------------------------------
    //! @brief Calculates the net time to collision between two (moving) objects
    //!
    //! Time to collision (TTC) is the time until two objects collide if their
    //! velocities remain unchanged.
    //!
    //! Net in this context means that the carlengths are already substracted from
    //! the caller, i.e. netDistance = |sDist| - c
    //!   where c REK-usually \in {carlengthEffective, carlength}
    //! More sophisticated would be c = dist(refPointRear, FrontEdgeRear) +
    //!  dist(refPointFront, RearEdgeFront) + MinGap
    //! where MinGap = 0 for true TTC, >0 for estimated TTC with a safety gap
    //!
    //! Default values of the function:
    //! -1 | netDistance < 0
    //! 99 | vRear - vFront < 1e-6
    //!
    //! Java: computeNetTTC
    //!
    //! @param [in]   vRear         velocity of the rear object
    //! @param [in]   vFront        velocity of the front object
    //! @param [in]   netDistance   distance between the objects
    //!
    //! @return time to collsion between the two objects
    //-----------------------------------------------------------------------------
    static double CalculateNetTTC(
        double vRear,
        double vFront,
        double netDistance)
    {
        if (netDistance < 0.0)
        {
            return -1.0;
        }

        double deltaV = vRear - vFront;
        return (deltaV < 1e-6) ? 99.0 : netDistance / deltaV;
    }

    //-----------------------------------------------------------------------------
    //! @brief Calculates the net time gap between two (moving) objects
    //!
    //! Time gap is the time until two objects collide if the front object stands
    //! still and the velocity of the rear object remains unchanged.
    //!
    //! Net in this context means that the carlengths are already substracted from
    //! the caller, i.e. netDistance = |sDist| - c
    //!   where c REK-usually \in {carlengthEffective, carlength}
    //! More sophisticated would be c = dist(refPointRear, FrontEdgeRear) +
    //!  dist(refPointFront, RearEdgeFront) + MinGap
    //! where MinGap = 0 for true TTC, >0 for estimated TTC with a safety gap
    //!
    //! Default values of the function:
    //! -1 | netDistance < 0
    //! 0  | netDistance = 0
    //! netDistance | vRear <= 1
    //!
    //! Java: computeNetGap
    //!
    //! @param [in]   vRear         velocity of the rear object
    //! @param [in]   netDistance   distance between the objects
    //!
    //! @return time gap between the two objects
    //-----------------------------------------------------------------------------
    static double CalculateNetTimeGap(
        double vRear,
        double netDistance)
    {
        if (netDistance < 0.0)
        {
            return -1.0;
        }

        return (netDistance <= 0.0) ? 0.0 : (vRear <= 1) ? netDistance : netDistance / vRear;
    }


    //-----------------------------------------------------------------------------
    //! @brief Limits a value between a lower and an upper limit
    //!
    //! Returns
    //! low | value < low
    //! value  | low <= value < high
    //! high | value >= high
    //!
    //! @param [in]   low       lower limit
    //! @param [in]   value     the value to be bounded
    //! @param [in]   high      higher limit
    //!
    //! @return bounded value
    //-----------------------------------------------------------------------------
    template <typename T>
    static T ValueInBounds(
        const T &low,
        const T &value,
        const T &high)
    {
        return (value < low) ? low : ((value < high) ? value : high);
    }

    //-----------------------------------------------------------------------------
    //! @brief Calculates if two agents will collide during braking
    //!
    //! /details    validates if cars crash during brake phase
    //!             if velocity and acceleration of agents will be the same.
    //!
    //! @param [in]   sDelta     distance between both agents
    //! @param [in]   vEgo       velocity ego
    //! @param [in]   aEgo       acceleration ego
    //! @param [in]   vFront     velocity front
    //! @param [in]   aFront     acceleration front
    //!
    //! @return true if collision would happen
    //-----------------------------------------------------------------------------
    static bool WillCrashDuringBrake(double sDelta, double vEgo, double aEgo, double vFront, double aFront)
    {
        //Calculate the intersection of the vehicles trajactory (quadratic functions)

        auto stopTime = - vEgo / aEgo;   //timepoint where ego stops
        bool frontStopsEarlier = false;
        double frontTravelDistance;

        if (aFront < 0)
        {
            auto stopTimeFront = - vFront / aFront;
            if (stopTimeFront < stopTime) //special case: FrontVehicle stops earlier than Ego
            {
                frontStopsEarlier = true;
                frontTravelDistance = - 0.5 * vFront * vFront / aFront;
            }
        }

        // acceleration equal -> difference of trajectories is linear
        if (aEgo == aFront)
        {

            // velocity is equal: distance constant -> no crash
            if(vEgo == vFront)
            {
                return false;
            }

            // intersection of lines
            auto intersectionTime = sDelta / (vEgo - vFront);
            if (0 <= intersectionTime && intersectionTime <= stopTime)
            {
                if (frontStopsEarlier)
                {
                    auto  travelDistance = - 0.5 * stopTime * stopTime * aEgo;
                    if (travelDistance < sDelta + frontTravelDistance)
                    {
                        return false;
                    }
                }

                return true;
            }
        }

        // intersection of quadratic functions
        auto discriminant = (vEgo - vFront) * (vEgo - vFront) + 2 * (aEgo - aFront) * sDelta;

        if (discriminant < 0)
        {
            return false;
        }


        auto intersectionPoint1 = ((vFront - vEgo) + std::sqrt(discriminant)) / (aEgo - aFront);
        auto intersectionPoint2 = ((vFront - vEgo) - std::sqrt(discriminant)) / (aEgo - aFront);

        if ((0 <= intersectionPoint1 && intersectionPoint1 <= stopTime) ||
            (0 <= intersectionPoint2 && intersectionPoint2 <= stopTime))
        {
            if (frontStopsEarlier)
            {
                auto travelDistance = - 0.5 * stopTime * stopTime * aEgo;
                if (travelDistance < sDelta + frontTravelDistance)
                {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------
    //! @brief Calculates if two agents will collide during braking
    //!
    //! \details validates if two cars crash within time to brake
    //!             if velocity and acceleration of agents will be the same.
    //!             Triggers WillCrashDuringBrake
    //!
    //! @param [in]   sDelta       distance between both agents
    //! @param [in]   vEgo         velocity rear agent (ego)
    //! @param [in]   aEgo         acceleration rear agent (ego)
    //! @param [in]   vFront       velocity front
    //! @param [in]   aFront       acceleratin front
    //! @param [in]   assumedTtb   assumed time to brake
    //!
    //! @return true if collision would happen
    //-----------------------------------------------------------------------------
    static bool WillCrash(double sDelta, double vEgo, double assumedBrakeAccelerationEgo, double vFront, double aFront, double assumedTtb)
    {
        auto sEgoAtTtb = vEgo * assumedTtb;

        // estimate values for front vehicle at t=ttb
        auto stopTimeFront = aFront < 0 ? -vFront/aFront : std::numeric_limits<double>::max();
        auto tFront = std::min(stopTimeFront, assumedTtb);
        auto sFrontAtTtb = sDelta + vFront*tFront + aFront*tFront*tFront/2;
        auto vFrontAtTtb = stopTimeFront < assumedTtb ? 0 : vFront + aFront*assumedTtb;
        auto aFrontAtTtb = stopTimeFront < assumedTtb ? 0 : aFront;

        if(sFrontAtTtb <= sEgoAtTtb) {
            return true;
        }

        return WillCrashDuringBrake(sFrontAtTtb - sEgoAtTtb, vEgo, assumedBrakeAccelerationEgo, vFrontAtTtb, aFrontAtTtb);
    }
};

#endif // COMMONTOOLS
