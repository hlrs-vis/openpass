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

/** \addtogroup Stochastics
* @{
* \brief implementation of StochasticsInterface
*
* \details The Stochastics module implements a StochasticsInterface which is used
* by all agents to create a random behavior.
*
* \section Stochastics_Inputs Inputs
* name | meaning
* -----|---------
* callbacks | CallbackInterface to send log information to the framework
*
* \section Stochastics_Outputs Outputs
 name | meaning
* -----|---------
* *StochasticsInterface | Provides a reference to an implementation of a StochasticsInterface to the framework.
*
* @see StochasticsInterface
*
* @} */

#pragma once

/// \brief prevents any long double functions from being defined or used with Boost.Math
/// this will reduce precision slightly, but will typically improve performance on 64-bit hardware
/// \remark make valgrind working if opimization is set by compiler flags -O2, -O3, or -Og
/// \sa     https://svn.boost.org/trac10/ticket/10005
#define BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#define BOOST_MATH_PROMOTE_DOUBLE_POLICY false

#include <random>
#include <vector>
#include <functional>
#include "Interfaces/stochasticsInterface.h"
#include "boost/math/distributions/lognormal.hpp"

using namespace boost::math;
using BinomialDist = std::binomial_distribution<>;

/*!
 * \brief implementation of StochasticsInterface
 *
 * The Stochastics module implements a StochasticsInterface which is used
 * by all agents to create a random behavior.
 *
 * \ingroup Stochastics
 */
class StochasticsImplementation : public StochasticsInterface
{
public:
    StochasticsImplementation(const CallbackInterface *callbacks);
    StochasticsImplementation(const StochasticsImplementation&) = delete;
    StochasticsImplementation(StochasticsImplementation&&) = delete;
    StochasticsImplementation& operator=(const StochasticsImplementation&) = delete;
    StochasticsImplementation& operator=(StochasticsImplementation&&) = delete;
    virtual ~StochasticsImplementation() = default;

    /*!
    * \brief Generates an uniform distributed number.
    *
    * \details Generates an uniform distributed number between two input values.
    *
    *
    * @param[in]     a         First boundary.
    * @param[in]     b         Second boundary.
    *
    * @return        Uniform distributed number.
    */
    double GetUniformDistributed(double a, double b);

    /*!
    * \brief Generates a normal distributed number.
    *
    * \details Generates a normal distributed number according to a mean value and a standard deviation.
    *
    *
    * @param[in]     mean               Average value.
    * @param[in]     stdDeviation       Standard deviation.
    *
    * @return        Normal distributed number.
    */
    double GetNormalDistributed(double mean, double stdDeviation);

    int GetBinomialDistributed(int upperRangeNum, double probSuccess);

    /*!
    * \brief Generates a exponential distributed number.
    *
    * \details Generates a exponential distributed number with a certain rate.
    *
    *
    * @param[in]     lambda             Rate.
    *
    * @return        Exponential distributed number.
    */
    double GetExponentialDistributed(double lambda);

    /*!
    * \brief Generates a gamma distributed number.
    *
    * \details Generates a gamma distributed number according to a mean value and a standard deviation.
    *
    *
    * @param[in]     mean               Average value.
    * @param[in]     stdDeviation       Standard deviation.
    *
    * @return        Gamma distributed number.
    */
    double GetGammaDistributed(double mean, double stdDeviation);

    /*!
    * \brief Generates a logarithmic normal distributed number.
    *
    * \details Generates a logrithmic normal distributed number according to a mean value and a standard deviation.
    *
    *
    * @param[in]     mean               Average value.
    * @param[in]     stdDeviation       Standard deviation.
    *
    * @return        Logarithmic normal distributed number.
    */
    double GetLogNormalDistributed(double mean, double stdDeviation);

    /*!
    * \brief Generates a special distributed number.
    *
    * \details Not implemented.
    *
    *
    * @param[in]     distributionName           Name of the Distribution.
    * @param[in]     args                       Arguments of the distribution.
    *
    * @return        0.
    */
    double GetSpecialDistributed(std::string distributionName, std::vector<double> args);

    /*!
    * \brief tbd.
    *
    * \details tbd.
    *
    *
    * @param[in]     mean               Average value.
    * @param[in]     stdDeviation       Standard deviation.
    *
    * @return        tbd.
    */
    double GetRandomCdfLogNormalDistributed(double mean, double stdDeviation);

    /*!
    * \brief tbd.
    *
    * \details tbd.
    *
    *
    * @param[in]     mean               Average value.
    * @param[in]     stdDeviation       Standard deviation.
    * @param[in]     probability        Probability.
    *
    * @return        tbd.
    */
    double GetPercentileLogNormalDistributed(double mean, double stdDeviation, double probability);

    /*!
    * \brief Returns the random seed.
    *
    * \details Returns the random seed.
    *
    * @return        Random seed.
    */
    std::uint32_t GetRandomSeed() const;

    /*!
    * \brief Reinitalizes the stochastics module.
    *
    * \details Uses the current random seed to generate a new random seed.
    * Then reinitalizes the generators with the new random seed.
    */
    void ReInit();

    /*!
    * \brief Initializes the generators with a random seed.
    *
    * \details Sets the seed for the base generator.
    * Then initalizes all other generators.
    *
    * @param[in]    seed        Random seed.
    */
    void InitGenerator(std::uint32_t seed);

protected:
    /*! Provides callback to LOG() macro
    *
    * @param[in]     logLevel    Importance of log
    * @param[in]     file        Name of file where log is called
    * @param[in]     line        Line within file where log is called
    * @param[in]     message     Message to log
    */
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    std::uint32_t randomSeed = 0;

    std::mt19937 baseGenerator;
    std::uniform_real_distribution<double> uniformDistribution;
    BinomialDist binomialDistribution;
    std::normal_distribution<double> normalDistribution;
    std::exponential_distribution<double> exponentialDistribution;

    const CallbackInterface *callbacks;
};
