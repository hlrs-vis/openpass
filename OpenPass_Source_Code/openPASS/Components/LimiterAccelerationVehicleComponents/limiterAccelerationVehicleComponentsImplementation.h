/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/** \addtogroup LimiterAccelerationVehicleComponents
* @{
* \brief Limits the acceleration output from all vehicle components
*
* @} */

#pragma once

#include "Common/accelerationSignal.h"
#include "Interfaces/modelInterface.h"
#include "Common/parametersVehicleSignal.h"

/**
* \brief This class implements the set limit
*/
class LimiterAccelerationVehicleComponentsImplementation : public UnrestrictedModelInterface
{
public:
    LimiterAccelerationVehicleComponentsImplementation(
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
        UnrestrictedModelInterface(
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
            agent)
    {
    }
    LimiterAccelerationVehicleComponentsImplementation(const LimiterAccelerationVehicleComponentsImplementation&) = delete;
    LimiterAccelerationVehicleComponentsImplementation(LimiterAccelerationVehicleComponentsImplementation&&) = delete;
    LimiterAccelerationVehicleComponentsImplementation& operator=(const LimiterAccelerationVehicleComponentsImplementation&) = delete;
    LimiterAccelerationVehicleComponentsImplementation& operator=(LimiterAccelerationVehicleComponentsImplementation&&) = delete;
    virtual ~LimiterAccelerationVehicleComponentsImplementation() = default;

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
    double InterpolateEngineTorqueBasedOnSpeed(const double &engineSpeed);

    std::vector<double> PrepareEngineTorqueVectorBasedOnGearRatios();

    double CalculateEngineSpeedBasedOnGear(const double &currentVelocity,
                                           const size_t &gear);

    void PrepareReferences();
    double CalculateAccelerationLimit();
    double CalculateDecelerationLimit();

    ComponentState componentState {ComponentState::Armed};

    const double twoPI {2.0 * M_PI};
    const double oneG {9.81}; //mps^2
    const double frictionCoefficient {0.015};
    const double airResistance {1.2};

    VehicleModelParameters vehicleModelParameters;

    std::vector<double> engineTorqueReferences;
    std::vector<double> engineSpeedReferences;

    double incomingAcceleration {0.0};
    double outgoingAcceleration {0.0};
};
