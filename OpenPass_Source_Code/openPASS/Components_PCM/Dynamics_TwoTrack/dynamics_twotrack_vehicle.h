/*******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*               2017, 2018, 2019 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#ifndef VEHICLESIMPLETT_H
#define VEHICLESIMPLETT_H

#include <QtGlobal>
#include <vector>
#include "vector2d.h"
#include "dynamics_twotrack_tire.h"
#define NUMBER_OF_WHEELS 4

class Tire;

//! Simple STATIC two-track vehicle model
class VehicleSimpleTT
{
public:
    VehicleSimpleTT();
    ~VehicleSimpleTT();

    /**
     *    \name Initialize
     *    @{
    */
    //! Initialize tire characteristics
    void InitSetEngine(double weight,
                       double P_engine, double T_brakeLimit);
    //! Initialize car's physics
    void InitSetGeometry(double x_wheelbase, double x_COG, double y_track, double y_COG);
    //! Initialize car's velocity
    void InitSetTire(double vel,
                     double F_max, double F_slide,
                     double s_max, double r_tire, double frictionScaleRoll);
    /**
     *    @}
    */

    /**
     *    \name Update state
     *    @{
    */
    //! Refresh car's position
    void UpdatePosition(double);
    //! Refresh car's velocity
    void SetVelocity(Common::Vector2d, const double);
    /**
     *    @}
    */

    /**
     *    \name Make step
     *    @{
    */
    //! Calculate local tire torques
    void DriveTrain(double throttlePedal, double brakePedal, std::vector<double> brakeSuperpose);
    //! Local forces and moments transferred onto road
    void ForceLocal(double timeStep, double, std::vector<double> forceVertical);
    //! Global force and moment
    void ForceGlobal();
    /**
     *    @}
    */

    /**
     *    \name Output
     *    @{
    */
    //! Total force on vehicle's CoM
    Common::Vector2d forceTotalXY;
    //! Total momentum on the vehicle around the z-axes
    double momentTotalZ;
    /**
     *    @}
    */

    /**
     *    \name Parameters
     *    @{
    */
    double forceTireVerticalStatic[NUMBER_OF_WHEELS];
    /**
     *    @}
    */

private:

    /** \name Parameters
     *    @{
    */
    //! Inertial moment of tires [kg*m^2]
    double inertiaTireX[NUMBER_OF_WHEELS];

    //! Maximal engine power [W]
    double powerEngineLimit;
    //! Brake force limit [N]
    double torqueBrakeLimit;

    //! Mass of the car [kg]
    double massTotal;
    //! Tire positions in car CS [m]
    Common::Vector2d positionTire[NUMBER_OF_WHEELS];
    /**
     *  @}
    */


    /** \name Constants
     *    @{
    */
    //! Drag coefficient (Asbo from http://rc.opelgt.org/indexcw.php) []
    const double coeffDrag = 0.34;
    //! Face area (Asbo from http://rc.opelgt.org/indexcw.php) [m^2]
    const double areaFace = 1.94;
    //! Air density [kg/m^3]
    const double densityAir = 1.29;
    //! Earth's gravitation acceleration
    const double accelVerticalEarth = -9.81;
    //! Toe-in/-out
    const double anglePreSet = 0.0;//0.003;
    //! Brake balance
    const double brakeBalance = 0.67;
    //! Max. engine moment
    const double torqueEngineLimit = 10000.0;
    /**
     *  @}
    */

    // Dynamics to remember
    double rotationVelocityTireX[NUMBER_OF_WHEELS];
    double rotationVelocityGradTireX[NUMBER_OF_WHEELS];
    double yawVelocity;
    Common::Vector2d velocityCar;
    Common::Vector2d forceTire[NUMBER_OF_WHEELS];
    Common::Vector2d slipTire[NUMBER_OF_WHEELS];
    double torqueTireXthrottle[NUMBER_OF_WHEELS];
    double torqueTireXbrake[NUMBER_OF_WHEELS];
    double momentTireZ[NUMBER_OF_WHEELS];

    /** \name Container
     *    @{
    */
    std::vector<Tire *> tires;
    /**
     *  @}
    */

};

#endif // VEHICLESIMPLETT_H
