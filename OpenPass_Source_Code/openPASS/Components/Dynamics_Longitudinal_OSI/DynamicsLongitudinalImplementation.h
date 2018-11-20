/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

/** \addtogroup DynamicsLongitudinal
* @{
* \brief models the pedal operation of the driver and the reaction of the vehicle on this
*
* This component models the pedal operation of the driver and the reaction of
* the vehicle on this. It calculates velocity and position of
* the agent according to his acceleration and his collision state.
* If no collision took place in the previous time step where the agent was
* the rear opponent, the velocity and position of the agent is calculated
* according to his acceleration wish.
* If the agent was the rear opponent in a collision, its velocity is set to
* the one of the front agent. The position is calculated according to this
* velocity.
*
* \section DynamicsLongitudinal_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* in_carParameters | Several parameters of the current vehicle.
* in_aVehicle      | Acceleration of the current agent.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0 | ALongitudinalDriverToDLongitudinalSignal  | in_carParameters
* 1 | BrakingSignal                             | in_aVehicle
*
* \section DynamicsLongitudinal_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|-------------
* VLowerLimit | 0 | The minimal velocity of the agent in m/s.
*
* @} */

#pragma once

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "../Algorithm_LongitudinalDriver_OSI/AlgorithmLongitudinalDriverDataprovider.h"

/**
* \brief models the pedal operation of the driver and the reaction of the vehicle
*
* \ingroup DynamicsLongitudinal
*/
class DynamicsLongitudinalImplementation : public DynamicsInterface
{
public:
    const std::string COMPONENTNAME = "DynamicsLongitudinal";

    DynamicsLongitudinalImplementation(
        int componentId,
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
            componentId,
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
            agent)
    {}
    DynamicsLongitudinalImplementation(const DynamicsLongitudinalImplementation&) = delete;
    DynamicsLongitudinalImplementation(DynamicsLongitudinalImplementation&&) = delete;
    DynamicsLongitudinalImplementation& operator=(const DynamicsLongitudinalImplementation&) = delete;
    DynamicsLongitudinalImplementation& operator=(DynamicsLongitudinalImplementation&&) = delete;
    virtual ~DynamicsLongitudinalImplementation() = default;

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
    // --- module internal functions

    //-----------------------------------------------------------------------------
    //! create info string with time, positions, etc.
    //!
    //! @param [in] time
    //!
    //! @return info string
    //-----------------------------------------------------------------------------
    std::string GetInfoString(double time);

    //-----------------------------------------------------------------------------
    //! Checks if v has to be corrected due to collision
    //!
    //! @return flag if v has to be corrected due to collision
    //-----------------------------------------------------------------------------
    bool HasVToBeCorrectedDueToCollision();

    //-----------------------------------------------------------------------------
    //! Get the acceleration of the vehicle from pedal position and gear.
    //!
    //! @return current acceleration
    //-----------------------------------------------------------------------------
    double GetAccVehicle(double accPedalPos, double brakePedalPos, std::vector<int> gear, ALongitudinalDataProvider::CarParameters carParameters, bool output);

    //-----------------------------------------------------------------------------
    //! Get the moment of the engine from pedal position and gear.
    //!
    //! @return engine moment
    //-----------------------------------------------------------------------------
    double GetEngineMoment(double gasPedalPos, int lastGear, ALongitudinalDataProvider::CarParameters carParameters);

    double GetEngineMomentMin(double engineSpeed, ALongitudinalDataProvider::CarParameters carParameters);

    double GetEngineMomentMax(double engineSpeed, ALongitudinalDataProvider::CarParameters carParameters);

    double GetEngineSpeedByVelocity(double xVel, int gear, ALongitudinalDataProvider::CarParameters carParameters);

    double GetAccFromEngineMoment(double xVel, double engineMoment, int chosenGear, ALongitudinalDataProvider::CarParameters carParameters, int cycleTime);

    double GetFrictionCoefficient();

    double _vCollision{0};
    unsigned int _nCollisions = 0;

    // just for testing - delete if no longer needed
    double in_aVehicle = 0;

    // double PI
    double _twoPi = 2 * 3.1415926535;

    // value of earth gravity
    double _oneG = 9.81;

    // air density
    double _rho = 1.23;

    // signals that brake is active or not
    bool _brakeState = false;

    // --- Inputs
    //! Position of the accecaleration pedal position in percent.
    double in_accPedalPos = 0;
    //! Position of the brake pedal position in percent.
    double in_brakePedalPos = 0;
    //! Number of gears and position of gear.
    std::vector<int> in_gear = {6, 0};
    //! contains: double carMass; double rDyn and more;
    ALongitudinalDataProvider::CarParameters in_carParameters;

    // --- Internal Parameter
    //! The minimal velocity of the agent in m/s.
    const double VLowerLimit = 0;
};
