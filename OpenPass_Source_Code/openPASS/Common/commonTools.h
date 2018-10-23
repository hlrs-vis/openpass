/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef COMMONTOOLS
#define COMMONTOOLS

#include "globalDefinitions.h"
#include "math.h"

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
};

#endif // COMMONTOOLS
