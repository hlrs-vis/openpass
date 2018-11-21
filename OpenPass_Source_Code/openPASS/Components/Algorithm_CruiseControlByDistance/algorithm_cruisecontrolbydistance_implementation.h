/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#ifndef ALGORITHM_CRUISECONTROLBYDISTANCE_IMPLEMENTATION_H
#define ALGORITHM_CRUISECONTROLBYDISTANCE_IMPLEMENTATION_H

#include "modelInterface.h"

/**
* \addtogroup Components_Basic openPASS components basic
* @{
* \addtogroup Algorithm_CruiseControlByDistance
* @{
* \brief models the lateral controller of the driver
*
* This component models the lateral controller of the driver. It calculates a desired
* acceleration based on the desired velocity and a safety distance which is
* numerically half of the velocity (km/h) in m.
*
* \section Algorithm_CruiseControlByDistance_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* in_distance2NextAgent          | Distance to next agent in front.
* in_velocity                    | Current velocity of agent.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0 | DoubleSignal   | in_distance2NextAgent
* 1 | DoubleSignal   | in_velocity
*
* \section Algorithm_CruiseControlByDistance_InitInputs Init Inputs
* none
*
* \section Algorithm_CruiseControlByDistance_Outputs Outputs
* Output variables:
* name | meaning
* -----|------
* out_desiredAcceleration | The desired acceleration based on the desired velocity and distance to next agent in front.
* out_aCoasting | The deceleration caused by the air.
*
* Output channel IDs:
* Output Id | signal class | contained variables
* ------------|--------------|-------------
* 0 | DoubleSignal | out_desiredAcceleration
* 1 | DoubleSignal | out_aCoasting
*
* \section Algorithm_CruiseControlByDistance_ExternalParameters External parameters
* name | value | meaning
* -----|-------|------
* Par_desiredVelocity | 20  | Desired velocity in m/s.
* Par_reactionTime | 0.3 | The reaction time of the driver.
*
* \section Algorithm_CruiseControlByDistance_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|------
* _standardCycleTimeSecond | 0.1 | standard cycle time for spawning, update, trigger tasks (in s)
* _cw | 0.5 | air drag coefficient
* _airDensity | 1.2 | air density at 20°C
* _carHeight | 1.5 | height
* _carWidth | 1.8 | width
* _carWeight | 1300 | weight
* _airDragCoefficent | 0 | Coefficent of car and enviromental constants to calculate the acceleration depending on current the velocity of the car.
* _accelerationMax | 3 | maximal plausilbe acceleration [m/s^2]
* _decelerationMax | 15 | maximal plausible deceleration [m/s^2]
*
* \section Algorithm_CruiseControlByDistance_ConfigParameters Parameters to be specified in agentConfiguration.xml
*
* |type | id | meaning | corresponding external paramter
* |-----|----|---------|----------------------------------
* |double            | 0 |a | Par_desiredVelocity
* |double            | 1 |a | Par_reactionTime
*
*   @}
*   @} */

/*!
* \brief models the lateral controller of the driver
*
* This component models the lateral controller of the driver. It calculates a
* acceleration based on the desired velocity and a safety distance which is
* numerically half of the velocity (km/h) in m.
*
* \ingroup Algorithm_CruiseControlByDistance
*/
class Algorithm_CruiseControlByDistance_Implementation : public AlgorithmInterface
{
public:
    const std::string COMPONENTNAME = "Algorithm_CruiseControlByDistance";

