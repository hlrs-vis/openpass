/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef CARINFO
#define CARINFO

#include <sstream>
#include "vector2d.h"
#include "commonTools.h"

//-----------------------------------------------------------------------------
//! @brief Containing information on a vehicle
//!
//! This class collects observed and calculated values of another vehicle.
//! Additionally, it can calculate additional information.
//! The described vehicle is refered to as "it".
//-----------------------------------------------------------------------------
class CarInfo
{
public:
    //-----------------------------------------------------------------------------
    //! @brief Default constructor setting only existence to false
    //-----------------------------------------------------------------------------
    CarInfo():
        existent(false)
    {}

    virtual ~CarInfo() = default;

    //-----------------------------------------------------------------------------
    //! @brief Full constructor requiring all external information on a vehicle
    //!
    //! @param [in]   s                 its (recognised) distance from start of the road
    //! @param [in]   w                 its (recognised) lateral displacement (i.e. distance to its lane mid, left of mid is positive)
    //! @param [in]   v                 its (recognised) velocity
    //! @param [in]   scale             vector containing the information on how to split velocity acting in road direction and perpendicular to it
    //! @param [in]   aRecognised       states whether the acceleration has been recognised
    //! @param [in]   a                 its (recognised) acceleration
    //! @param [in]   yaw               its (recognised) yaw angle (global coordinate system)
    //! @param [in]   relativeYaw       its (recognised) relative yaw angle (relative to its road/lane)
    //! @param [in]   carwidth          its (recognised) width
    //! @param [in]   isBrakeLightOn    states whether its brake lights are (recognised to be) on
    //! @param [in]   agentID           the ID of the agent
    CarInfo(
        double s,
        double w,
        double v,
        Common::Vector2d scale,
        bool aRecognised,
        double a,
        double yaw,
        double relativeYaw,
        double carwidth,
        bool isBrakeLightOn,
        int agentID):
        existent(true),
        dist(s),
        w(w),
        v(v),
        vs(scale.x * v),
        vw(scale.y * v),
        yaw(yaw),
        relativeYaw(relativeYaw),
        carwidth(carwidth),
        isBrakeLightOn(isBrakeLightOn),
        aRecognised(aRecognised),
        agentID(agentID),
        _a(a)
    {}

    //-----------------------------------------------------------------------------
    //! @brief Converts signal to string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::ostringstream str;
        str << "s: " << dist <<
            "w: " << w <<
            "v: " << v <<
            "vs: " << vs <<
            "vw: " << vw <<
            "a: " << _a <<
            "yaw: " << yaw <<
            "lb: " << lb <<
            "carwidth: " << carwidth;
        return str.str();
    }

    //-----------------------------------------------------------------------------
    //! @brief Compares it to another vehicle
    //!
    //! @param [in]   other     the CarInfo on the other vehicle
    //!
    //! @return true if the two CarInfos are equal
    //-----------------------------------------------------------------------------
    bool Equals(const CarInfo &other) const
    {
        return dist == other.dist &&
               w == other.w &&
               v == other.v &&
               vs == other.vs &&
               vw == other.vw &&
               _a == other._a &&
               yaw == other.yaw;
    }

    bool existent;              //! is it seen? -> is the class's content valid?

    double dist =
        0;            //! its signed distance to the agent instantiating this class, <0 means that it is behind the agent
    double w = 0;               //! its lateral displacement (to its lane mid)
    double v = 0;               //! its velocity
    double vs = 0;              //! its part-velocity acting in road direction
    double vw = 0;              //! its part-velocity acting perpendicular to the road direction
    double yaw = 0;             //! its (global) yaw angle
    double relativeYaw = 0;     //! its (recognised) relative yaw angle (relative to its road/lane)
    double lb = 2.97 +
                1.14;    //! its distance front axle (=reference point) to rear edge - Java: VehicleXYTransformLib.l + VehicleXYTransformLib.b
    double carwidth = 2.1;      //! its width - Java: Parameters.carwidth
    bool isBrakeLightOn = false;//! state of its brake light
    bool aRecognised = false;   //! whether its acceleration has been recognised ot not
    int agentID = -2000;        //! agent ID

    //-----------------------------------------------------------------------------
    //! @brief Getter for its acceleration considering aRecognised
    //!
    //! If aRecognised==false, a runtime_error is thrown.
    //!
    //! @return its acceleration
    //-----------------------------------------------------------------------------
    double Acceleration()
    {
        AssertRecognised();
        return _a;
    }

    //-----------------------------------------------------------------------------
    //! @brief Getter for its time to collision
    //!
    //! The time to collision is with respect to the agent instantiating this class
    //! and can't be calculated only by the values in this class - therefore throws
    //! a runtime_error, if this value hasn't been calculated before using the
    //! function CalculateTtc. Default in case of non existence is 99.
    //!
    //! @return its ttc with respect to the agent instantiating this class
    //-----------------------------------------------------------------------------
    double Ttc()
    {
        if (!existent) {
            return 99.0;
        }

        AssertCalculation();
        return _ttc;
    }

    //-----------------------------------------------------------------------------
    //! @brief Getter for its net distance
    //!
    //! The net distance is with respect to the agent instantiating this class
    //! and can't be calculated only by the values in this class - therefore throws
    //! a runtime_error, if this value hasn't been calculated before using the
    //! function CalculateTtc. Default in case of non existence is 99.
    //!
    //! @return its ttc with respect to the agent instantiating this class
    //-----------------------------------------------------------------------------
    double NetDistance()
    {
        if (!existent) {
            return 99.0;
        }

        AssertCalculation();
        return _netDist;
    }

    //-----------------------------------------------------------------------------
    //! @brief Calculates the time to collision (and net distance)
    //!
    //! Only after calling this function, the time to collision (and net distance)
    //! can be obtained with the apropiate getter method.
    //-----------------------------------------------------------------------------
    void CalculateTtc(
        double vOther,
        double carLengthEffective)
    {
        if (!existent) {
            return;
        }

        if (dist > 0) {
            _netDist = dist - carLengthEffective;
            _ttc = TrafficHelperFunctions::CalculateNetTTC(vOther, v, _netDist);
        } else {
            _netDist = -dist - carLengthEffective;
            _ttc = TrafficHelperFunctions::CalculateNetTTC(v, vOther, _netDist);
        }

        _isCalculated = true;
    }

