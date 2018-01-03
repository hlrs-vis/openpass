/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef ACTION_BRAKELIGHT_BASIC_IMPLEMENTATION_H
#define ACTION_BRAKELIGHT_BASIC_IMPLEMENTATION_H

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"

/*!
* \addtogroup Components_Basic openPASS components basic
* @{
* \addtogroup Action_BrakeLight_Basic
* @{
* \brief sets the brake light
*
* This component sets the brake light to on if the acceleration of the
* vehicle is negative and lower the coasting deceleration.
*
* \section Action_BrakeLight_Basic_Inputs Inputs
* Input variables:
* name | meaning
* -----|---------
* in_aVehicle | The acceleration of the agent in m/s^2.
* in_aCoasting | The deceleration caused by the air.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0   | double | in_aVehicle
* 1   | double | in_aCoasting
*
* \section Action_BrakeLight_Basic_InitInputs Init Inputs
* none
*
* \section Action_BrakeLight_Basic_Outputs Outputs
* none
*
* \section Action_BrakeLight_Basic_ExternalParameters External parameters
* none
*
* \section Action_BrakeLight_Basic_InternalParameters Internal paramters
* none
*
* \section Action_BrakeLight_Basic_ConfigParameters Parameters to be specified in agentConfiguration.xml
* none
*  @}
*  @} */

/*!
 * \brief sets the brake light
 *
 * This class sets the brake light to on if the acceleration of the
 * vehicle is negative and lower the coasting deceleration.
 *
 * \ingroup Action_BrakeLight_Basic
 * */
class Action_BrakeLight_Basic_Implementation : public ActionInterface
{
public:
    const std::string COMPONENTNAME = "Action_BrakeLight_Basic";

    using ActionInterface::ActionInterface;
    Action_BrakeLight_Basic_Implementation(const Action_BrakeLight_Basic_Implementation &) = delete;
    Action_BrakeLight_Basic_Implementation(Action_BrakeLight_Basic_Implementation &&) = delete;
    Action_BrakeLight_Basic_Implementation &operator=(const Action_BrakeLight_Basic_Implementation &) =
        delete;
    Action_BrakeLight_Basic_Implementation &operator=(Action_BrakeLight_Basic_Implementation &&) =
        delete;
    virtual ~Action_BrakeLight_Basic_Implementation() = default;

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
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data,
                             int time);

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
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

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
    virtual void Trigger(int time);

private:
    //! The acceleration of the agent in m/s^2.
    double in_aVehicle = 0;
    //! The brake acceleration that is assumed to happen if no pedal is pressed due to air drag in m/s^2.
    double in_aCoasting = 0;
};

#endif // ACTION_BRAKELIGHT_BASIC_IMPLEMENTATION_H
