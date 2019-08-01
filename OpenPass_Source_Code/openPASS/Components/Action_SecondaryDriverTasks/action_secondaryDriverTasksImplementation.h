/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
//! @file action_secondaryDriverTasksImplementation.h

# pragma once

#ifndef ACTION_SECONDARY_DRIVER_TASKS_IMPLEMENTATION_H
#define ACTION_SECONDARY_DRIVER_TASKS_IMPLEMENTATION_H

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/primitiveSignals.h"

/*! \addtogroup Action_SecondaryDriverTasks
 * @{
 * \brief controls secondary driver tasks
 *
 * This component sets the brake light to on if the acceleration of the
 * vehicle is smaller than the coasting acceleration, sets the indicator and other switches the driver uses during driving.

 * \section Action_SecondaryDriverTasks_Inputs Inputs
 * Input variables:
 * name                      |meaning
 * ---------                 |---------
 * in_IndicatorState         |State of the turning indicator (left, center/off, right).
 * in_hornSwitch             |Horn activation (true/false)
 * in_headLightSwitch        |Light activation (true/false)
 * in_highBeamLightSwitch    |HighBeamLight activation (true/false)
 * in_flasherSwitch          |Flasher activation (true/false)
 *
 * Input channel IDs:
 * Input ID                  |signal class                                    |contained variables
 * ------------              |--------------                                  |-------------
 * 0                         |SecondaryDriverTasksSignal                      |in_IndicatorState, in_hornSwitch, in_headLightSwitch, in_highBeamLightSwitch, in_flasherSwitch
 *
 * \section Action_SecondaryDriverTasks_Outputs Outputs
 * none
 *
 * \section Action_SecondaryDriverTasks_InternalParameters InternalParameters
 * none
 *
 * \section Action_SecondaryDriverTasks_ExternalParameters ExternalParameters
 * none
 * @} */

/*!
 * \brief controls secondary driver tasks
 *
 * This class controls the brakelight, indicators and switches the driver uses during driving.

 * \ingroup ActionScondaryDriverTasks
 * */

class ActionSecondaryDriverTasksImplementation : public ActionInterface
{
public:
    const std::string COMPONENTNAME = "ActionSecondaryDriverTasks";

    using ActionInterface::ActionInterface;
    ActionSecondaryDriverTasksImplementation(const ActionSecondaryDriverTasksImplementation&) = delete;
    ActionSecondaryDriverTasksImplementation(ActionSecondaryDriverTasksImplementation&&) = delete;
    ActionSecondaryDriverTasksImplementation& operator=(const ActionSecondaryDriverTasksImplementation&) = delete;
    ActionSecondaryDriverTasksImplementation& operator=(ActionSecondaryDriverTasksImplementation&&) = delete;
    virtual ~ActionSecondaryDriverTasksImplementation() = default;

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
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

    /*!
     * \brief Update outputs.
     *
     * Function is called by framework when this Component.has to deliver a signal over
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
    /**
     * @} */ // End of Public Variables

private:

    //! State of the turning indicator (left, center/off, right).
    IndicatorState in_IndicatorState = IndicatorState::IndicatorState_Off;

    //! Switch for activation of horn
    bool in_hornSwitch = false;

    //! Switch for activation of headlight
    bool in_headLightSwitch = false;

    //! Switch for activation of Highbeamlight
    bool in_highBeamLightSwitch = false;

    //! Switch for activation of Flasher
    bool in_flasherSwitch = false;
};

    /**
     * @} */ // End of Private Variables

#endif // ACTION_SECONDARY_DRIVER_TASKS_IMPLEMENTATION_H

/** @} */ // End of group ActionSecondaryDriverTasks
