/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  StochasticsImplementation.cpp */
//-----------------------------------------------------------------------------

#include <qglobal.h>
#include <stdexcept>
#include <cmath>
#include "stochastics_implementation.h"

StochasticsImplementation::StochasticsImplementation(const CallbackInterface *callbacks) :
    baseGenerator(0),
    uniformDistribution(0, 1),
    binomialDistribution(1, 0.5),
    normalDistribution(0, 1),
    exponentialDistribution(1),
    callbacks(callbacks)
{
}

int StochasticsImplementation::GetBinomialDistributed(int upperRangeNum, double probSuccess)
{
    binomialDistribution.param(BinomialDist::param_type(upperRangeNum,probSuccess));
    int draw = binomialDistribution(baseGenerator);
    LOG(CbkLogLevel::Debug, "GetBinomialDistributed " + std::to_string(draw));
    return draw;
}

double StochasticsImplementation::GetUniformDistributed(double a, double b)
{
    uniformDistribution.param(std::uniform_real_distribution<double>::param_type(a, b));
    double draw = uniformDistribution(baseGenerator);
    LOG(CbkLogLevel::Debug, "GetUniformDistributed " + std::to_string(draw));
    return  draw;
}

double StochasticsImplementation::GetNormalDistributed(double mean, double stdDeviation)
{
    if(0 > stdDeviation)
    {
        LOG(CbkLogLevel::Warning, "GetNormalDistributed: stdDeviation negative");
        return mean;
    }
    double draw = normalDistribution(baseGenerator);
    LOG(CbkLogLevel::Debug, "GetNormalDistributed " + std::to_string(draw));
    return stdDeviation * draw + mean;
}

double StochasticsImplementation::GetExponentialDistributed(double lambda)
{
    double draw = exponentialDistribution(baseGenerator);
    LOG(CbkLogLevel::Debug, "GetExponentialDistributed " + std::to_string(draw));
    return draw / lambda;
}

double StochasticsImplementation::GetGammaDistributed(double mean, double stdDeviation)
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
    return draw;
}

double StochasticsImplementation::GetLogNormalDistributed(double mean, double stdDeviation)
{
    double s2 = log(pow(stdDeviation/mean, 2)+1);

    std::lognormal_distribution<double> lognormalDistribution(log(mean)-s2/2, sqrt(s2));

    double draw = lognormalDistribution(baseGenerator);
    LOG(CbkLogLevel::Debug, "GetLogNormalDistributed " + std::to_string(draw));

    return draw;
}

double StochasticsImplementation::GetSpecialDistributed(std::string distributionName, std::vector<double> args)
{
    Q_UNUSED(distributionName);
    Q_UNUSED(args);
    return 0;
}

double StochasticsImplementation::GetPercentileLogNormalDistributed(double mean, double stdDeviation, double probability)
{
    double s2 = log(pow(stdDeviation/mean, 2)+1);

    double location = log(mean)-s2/2;
    double scale = sqrt(s2);

    boost::math::lognormal_distribution<> logNormD(location, scale);

    double draw = boost::math::quantile(logNormD, probability);

    return draw;
}

double StochasticsImplementation::GetRandomCdfLogNormalDistributed(double mean, double stdDeviation)
{
    double draw = GetLogNormalDistributed(mean, stdDeviation);

    // std::cout << "DRAW orig: " << draw << std::endl;

    double s2 = log(pow(stdDeviation/mean, 2)+1);
    double location = log(mean)-s2/2;
    double scale = sqrt(s2);

    boost::math::lognormal_distribution<> logNormD(location, scale);

    double probabilityCdf = boost::math::cdf(logNormD, draw);

    // std::cout << "DRAW from boost math distribution LogNorm at cdfProb " << probabilityCdf * 100 << "%: " << GetPercentileNormalDistributed(mean, stdDeviation, probabilityCdf)<< std::endl;

    return probabilityCdf;
}

std::uint32_t StochasticsImplementation::GetRandomSeed() const
{
    return randomSeed;
}

void StochasticsImplementation::ReInit()
{
    randomSeed = baseGenerator();
    InitGenerator(randomSeed);
}

void StochasticsImplementation::InitGenerator(std::uint32_t seed)
{
    randomSeed = seed;
    baseGenerator.seed(seed);

    uniformDistribution.reset();
    binomialDistribution.reset();
    normalDistribution.reset();
    exponentialDistribution.reset();
}
