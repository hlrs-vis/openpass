/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  Trajectory.h
//! @brief This file contains class for a trajectory that might be exchanged
//!        between modules and its signal class.
//-----------------------------------------------------------------------------
#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>
#include <sstream>

//-----------------------------------------------------------------------------
//! Class for a trajectory that might be exchanged between modules
//-----------------------------------------------------------------------------
class Trajectory
{
public:
    Trajectory() = default;
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     timeVec      The vector of the time of the trajectory points
    //! @param[in]     xPosVec      The vector of the x-coordinates of the trajectory points
    //! @param[in]     yPosVec      The vector of the y-coordinates of the trajectory points
    //! @param[in]     velVec       The vector of the velocities of the trajectory points
    //! @param[in]     psiVec       The vector of the yaw angles of the trajectory points
    //-----------------------------------------------------------------------------
    Trajectory( const std::vector<int> *timeVec,
                const std::vector<double> *xPosVec,
                const std::vector<double> *yPosVec,
                const std::vector<double> *velVec,
                const std::vector<double> *psiVec):
        _timeVec(timeVec),
        _xPosVec(xPosVec),
        _yPosVec(yPosVec),
        _velVec(velVec),
        _psiVec(psiVec)
    {}

    /*!
     * \name
     *
     * Standard getter functions
     * @return  The vector with the time/x-coordinates/y-coordinates/velocities/yaw angles of the trajectory points
     * @{
     *
     */
    const std::vector<int> *GetTimeVec() const
    {
        return _timeVec;
    }
    const std::vector<double> *GetXPosVec() const
    {
        return  _xPosVec;
    }
    const std::vector<double> *GetYPosVec() const
    {
        return  _yPosVec;
    }
    const std::vector<double> *GetVelVec() const
    {
        return  _velVec;
    }
    const std::vector<double> *GetPsiVec() const
    {
        return  _psiVec;
    }
    /**
     *  @}
     */

    //! Clear all trajectory data.
    void Clear()
    {
        if (_timeVec != nullptr)
        {
            delete _timeVec ;
            _timeVec = nullptr;
        }
        if (_xPosVec != nullptr)
        {
            delete _xPosVec ;
            _xPosVec = nullptr;
        }
        if (_yPosVec != nullptr)
        {
            delete _yPosVec ;
            _yPosVec = nullptr;
        }
        if (_velVec != nullptr)
        {
            delete _velVec ;
            _velVec = nullptr;
        }
        if (_psiVec != nullptr)
        {
            delete _psiVec ;
            _psiVec = nullptr;
        }
    }

private:
    const std::vector<int> *_timeVec = nullptr;      //!< vector of the time of the trajectory points
    const std::vector<double> *_xPosVec =
        nullptr;   //!< vector of the x-coordinates of the trajectory points
    const std::vector<double> *_yPosVec =
        nullptr;   //!< vector of the y-coordinates of the trajectory points
    const std::vector<double> *_velVec =
        nullptr;    //!< vector of the velocities of the trajectory points
    const std::vector<double> *_psiVec =
        nullptr;    //!< vector of the yaw angles of the trajectory points

};

#endif // TRAJECTORY_H
