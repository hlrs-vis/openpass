/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/primitiveSignals.h"
#include "Common/dynamicsSignal.h"
#include "globalDefinitions.h"


/** \addtogroup Dynamics_RegularDriving
* @{
* \brief models the longitudinal and lateral dynamics of the vehicle coresponding to the pedal and steering inputs
*
* This component models the reaction of the vehicle based on the pedal and steering wheel operation of the driver.
* It calculates velocity and position of the agent according to his acceleration and his collision state.
* If no collision took place in the previous time step where the agent was
* the rear opponent, the velocity and position of the agent is calculated
* according to his acceleration wish.
* If the agent was the rear opponent in a collision, its velocity is set to
* the one of the front agent. The position is calculated according to this
* velocity. Additionaly the maximum change of steering wheel angle according to the
* desired steering wheel angle is calculated.
*
* \section Dynamics_RegularDriving_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* in_accPedalPos        | Position of the gas pedal in percent
* in_brakePedalPos      | Position of the brake pedal in percent
* in_gear               | Current gear
* in_steeringWheelAngle | Steering wheel angle of the agent
*
*
* Input channel IDs:
* Input ID  | signal class                              | contained variables
* ----------|-------------------------------------------|-------------
* 1         | LongitudinalSignal                        | in_accPedalPos, in_brakePedalPos, in_gear
* 2         | SteeringSignal                            | in_steeringWheelAngle
*
*
** \section Dynamics_RegularDriving_InitInputs Init Inputs
* Init input variables:
* name          | meaning
* --------------|------
* vehicleModelParameters | VehicleModelParameters
*
*
* * Init input channel IDs:
* Input Id      | signal class                | contained variables
* --------------|-----------------------------|------------
* 100           | ParametersVehicleSignal     | vehicleModelParameters
*
*
*
* * \section Dynamics_RegularDriving_Outputs Outputs
* Output variables:
* name                       | meaning
* ---------------------------|------
* acceleration               | Acceleration of the current agent coresponding to the pedalpositions [m/s²]
* velocity                   | The driven curvature of the car coresponding to the steering wheel angle based on Ackermann [radiant]
* positionX                  | new inertial x-position [m]
* positionY                  | new inertial y-position [m]
* yaw                        | new yaw angle in current time step [radiant]
* yawRate                    | change of yaw angle due to ds and curvature [radiant]
* steeringWheelAngle         | new angle of the steering wheel angle [degree]
* travelDistance             | distance traveled by the agent during this timestep [m]
*
*
* Output channel IDs:
* Output Id                  | signal class    | contained variables
* ---------------------------|-----------------|-------------
* 0                          | DynamicsSignal  | acceleration, velocity, positionX, positionY, yaw, yawRate, steeringWheelAngle, travelDistance
*

* \section Dynamics_RegularDriving_ExternalParameters External parameters
* none
*
* \section Dynamics_RegularDriving_InternalParameters Internal paramters
* name                  | value  | meaning
* ----------------------|--------|-------------
* VLowerLimit           |      0 | The minimal velocity of the agent in m/s.
*
* @} */

//! \ingroup Dynamics_RegularDriving
class DynamicsRegularDrivingImplementation : public DynamicsInterface
{
public:

    //! Name of the current component
    const std::string COMPONENTNAME = "DynamicRegularDriving";

    DynamicsRegularDrivingImplementation(
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
        AgentInterface *agent) :
        DynamicsInterface(
            componentName,
            isInit,
            priority,
            offsetTime,
            responseTime,
            cycleTime,
            stochastics,
            world,
            parameters,
            observations,
            callbacks,
            agent),
        dynamicsSignal {ComponentState::Acting}
    {
        try
        {
            this->observation = observations->at(0);
        }
        catch (const std::out_of_range&)
        {
            std::string msg = "DynamicsRegularDriving requires an Observer";
            LOG(CbkLogLevel::Error, msg);
            throw std::out_of_range(msg);
        }
    }
    DynamicsRegularDrivingImplementation(const DynamicsRegularDrivingImplementation&) = delete;
    DynamicsRegularDrivingImplementation(DynamicsRegularDrivingImplementation&&) = delete;
    DynamicsRegularDrivingImplementation& operator=(const DynamicsRegularDrivingImplementation&) = delete;
    DynamicsRegularDrivingImplementation& operator=(DynamicsRegularDrivingImplementation&&) = delete;
    virtual ~DynamicsRegularDrivingImplementation() = default;

