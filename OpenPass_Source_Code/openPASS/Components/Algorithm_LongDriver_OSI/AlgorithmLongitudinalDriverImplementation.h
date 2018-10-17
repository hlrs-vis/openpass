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

#pragma once

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "AlgorithmLongitudinalDriverDP.h"

/** \addtogroup AlgorithmLongitudinalDriver
* @{
* \brief models the longitudinal controller of the driver
*
* This component models the longitudinal controller of the driver.
* It calculates the positions of the acceleration pedal and the brake pedal,
* as well as the chosen gear
*
* \section AlgorithmLongitudinalDriver_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* xVel                   | The longitudinal velocity of the current vehicle in m/s.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------

*
* \section AlgorithmLongitudinalDriver_InitInputs Init Inputs
* Init input variables:
* name | meaning
* -----|------
* in_aVehicle            | acceleration wish in ...
*
* Note that these variables are only used implicetly by the component without assigning the values to variable names.
*
* Init input channel IDs:
* Input Id | signal class | contained variables
* ------------|--------------|-------------
* 0 | DoubleSignal | in_aVehicle
*
* \section AlgorithmLongitudinalDriver_Outputs Outputs
* Output variables:
* name | meaning
* -----|------
* out_accPedalPos | The position of the acceleration pedal in percent.
* out_brakePedalPos | The position of the brake pedal in percent.
* out_gear | The total number of gears and the currently chosen gear.
*
* Output channel IDs:
* Output Id | signal class | contained variables
* ------------|--------------|-------------
* 0 | ALongitudinalDriverToDLongitudinalSignal | send by signal: out_accPedalPos, out_brakePedalPos, out_gear
*
* \section AlgorithmLongitudinalDriver_ExternalParameters External parameters
* none
*
* \section AlgorithmLongitudinalDriver_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|------
* none
*
* \section AlgorithmLongitudinalDriver_ConfigParameters Parameters to be specified in agentConfiguration.xml
* none
*
*   @} */

/*!
* \brief models the longitudinal controller of the driver
*
* This class models the longitudinal controller of the driver. It calculates ...
*
* \ingroup AlgorithmLongitudinalDriver
*/
class AlgorithmLongitudinalDriverImplementation : public SensorInterface
{
public:
    const std::string COMPONENTNAME = "AlgorithmLongitudinalDriver";

    AlgorithmLongitudinalDriverImplementation(
        int componentId,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        WorldInterface* world,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface*> *observations,
        const CallbackInterface *callbacks,
        AgentInterface* agent) :
        SensorInterface(
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
    {
        _spawn = true;
        _cycleTime = cycleTime;
        _showValues = false; // for testing purposes
        _stochastics = stochastics;
    }
    AlgorithmLongitudinalDriverImplementation(const AlgorithmLongitudinalDriverImplementation&) = delete;
    AlgorithmLongitudinalDriverImplementation(AlgorithmLongitudinalDriverImplementation&&) = delete;
    AlgorithmLongitudinalDriverImplementation& operator=(const AlgorithmLongitudinalDriverImplementation&) = delete;
    AlgorithmLongitudinalDriverImplementation& operator=(AlgorithmLongitudinalDriverImplementation&&) = delete;

    virtual ~AlgorithmLongitudinalDriverImplementation() = default;

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
    void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

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
    void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    /*!
    * \brief Process data within component.
    *
    * Function is called by framework when the scheduler calls the trigger task
    * of this component
    *
    * @param[in]     time           Current scheduling time
    */
    void Trigger(int time);

private:

    bool _spawn = false;
    bool _showValues = false;
    double _cycleTime = 0;
    bool _firstActuation = true;
    bool _footOnGas = false;
    bool _footOnBrake = false;
    bool _lastFootPosition = false;
    bool _pedalChange = false;
    double _pedalChangeTime = 0;
    std::vector<double> pedalPositions;
    std::vector<double> pedalPositionsHelper;
    std::vector<double> pedalPositionsOut;
    std::vector<double> gasPedalPositionsHelper;
    std::vector<double> brakePedalPositionsHelper;

    //if pedal change ist started
    bool _startPedalChange = true;

    // if correction is finished

    bool _correction = false;
    //whole multiples of cycletime
    int _timeSteps = 0;

    // --- module internal variables
    ObservationInterface* _observer = nullptr;

    // --- Inputs
    //! The wish acceleration of the agent in m/s^2.
    double in_aVehicle = 0;

    //! The current velocity of the agent in m/s
    double in_vVehicle = 0.0;

    double _errorVal = -999;

    // last calculated pedal position (from 1 to -1)
    double _lastPedalPosition = _errorVal;
    // last calculated engine turns
    double _lastEngineSpeed = _errorVal;
    // last known velocity of the vehicle
    double _lastXVel = _errorVal;
    // last known setpoint of acceleration value
    double _lastIn_aVehicle = _errorVal;

    //  --- Outputs
    //! Position of the accecaleration pedal position in percent.
    double out_accPedalPos = 0;
    //! Position of the brake pedal position in percent.
    double out_brakePedalPos = 0;
    //! Number of gears and the currently choosen gear.
    std::vector<int> out_gear = {_carParameters.maxGear, 0};

    // Stochastics
    StochasticsInterface* _stochastics;

    /** @} @} */

    //  --- Init Inputs
    //! contains: double carMass; double rDyn and more;
    ALongitudinalDataProvider::CarParameters _carParameters;

    //  --- Internal Parameters

    /** @} @} */
    void PrintCurrentValues(double lastPedalPosition, double lastEngineSpeed, double wishPedalPosition, int lastGear, int chosenGear, ALongitudinalDataProvider::CarParameters _carParameters, double xVel, double xAcc, double out_accPedalPos, double out_brakePedalPos, double loweredPedalPosition, double regulatedPedalPosition, double nextPedalPosition);
    double GetPedalChangeTime();
    bool IsPedalChangeFinished(double pedalChangeTime, double cycleTime);
};


