/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <qglobal.h>
#include "stochastics_implementation.h"

Stochastics_Implementation::Stochastics_Implementation(const CallbackInterface *callbacks) :
    baseGenerator(0),
    uniformDistribution(0,1),
    normalDistribution(0,1),
    exponentialDistribution(1),
    callbacks(callbacks)
{
    uniformGenerator = std::bind(uniformDistribution, baseGenerator);
    normalGenerator = std::bind(normalDistribution, baseGenerator);
    exponentialGenerator = std::bind(exponentialDistribution, baseGenerator);
}

double Stochastics_Implementation::GetUniformDistributed(double a, double b)
{
    double draw = uniformGenerator();
    LOG(CbkLogLevel::Debug, "GetUniformDistributed " + std::to_string(draw));
    return  draw * (b - a) + a;
}

double Stochastics_Implementation::GetNormalDistributed(double mean, double stdDeviation)
{
    if(0 > stdDeviation)
    {
        LOG(CbkLogLevel::Warning, "GetNormalDistributed: stdDeviation negative");
        return mean;
    }
    double draw = normalGenerator();
    LOG(CbkLogLevel::Debug, "GetNormalDistributed " + std::to_string(draw));
    return stdDeviation * draw + mean;
}

double Stochastics_Implementation::GetExponentialDistributed(double lambda)
{
    double draw = exponentialGenerator();
    LOG(CbkLogLevel::Debug, "GetExponentialDistributed " + std::to_string(draw));
    return draw / lambda;
}

double Stochastics_Implementation::GetGammaDistributed(double mean, double stdDeviation)
{
    // b=1/beta; p=alpha;
    // E = alpha * beta; stdDev = sqrt(alpha) * beta;
    // => alpha = E^2 / std^2; beta = std^2 / E
    double var = stdDeviation * stdDeviation;
    if(0 > var / mean)
    {
        LOG(CbkLogLevel::Warning, "GetGammaDistributed: stdDeviation^2/mean negative");
        return mean;
    }
    std::gamma_distribution<double> gammaDistribution(mean * mean / var, var / mean);
    auto gammaGenerator = std::bind(gammaDistribution, baseGenerator);

    double draw = gammaGenerator();
    LOG(CbkLogLevel::Debug, "GetGammaDistributed " + std::to_string(draw));
    baseGenerator.seed(baseGenerator());
    return draw;
}

double Stochastics_Implementation::GetLogNormalDistributed(double mean, double stdDeviation)
{
    double s2 = log(pow(stdDeviation/mean,2)+1);
    std::lognormal_distribution<double> lognormalDistribution(log(mean)-s2/2, sqrt(s2));

    auto lognormalGenerator = std::bind(lognormalDistribution, baseGenerator);

    double draw = lognormalGenerator();
    LOG(CbkLogLevel::Debug, "GetLogNormalDistributed " + std::to_string(draw));

    baseGenerator.seed(baseGenerator());

    return draw;
}

double Stochastics_Implementation::GetSpecialDistributed(std::string distributionName, std::vector<double> args)
{
    Q_UNUSED(distributionName);
    Q_UNUSED(args);
    return 0;
}

std::uint32_t Stochastics_Implementation::GetRandomSeed() const
{
    return randomSeed;
}

void Stochastics_Implementation::ReInit()
{
    randomSeed = baseGenerator();
    InitGenerator(randomSeed);
}

void Stochastics_Implementation::InitGenerator(std::uint32_t seed)
{
    randomSeed = seed;
    baseGenerator.seed(seed);
    uniformDistribution.reset();
    normalDistribution.reset();
    exponentialDistribution.reset();
    uniformGenerator = std::bind(uniformDistribution, baseGenerator);
    normalGenerator = std::bind(normalDistribution, baseGenerator);
    exponentialGenerator = std::bind(exponentialDistribution, baseGenerator);
}