    Algorithm_CruiseControlByDistance_Implementation(
        int componentId,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface *> *observations,
        const CallbackInterface *callbacks,
        int agentId) :
        AlgorithmInterface(
            componentId,
            isInit,
            priority,
            offsetTime,
            responseTime,
            cycleTime,
            stochastics,
            parameters,
            observations,
            callbacks,
            agentId)
    {
        double meanVelocity = GetParameters()->GetParametersDouble().at(0);
        Par_desiredVelocity = std::fmax(5.0, GetStochastics()->GetNormalDistributed(meanVelocity, 10.0));

        Par_reactionTime = GetParameters()->GetParametersDouble().at(1);
        _standardCycleTimeSecond = (double)GetCycleTime() / 1000.0; //ms in s

        // Coefficent of car and enviromental constants to calculate the acceleration depending on current the velocity of the car.
        _airDragCoefficent = _cw * _airDensity * _carHeight * _carWidth / _carWeight / 2;
    }
    Algorithm_CruiseControlByDistance_Implementation(const
                                                     Algorithm_CruiseControlByDistance_Implementation &) = delete;
    Algorithm_CruiseControlByDistance_Implementation(Algorithm_CruiseControlByDistance_Implementation
                                                     &&) = delete;
    Algorithm_CruiseControlByDistance_Implementation &operator=(const
                                                                Algorithm_CruiseControlByDistance_Implementation &) = delete;
    Algorithm_CruiseControlByDistance_Implementation &operator=
    (Algorithm_CruiseControlByDistance_Implementation &&) = delete;
    virtual ~Algorithm_CruiseControlByDistance_Implementation() = default;

    /*!
     * \brief Update Inputs
     *
     * Function is called by framework when another component delivers a signal over
     * a channel to this component (scheduler calls update taks of other component).
     *
     * Refer to module description for input channels and input ids.
     *
     * \param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
     * \param[in]     data           Referenced signal (copied by sending component)
     * \param[in]     time           Current scheduling time
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
     * \param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
     * \param[out]    data           Referenced signal (copied by this component)
     * \param[in]     time           Current scheduling time
     */
    void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    /*!
     * \brief Process data within component.
     *
     * Function is called by framework when the scheduler calls the trigger task
     * of this component.
     *
     * Refer to module description for information about the module's task.
     *
     * \param[in]     time           Current scheduling time
     */
    void Trigger(int time);

private:
    /*!
    * \brief Check an accelation for plausibility
    *
    * Test a desired acceleration for plausibility and return _accelerationMax or
    * _decelerationMax or the same acceleration if it is in range of them.
    *
    * @param[in]    desiredAcceleration         The desired acceleration
    *
    * return                                    A plausible acceleration
    *
    */
    double PlausibleTestAcceleration(double desiredAcceleration);

    //! Distance to next agent in front.
    double in_distance2NextAgent = 0;
    //! Current velocity of agent.
    double in_velocity = 0;
    //! The desired acceleration based on the desired velocity and distance to next agent in front.
    double out_desiredAcceleration = 0;
    //! The brake acceleration that is assumed to happen if no pedal is pressed due to air drag in m/s^2.
    double out_aCoasting = 0;

    /**
        * \addtogroup Algorithm_CruiseControlByDistance
        * @{
        *
        *    @name External Parameters
        *    @{*/
    //! Desired velocity in m/s.
    double Par_desiredVelocity = 0;
    //! The reaction time of the driver.
    double Par_reactionTime = 0;

    /**    @}
           *    @name Internal Parameters
           *    @{*/
    //! standard cycle time for spawning, update, trigger tasks (in s)
    double _standardCycleTimeSecond = 0.1;

    //! air drag coefficient
    double _cw = 0.5;
    //! air density at 20°C
    double _airDensity = 1.2;
    //! height
    double _carHeight = 1.5;
    //! width
    double _carWidth = 1.8;
    //! weight
    double _carWeight = 1300;
    //! Coefficent of car and enviromental constants to calculate the acceleration depending on current the velocity of the car.
    double _airDragCoefficent = 0;

    //! maximal plausilbe acceleration [m/s^2]
    double _accelerationMax = 3;
    //! maximal plausible deceleration [m/s^2]
    double accelerationMin = -15;
    /**@}*/
    /**@}*/

    //! stores the distance of the previous time step
    double previousDistance = 0;
};

#endif // ALGORITHM_CRUISECONTROLBYDISTANCE_IMPLEMENTATION_H
