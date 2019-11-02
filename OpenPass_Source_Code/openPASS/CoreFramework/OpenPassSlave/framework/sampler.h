/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  Sampler.h
//! @brief This file contains the Sampler, which samples parameters and
//!  probabilities.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/samplerInterface.h"
#include "Interfaces/stochasticsInterface.h"

//-----------------------------------------------------------------------------
/** \brief Sampler samples probabilities and SpawnPoint and World parameters.
*   \details This class contains all stochastics functions of the SpawnPoint and the world
*               It samples parameters according to probabilities specified in the configurations
*
*/
//-----------------------------------------------------------------------------
class Sampler : public SamplerInterface
{
public:
    Sampler(StochasticsInterface& stochastics);
    virtual ~Sampler() override = default;

    virtual bool RollFor(double chance) const override;
    virtual double RollGapBetweenCars(double carRatePerSecond) const override;
    virtual double RollGapExtension(double extension) const override;
    virtual double RollForVelocity(double meanVelocity, double stdDeviationVelocity) const override;
    virtual double RollForStochasticAttribute(double mean, double stdDev, double lowerBound, double upperBound) const override;
    virtual size_t RollUniformDistributedVectorIndex(size_t vectorSize) const override;
    virtual std::string SampleStringProbability(StringProbabilities probabilities) const override;
    virtual int SampleIntProbability(IntProbabilities probabilities) const override;
    virtual double SampleDoubleProbability(DoubleProbabilities probabilities) const override;
    virtual void SampleSpawnPointParameters(TrafficConfig& trafficConfig, ParameterInterface* parameters) const override;
    virtual void SampleWorldParameters(EnvironmentConfig& environmentConfig, ParameterInterface* parameters) const override;

private:
    const std::string COMPONENTNAME = "Sampler";

    StochasticsInterface& stochastics;

    static constexpr double RATE_ALWAYS_TRUE {1.0};
    static constexpr double RATE_ALWAYS_FALSE {0.0};
};