private:
    void AssertCalculation()
    {
        if (!_isCalculated) {
            throw std::runtime_error("CarInfo has not been calculated yet.");
        }
    }

    void AssertRecognised()
    {
        if (!aRecognised) {
            throw std::runtime_error("Acceleration has not been recognised.");
        }
    }

    bool _isCalculated = false;
    double _netDist = -1;
    double _a = -1;
    double _ttc = -1;
};

//-----------------------------------------------------------------------------
//! @brief Signal simply containing a CarInfo
//-----------------------------------------------------------------------------
class CarInfoSignal : public SignalInterface
{
public:
    //-----------------------------------------------------------------------------
    //! @brief standard constructor to fill the content of the signal
    //-----------------------------------------------------------------------------
    CarInfoSignal(CarInfo inValue) : value(inValue)
    {}
    CarInfoSignal(const CarInfoSignal &) = default;
    CarInfoSignal(CarInfoSignal &&) = default;
    CarInfoSignal &operator=(const CarInfoSignal &) = default;
    CarInfoSignal &operator=(CarInfoSignal &&) = default;
    virtual ~CarInfoSignal() = default;

    //-----------------------------------------------------------------------------
    //! @brief Converts signal to string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        return value;
    }

    CarInfo value; //!< signal content
};

//-----------------------------------------------------------------------------
//! @brief Signal simply containing a std::vector of CarInfos
//-----------------------------------------------------------------------------
class CarInfoListSignal : public SignalInterface
{
public:
    //-----------------------------------------------------------------------------
    //! @brief standard constructor to fill the content of the signal
    //-----------------------------------------------------------------------------
    CarInfoListSignal(std::vector<CarInfo> inValue) : value(inValue)
    {}
    CarInfoListSignal(const CarInfoListSignal &) = default;
    CarInfoListSignal(CarInfoListSignal &&) = default;
    CarInfoListSignal &operator=(const CarInfoListSignal &) = default;
    CarInfoListSignal &operator=(CarInfoListSignal &&) = default;
    virtual ~CarInfoListSignal() = default;

    //-----------------------------------------------------------------------------
    //! @brief Converts signal to string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::ostringstream msg;
        for (unsigned int ii = 0; ii < value.size(); ii++) {
            std::string str = value[ii];
            msg << "CarInfo " << ii << ": " << str << "\n";
        }

        return msg.str();
    }

    std::vector<CarInfo> value; //!< signal content
};

#endif // CARINFO
