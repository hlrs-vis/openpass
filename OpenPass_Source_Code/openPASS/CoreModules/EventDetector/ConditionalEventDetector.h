/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "EventDetectorCommonBase.h"
#include "ConditionInterface.h"

class ConditionalEventDetector : public EventDetectorCommonBase
{
public:
    ConditionalEventDetector(WorldInterface *world,
                             ParameterInterface *parameters,
                             SimulationSlave::EventNetworkInterface *eventNetwork,
                             const CallbackInterface *callbacks,
                             StochasticsInterface *stochastics);

    ConditionalEventDetector() = delete;
    ConditionalEventDetector(const ConditionalEventDetector&) = delete;
    ConditionalEventDetector(ConditionalEventDetector&&) = delete;
    ConditionalEventDetector& operator=(const ConditionalEventDetector&) = delete;
    ConditionalEventDetector& operator=(ConditionalEventDetector&&) = delete;

    virtual void Trigger(int time) override;

private:
    using Conditions = std::list<std::shared_ptr<ConditionInterface>>;

    Conditions conditions;
    bool triggered{false};

    void ParseParameters(const ParameterInterface* const parameters);

    /*!
     * ------------------------------------------------------------------------
     * \brief GetConditions Get the conditions for which the event detector
     *        will activate.
     *
     * \returns the conditions for which the event detector will activate.
     * ------------------------------------------------------------------------
     */
    Conditions GetConditions() const;

    /*!
     * ------------------------------------------------------------------------
     * \brief AllConditionsMet Checks if all conditions parameters are met.
     *
     * \param[in] time the time at which the check is made.
     *
     * \returns true if all conditions are met, false otherwise.
     * ------------------------------------------------------------------------
     */
    bool AllConditionsMet(int time) const;

    /*!
     * ------------------------------------------------------------------------
     * \brief TriggerEventInsertion inserts an event into the EventNetwork
     *        at the specified time.
     *
     * \param[in] time The time at which to insert the event into the
     *            EventNetwork.
     * ------------------------------------------------------------------------
     */
    void TriggerEventInsertion(int time);
};
