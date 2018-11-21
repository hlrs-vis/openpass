/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//! @file  pid_controller.h
//! @brief This file contains the definition of a PID-controller.

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <iostream>

//! class containing a PID-controller
class PIDController
{
public:

    //! Standard constructor
    PIDController():
        P_(1), I_(0), D_(0),
        integratedError_(0), lastError_(0), integratorMin_(-1e9), integratorMax_(1e9)
    {}

    //! Constructor
    //!
    //! @param[in]     P      the porportional gain
    //! @param[in]     I      the integral gain
    //! @param[in]     D      the derivative gain
    PIDController(double P, double I, double D):
        P_(P), I_(I), D_(D),
        integratedError_(0), lastError_(0), integratorMin_(-1e9), integratorMax_(1e9)
    {}

    //! Sophisticated Constructor, that sets also some additional internal stati
    //!
    //! @param[in]     P                    the porportional gain
    //! @param[in]     I                    the integral gain
    //! @param[in]     D                    the derivative gain
    //! @param[in]     integratedErrorMin   the minimal value for the integral term
    //! @param[in]     integratedErrorMax   the maximum value for the integral term
    PIDController(double P, double I, double D, double integratedErrorMin,
                  double integratedErrorMax):
        P_(P), I_(I), D_(D),
        integratedError_(0), lastError_(0),
        integratorMin_(integratedErrorMin), integratorMax_(integratedErrorMax)
    {}
    //! Sophisticated Constructor, that sets all additional internal stati
    //!
    //! @param[in]     P                            the porportional gain
    //! @param[in]     I                            the integral gain
    //! @param[in]     D                            the derivative gain
    //! @param[in]     integratedErrorMin           the minimal value for the integral term
    //! @param[in]     integratedErrorMax           the maximum value for the integral term
    //! @param[in]     ignoreIntegratorThreshold_   a threshold for ignoring the integral term
    PIDController(double P, double I, double D, double integratedErrorMin,
                  double integratedErrorMax, double ignoreIntegratorThreshold):
        P_(P), I_(I), D_(D),
        integratedError_(0), lastError_(0),
        integratorMin_(integratedErrorMin), integratorMax_(integratedErrorMax),
        ignoreIntegratorThreshold_(ignoreIntegratorThreshold)
    {}

    //! Function setting the integrated error to 0
    inline void resetIntegratedError()
    {
        integratedError_ = 0;
    }

    //! Function setting the last error to 0
    inline void resetLastError()
    {
        lastError_ = 0;
    }

    //! Function setting the proportional gain
    //!
    //! @param[in]     value        the new porportional gain
    void setP(double value)
    {
        P_ = value;
    }

    //! Function setting the intagral gain
    //!
    //! @param[in]     value        the new integral gain
    void setI(double value)
    {
        I_ = value;
    }

    //! Function setting the derivative gain
    //!
    //! @param[in]     value        the new derivative gain
    void setD(double value)
    {
        D_ = value;
    }

    //! Function giving out gains on the standard display
    //!
    //! @param[in]     value        the new derivative gain
    void printCoefficients()
    {
        std::cout << "P = " << P_ << " I = " << I_ << " D = " << D_ << std::endl;
    }

    //! Function calculating the current control
    //!
    //! @param[in]     currentError     the current error
    //! @param[in]     delta_T          the time step since the last call
    //! @return                         the current control
    double calculate(double currentError, double delta_T)
    {
        integratedError_ += currentError;
        if (integratedError_ > integratorMax_) {
            integratedError_ = integratorMax_;
        } else if (integratedError_ < integratorMin_) {
            integratedError_ = integratorMin_;
        }

        //estimate error slope
        double differentialError = (currentError - lastError_) / delta_T;
        lastError_ = currentError;
        if (currentError >= ignoreIntegratorThreshold_) {
            return P_ * currentError + I_ * integratedError_ + D_ * differentialError;
        } else {
            return P_ * currentError + D_ *
                   differentialError;    //ignore the integrated error when the error is too low
        }
    }

private:
    double P_;
    double I_;
    double D_;

    double integratedError_;
    double lastError_;
    double integratorMin_;
    double integratorMax_;
    double ignoreIntegratorThreshold_;
};


#endif // PID_CONTROLLER_H
