/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  pcm_trajectory.h
//! @brief This file contains class for a trajectory that might be exchanged
//!        between modules and its signal class.
//-----------------------------------------------------------------------------
#ifndef PCM_TRAJECTORY_H
#define PCM_TRAJECTORY_H

#include <vector>
#include <sstream>
#include <QString>

//-----------------------------------------------------------------------------
//! Class for a trajectory that might be exchanged between modules
//-----------------------------------------------------------------------------
class PCM_Trajectory
{
public:
    PCM_Trajectory() = default;
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     timeVec      The vector of the time of the trajectory points
    //! @param[in]     xPosVec      The vector of the x-coordinates of the trajectory points
    //! @param[in]     yPosVec      The vector of the y-coordinates of the trajectory points
    //! @param[in]     uVelVec      The vector of the velocities of the trajectory points (forward)
    //! @param[in]     vVelVec      The vector of the velocities of the trajectory points (sideward)
    //! @param[in]     psiVec       The vector of the yaw angles of the trajectory points
    //-----------------------------------------------------------------------------
    PCM_Trajectory( const std::vector<int> *timeVec,
                    const std::vector<double> *xPosVec,
                    const std::vector<double> *yPosVec,
                    const std::vector<double> *uVelVec,
                    const std::vector<double> *vVelVec,
                    const std::vector<double> *psiVec):
        timeVec(timeVec),
        xPosVec(xPosVec),
        yPosVec(yPosVec),
        uVelVec(uVelVec),
        vVelVec(vVelVec),
        psiVec(psiVec)
    {}

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_Trajectory() = default;

    //-----------------------------------------------------------------------------
    //! Getter function
    //! @return     the end time of the trajectory = last entry of the time vector
    //-----------------------------------------------------------------------------
    int GetEndTime();

    /*!
     * \name
     *
     * Standard getter functions
     * @return  The vector with the time/x-coordinates/y-coordinates/velocities/yaw angles of the trajectory points
     * @{
     *
     */
    const std::vector<int> *GetTimeVec() const;
    const std::vector<double> *GetXPosVec() const;
    const std::vector<double> *GetYPosVec() const;
    const std::vector<double> *GetUVelVec() const;
    const std::vector<double> *GetVVelVec() const;
    const std::vector<double> *GetPsiVec() const;

    QString GetTimeVecString() const;
    QString GetXPosVecString() const;
    QString GetYPosVecString() const;
    QString GetUVelVecString() const;
    QString GetVVelVecString() const;
    QString GetPsiVecString() const;
    /**
     *  @}
     */

    //-----------------------------------------------------------------------------
    //! Function transforming a vector of ints to one QString where the vector
    //! values are comma separated
    //!
    //! @param[in]     inVec    vector of int to be transformed into a comma separated list
    //! @return                 QString with the comma separated list
    //-----------------------------------------------------------------------------
    QString VecIntToCsv(const std::vector<int> *inVec) const;

    //-----------------------------------------------------------------------------
    //! Function transforming a vector of doubles to one QString where the vector
    //! values are comma separated
    //!
    //! @param[in]     inVec    vector of int to be transformed into a comma separated list
    //! @return                 QString with the comma separated list
    //-----------------------------------------------------------------------------
    QString VecDoubleToCsv(const std::vector<double> *inVec) const;

    //! Clear all trajectory data.
    void Clear();

private:
    const std::vector<int> *timeVec = nullptr; //!< vector of the time of the trajectory points
    const std::vector<double> *xPosVec =
        nullptr; //!< vector of the x-coordinates of the trajectory points
    const std::vector<double> *yPosVec =
        nullptr; //!< vector of the y-coordinates of the trajectory points
    const std::vector<double> *uVelVec =
        nullptr; //!< vector of the velocities of the trajectory points (forward)
    const std::vector<double> *vVelVec =
        nullptr; //!< vector of the velocities of the trajectory points (sideward)
    const std::vector<double> *psiVec = nullptr; //!< vector of the yaw angles of the trajectory points

};

#endif // PCM_TRAJECTORY_H
