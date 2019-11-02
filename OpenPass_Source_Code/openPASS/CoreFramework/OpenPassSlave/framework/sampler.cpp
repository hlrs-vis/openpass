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
/** \file  Sampler.cpp */
//-----------------------------------------------------------------------------

#include "sampler.h"

Sampler::Sampler(StochasticsInterface &stochastics):
    stochastics(stochastics)
{
}

bool Sampler::RollFor(double chance) const
{
    if (chance == RATE_ALWAYS_FALSE)
    {
        return false;
    }

    double roll = stochastics.GetUniformDistributed(0, 1);
    return (roll <= chance);
}

double Sampler::RollGapBetweenCars(double carRatePerSecond) const
{
    return stochastics.GetExponentialDistributed(carRatePerSecond);
}

double Sampler::RollGapExtension(double extension) const
{
    return stochastics.GetUniformDistributed(0, extension);
}

double Sampler::RollForVelocity(double meanVelocity, double stdDeviationVelocity) const
{
    return stochastics.GetNormalDistributed(meanVelocity, stdDeviationVelocity);
}

double Sampler::RollForStochasticAttribute(double mean, double stdDev, double lowerBound, double upperBound) const
{
    int run = 0;
    double result = stochastics.GetNormalDistributed(mean, stdDev);

    while (result > upperBound || result < lowerBound)
    {
        run++;
        result = stochastics.GetNormalDistributed(mean, stdDev);
        if (run == 10)
        {
            return mean;
        }
    }

    return result;
}

size_t Sampler::RollUniformDistributedVectorIndex(size_t vectorSize) const
{
    size_t index = static_cast<size_t>(floor(stochastics.GetUniformDistributed(0, 1) * vectorSize));

    if (vectorSize == index)
    {
        index--;
    }

    return index;
}

std::string Sampler::SampleStringProbability(StringProbabilities probabilities) const
{
    double roll = stochastics.GetUniformDistributed(0, 1);
    double probability = 0.0;

    for (auto entry : probabilities)
    {
        probability += entry.second;

        if (probability == 0.0)
        {
            continue;
        }

        if (roll <= probability)
        {
            return entry.first;
        }
    }

    return "";
}

int Sampler::SampleIntProbability(IntProbabilities probabilities) const
{
    double roll = stochastics.GetUniformDistributed(0, 1);
    double probability = 0.0;

    for (auto entry : probabilities)
    {
        probability += entry.second;

        if (probability == 0.0)
        {
            continue;
        }

        if (roll <= probability)
        {
            return entry.first;
        }
    }

    throw std::logic_error("Could not find matching probability within range.");
}

double Sampler::SampleDoubleProbability(DoubleProbabilities probabilities) const
{
    double roll = stochastics.GetUniformDistributed(0, 1);
    double probability = 0.0;

    for (auto entry : probabilities)
    {
        probability += entry.second;

        if (probability == 0.0)
        {
            continue;
        }

        if (roll <= probability)
        {
            return entry.first;
        }
    }

    throw std::logic_error("Could not find matching probability within range.");
}

void Sampler::SampleSpawnPointParameters(TrafficConfig& trafficConfig, ParameterInterface* parameters) const
{
    //Sample traffic volume
    double trafficVolume = SampleDoubleProbability(trafficConfig.trafficVolumes);
    parameters->AddParameterDouble("TrafficVolume", trafficVolume);

    //Sample platoon rate
    double platoonRate = SampleDoubleProbability(trafficConfig.platoonRates);
    parameters->AddParameterDouble("PlatoonRate", platoonRate);

    //Sample velocity
    double velocity = SampleDoubleProbability(trafficConfig.velocities);
    parameters->AddParameterDouble("Velocity", velocity);

    //Sample homogenity
    double homogenity = SampleDoubleProbability(trafficConfig.homogenities);
    parameters->AddParameterDouble("Homogenity", homogenity);
}

void Sampler::SampleWorldParameters(EnvironmentConfig& environmentConfig, ParameterInterface* parameters) const
{
    //Sample time of day
    std::string timeOfDay = SampleStringProbability(environmentConfig.timeOfDays);
    parameters->AddParameterString("TimeOfDay", timeOfDay);

    //Sample visibility distance
    int visibilityDistance = SampleIntProbability(environmentConfig.visibilityDistances);
    parameters->AddParameterInt("VisibilityDistance", visibilityDistance);

    //Sample friction
    double friction = SampleDoubleProbability(environmentConfig.frictions);
    parameters->AddParameterDouble("Friction", friction);

    //Sample weather
    std::string weather = SampleStringProbability(environmentConfig.weathers);
    parameters->AddParameterString("Weather", weather);
}

