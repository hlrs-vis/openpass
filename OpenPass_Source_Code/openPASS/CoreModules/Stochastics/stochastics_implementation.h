/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef STOCHASTIC_IMPLEMENTATION_H
#define STOCHASTIC_IMPLEMENTATION_H

#include <random>
#include <functional>
#include "stochasticsInterface.h"

/**
* \addtogroup CoreModules_Basic openPASS CoreModules basic
* @{
* \addtogroup Stochastics
*
* \brief Stochastics module to provide stochastic functions.
*
* The Stochastics module implements a StochasticsInterface which is used
* by all agents to create a random behavior.
*
* @see StochasticsInterface
* @}
*/

/*!
 * \copydoc Stochastics
 * \ingroup Stochastics
 */
class Stochastics_Implementation : public StochasticsInterface
{
public:
    Stochastics_Implementation(const CallbackInterface *callbacks);
    Stochastics_Implementation(const Stochastics_Implementation &) = delete;
    Stochastics_Implementation(Stochastics_Implementation &&) = delete;
    Stochastics_Implementation &operator=(const Stochastics_Implementation &) = delete;
    Stochastics_Implementation &operator=(Stochastics_Implementation &&) = delete;
    virtual ~Stochastics_Implementation() = default;

    double GetUniformDistributed(double a, double b);
    double GetNormalDistributed(double mean, double stdDeviation);
    double GetExponentialDistributed(double lambda);
    double GetGammaDistributed(double mean, double stdDeviation);
    double GetLogNormalDistributed(double mean, double stdDeviation);
    double GetSpecialDistributed(std::string distributionName, std::vector<double> args);
    std::uint32_t GetRandomSeed() const;
    void ReInit();
    void InitGenerator(std::uint32_t seed);

protected:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if (callbacks)
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
    std::normal_distribution<double> normalDistribution;
    std::exponential_distribution<double> exponentialDistribution;
    std::function<double(void)> uniformGenerator;
    std::function<double(void)> normalGenerator;
    std::function<double(void)> exponentialGenerator;

    const CallbackInterface *callbacks;
};

#endif // STOCHASTIC_IMPLEMENTATION_H
