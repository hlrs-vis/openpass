/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/** \addtogroup SignalPrioritizer
* @{
* \brief  Collects signals of same kind and relays the one with highest priority
*
* This components acts as intersection between multiple modules, potentially
* setting the same signals. In such a case, the signal with the highest priority
* will be hold until UpdateOutput is invoked.
*
* Example: UpdateInput (t=0): BrakeSignal SenderId=2, Priority=2
*          UpdateInput (t=0): BrakeSignal SenderId=3, Priority=5
*          UpdateInput (t=0): BrakeSignal SenderId=1, Priority=1
*          UpdateOutput (t=0): Transmit BrakeSignal SenderId=3
*
* The priorities are set by means of the parameter interface providing a map of
* "ModuleIds" (sender) and "Priority" (largest priority wins).
* */

#pragma once

#include <unordered_map>
#include <map>
#include "Interfaces/modelInterface.h"

//-----------------------------------------------------------------------------
/** \brief This class manages the routing of signals to the sepcific components.
*
* 	\ingroup SignalPrioritizer
*/
//-----------------------------------------------------------------------------
class SignalPrioritizerImplementation : public AlgorithmInterface
{
private:
    using Signal = std::shared_ptr<SignalInterface const>;

    std::unordered_map<int, int> senderPriorities;

    std::map<int, std::shared_ptr<ComponentStateSignalInterface const>> holdSignals;

public:
    const std::string COMPONENTNAME = "SignalPrioritizer";

    SignalPrioritizerImplementation(
            std::string componentName,
            bool isInit,
            int priority,
            int offsetTime,
            int responseTime,
            int cycleTime,
            StochasticsInterface *stochastics,
            const ParameterInterface *parameters,
            const std::map<int, ObservationInterface*> *observations,
            const CallbackInterface *callbacks,
            AgentInterface *agent);

    /*!
    * \brief Holds the signal if there are other with higher priority.
    *
    * @param[in]    signal     Signal.
    * @param[in]     time       Timestamp in milliseconds.
    */
    void UpdateInput(int localLinkId, const Signal &signal, int time) override;

    /*!
    * \brief Changes the signal to a signal on hold if there is still on on hold in the current time step.
    *
    * @param[out]    signal     Signal.
    * @param[in]     time       Timestamp in milliseconds.
    */
    void UpdateOutput(int localLinkId, Signal &signal, int time) override;

    void Trigger(int) override {}

private:
    /*!
    * \brief Translates the string/int parameter representation to the internally used representation of priorities
    *
    * \param[in] priorities   map linking moduleIds to priorities (larger = higher priority)
    */
    void ParseSenderPriorities(const std::map<std::string, int> &priorities);

    /*!
    * \brief Checks if the provided signal is valid and in the active state
    *
    * \param[in] signal       signal with component state
    * \return             true if active
    */
    bool IsActive(const std::shared_ptr<const ComponentStateSignalInterface> &signal) const;

    /*!
    * Compares the priority of two signals
    *
    * \param[in] firstSignal        firstSignal
    * \param[in] secondSignal       secondSignal
    * \return             true if firstSignal has highest priority
    */
    bool IsPrior(int firstSignal, int secondSignal) const;

    /*!
    * \brief Returns the signal with the highest priority
    *
    * \return       signal with highest priority
    */
    std::shared_ptr<SignalInterface const> GetSignalWithHighestPriority() const;

    /*!
    * \brief Helper method for reporting errors
    *
    * \param[in] message      message for to the log and thrown exception
    */
    [[noreturn]]void Error(std::string message) const;
};



