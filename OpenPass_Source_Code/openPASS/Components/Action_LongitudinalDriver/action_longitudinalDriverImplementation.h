/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2019 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/*! \addtogroup Action_LongitudinalDriver
 * @{
 * \brief Sets the longitudinal values.
 *
 * \details This component sets pedal positions and gear
 *
 * \section Action_LongitudinalDriver_Inputs Inputs
 * Input variables:
 * name | meaning
 * -----|---------
 * in_accPedalPos   | Acceleration pedal position
 * in_brakePedalPos | Brake pedal position
 * in_gear          | Gear
 *
 * Input channel IDs:
 * Input ID | signal class | contained variables
 * ------------|--------------|-------------
 * 0 | LongitudinalSignal | in_accPedalPos, in_brakePedalPos, in_gear
 *
 * @} */

#ifndef ACTION_LONGITUDINAL_DRIVER_IMPLEMENTATION_H
#define ACTION_LONGITUDINAL_DRIVER_IMPLEMENTATION_H

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/primitiveSignals.h"

//! \ingroup Action_LongitudinalDriver
class ActionLongitudinalDriverImplementation : public ActionInterface
{
public:
    const std::string COMPONENTNAME = "ActionLongitudinalDriver";

    using ActionInterface::ActionInterface;
    ActionLongitudinalDriverImplementation(const ActionLongitudinalDriverImplementation&) = delete;
    ActionLongitudinalDriverImplementation(ActionLongitudinalDriverImplementation&&) = delete;
    ActionLongitudinalDriverImplementation& operator=(const ActionLongitudinalDriverImplementation&) = delete;
    ActionLongitudinalDriverImplementation& operator=(ActionLongitudinalDriverImplementation&&) = delete;
    virtual ~ActionLongitudinalDriverImplementation() = default;

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
    virtual void UpdateInput(int localLinkId,
                             const std::shared_ptr<SignalInterface const> &data,
                             int time);

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
    virtual void UpdateOutput(int localLinkId,
                              std::shared_ptr<SignalInterface const> &data,
                              int time);

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

    // --- Inputs
    //! Position of the accecaleration pedal position in percent.
    double in_accPedalPos = 0;
    //! Position of the brake pedal position in percent.
    double in_brakePedalPos = 0;
    //! Number of gears and position of gear.
    int in_gear = 0;
};

#endif // ACTION_LONGITUDINAL_DRIVER_IMPLEMENTATION_H