    /*!
    * \brief Update Inputs
    *
    * Function is called by framework when another component delivers a signal over
    * a channel to this component (scheduler calls update taks of other component).
    *
    * Refer to module description for input channels and input ids.
    *
    * @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    * @param[in]     data           Referenced signal (copied by sending component)
    * @param[in]     time           Current scheduling time
    */
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

    /*!
    * \brief Update outputs.
    *
    * Function is called by framework when this component has to deliver a signal over
    * a channel to another component (scheduler calls update task of this component).
    *
    * Refer to module description for output channels and output ids.
    *
    * @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    * @param[out]    data           Referenced signal (copied by this component)
    * @param[in]     time           Current scheduling time
    */
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

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

    //! Applies limit to incoming geer
    void ApplyGearLimit();

    //! Applies limits to the incoming steering wheel angle and incoming pedal position
    void ApplyPedalPositionLimits();

    //! Get the acceleration of the vehicle from pedal position and gear.
    //! @param [in] accPedalPos         current Gas pedal position [%]
    //! @param [in] brakePedalPos       current Brake pedal position [%]
    //! @param [in] gear                current Gear
    //! @param [in] carParameters       parameters of the car
    //! @return current acceleration
    double GetAccVehicle(double accPedalPos, double brakePedalPos, int gear, int time);


    //! Get the moment of the engine from pedal position and gear.
    //! @param [in] gasPedalPos         current Gas pedal position. [%]
    //! @param [in] lastGear            the last chosen gear.
    //! @param [in] carParameters       parameters of the car
    //! @return engine moment [Nm]
    //-----------------------------------------------------------------------------
    double GetEngineMoment(double gasPedalPos, int lastGear);



    //! Get the drag moment of the engine corresponding to its speed
    //! @param [in] engineSpeed         the speed of the engine
    //! @param [in] carParameters       parameters of the car
    //! @return drag moment [Nm]
    double GetEngineMomentMin(double engineSpeed);


    //! Get the maximum possible torque of the engine in the current state
    //! @param [in] engineSpeed         the speed of the engine
    //! @param [in] carParameters       parameters of the car
    //! @return maximum moment in the current state [Nm]
    double GetEngineMomentMax(double engineSpeed);


     //! Calculate the engine speed coresponding to the current speed and gear of the car
     //! @param [in] xVel               longitudinal speed of the car
     //! @param [in] gear               current gear
     //! @param [in] carParameters      parameters of the car
     //! @return engine speed [1/min]
    double GetEngineSpeedByVelocity(double xVel, int gear);


    //! Calculate the resulting acceleration of the car with the delivered moment of the engine
    //! @param [in] xVel               longitudinal speed of the car (unused)
    //! @param [in] engineMoment       delivered moment of the engine
    //! @param [in] chosenGear         the curent gear
    //! @param [in] carParameters      parameters of the car
    //! @param [in] cycleTime          Cycle time of this components trigger task [ms] (unused)
    //! @return resulting acceleration [m/s²]
    double GetAccFromEngineMoment(double xVel, double engineMoment, int chosenGear, int cycleTime);


    //! Returns the friction coefficient according to enviroment and car conditions
    //! @return friction coefficient (currently ALWAYS 1)
    double GetFrictionCoefficient();



    /** @name Private Variables
     * @{
     * */
        /** @name Internal Parameters
         * @{
         */

    //! pointer to observation at observations map element 0
    ObservationInterface* observation;

    //! The minimal velocity of the agent [m/s].
    const double VLowerLimit = 0;

    /**
     *  @} */ // End of Internal Parameters
    /** @name External Parameters
     * @{
     */

    // --- Inputs

    //! Position of the accecaleration pedal position [%].
    double in_accPedalPos = 0;

    //! Position of the brake pedal position [%].
    double in_brakePedalPos = 0;

    //! Number of gears and position of gear.
    int in_gear = 0;

    //! The steering wheel angle [°].
    double in_steeringWheelAngle = 0;

    // --- Outputs

    //! Output Signal containing (aLong,v,x,y,dpsi,psi)
    DynamicsSignal dynamicsSignal;

    // --- Init Inputs

    //! Containing the vehicle parameters e.g. double carMass; double rDyn and more.
    VehicleModelParameters vehicleModelParameters;

    // Constants

    //! Conversion factor degree to radiant.
    const double DegreeToRadiant = 0.01745329251994329576923690768489;

    //! PI.
    double _twoPi = 2 * M_PI;

    //! value of earth gravity [m/s²].
    double _oneG = 9.81;

    //! air density.
    double _rho = 1.23;
    /**
     *  @} */ // End of External Parameters
    /**
    *  @} */ // End of Private Variables
};

/** @} */ // End of group DynamicsRegularDriving
