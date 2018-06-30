/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef VEHICLESIMPLETT_H
#define VEHICLESIMPLETT_H

#include <QtGlobal>
#include "vector2d.h"
#include "dynamics_twotrack_tire.h"
#include <vector>
#define NUMBER_TIRES 4

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
    void InitSetGeometry(double h_COG, double x_COG,
                         double y_wheelbase, double y_track);
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
    void SetVelocity(const double v_abs, const double slideAngle, const double w);
    /**
     *    @}
    */

    /**
     *    \name Make step
     *    @{
    */
    //! Calculate local tire torques
    void DriveTrain(double throttlePedal, double brakePedal);
    //! Local forces and moments transferred onto road
    void ForceLocal(double timeStep, double);
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

private:

    /** \name Parameters
     *    @{
    */
    //! Inertial moment of tires [kg*m^2]
    double inertiaTireX[NUMBER_TIRES];

    //! Maximal engine power [W]
    double powerEngineMax;
    //! Minimal brake force [N]
    double torqueBrakeMin;

    //! Mass of the car [kg]
    double massTotal;
    //! Tire positions in car CS [m]
    Common::Vector2d positionTire[NUMBER_TIRES];
    //! Inertial moment of car around Z [kg*m^2]
    //double inertiaCarZ;
    //! Vertical position of the center of mass of the car [m]
    double heightCOG;
    //! Wheelbase [m]
    double wheelBase;
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
    /**
     *  @}
    */

    // Dynamics to remember
    double rotationVelocityTireX[NUMBER_TIRES];
    double rotationVelocityGradTireX[NUMBER_TIRES];
    double angleSlide;
    double yawVelocity;
    double forceTireVerticalStatic[NUMBER_TIRES];
    Common::Vector2d velocityCar;
    Common::Vector2d forceTire[NUMBER_TIRES];
    Common::Vector2d slipTire[NUMBER_TIRES];
    double torqueTireX[NUMBER_TIRES];
    double momentTireZ[NUMBER_TIRES];

    /** \name Container
     *    @{
    */
    std::vector<Tire *> tires;
    /**
     *  @}
    */

};

#endif // VEHICLESIMPLETT_H
