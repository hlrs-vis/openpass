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
//! @file  StochasticsInterface.h
//! @brief This file provides the interface of the stochastic algorithms to
//!        custom libraries.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include "Interfaces/callbackInterface.h"

//-----------------------------------------------------------------------------
//! Provides access to the stochastics functionality of the framework
//-----------------------------------------------------------------------------
class StochasticsInterface
{
public:    
    StochasticsInterface() = default;
    StochasticsInterface(const StochasticsInterface&) = delete;
    StochasticsInterface(StochasticsInterface&&) = delete;
    StochasticsInterface& operator=(const StochasticsInterface&) = delete;
    StochasticsInterface& operator=(StochasticsInterface&&) = delete;
    virtual ~StochasticsInterface() = default;

    //-----------------------------------------------------------------------------
    //! Draw from uniform distribution.
    //!
    //! @param[in]     a       minimum value
    //! @param[in]     b       maximum value
    //! @return                value drawn
    //-----------------------------------------------------------------------------
    virtual double GetUniformDistributed(double a, double b) = 0;

    //-----------------------------------------------------------------------------
    //! Draw from uniform distribution.
    //!
    //! @param[in]     upperRangeNum     maximum value (min value is 0)
    //! @param[in]     probSuccess       probability of success
    //! @return                          value drawn
    //-----------------------------------------------------------------------------
    virtual int GetBinomialDistributed(int upperRangeNum, double probSuccess) = 0;

    //-----------------------------------------------------------------------------
    //! Draw from normal distribution.
    //!
    //! @param[in]     mean         mean value
    //! @param[in]     stdDeviation standard deviation from mean value
    //! @return                     value drawn
    //-----------------------------------------------------------------------------
    virtual double GetNormalDistributed(double mean, double stdDeviation) = 0;

    //-----------------------------------------------------------------------------
    //! Draw from exponential distribution.
    //!
    //! @param[in]     lambda       rate parameter
    //! @return                     value drawn
    //-----------------------------------------------------------------------------
    virtual double GetExponentialDistributed(double lambda) = 0;

    //-----------------------------------------------------------------------------
    //! Draw from gamma distribution.
    //!
    //! @param[in]     mean         mean value
    //! @param[in]     stdDeviation standard deviation from mean value
    //! @return                     value drawn
    //-----------------------------------------------------------------------------
    virtual double GetGammaDistributed(double mean, double stdDeviation) = 0;

    //-----------------------------------------------------------------------------
    //! Draw from log-normal distribution.
    //!
    //! @param[in]     mean         mean value
    //! @param[in]     stdDeviation standard deviation from mean value
    //! @return                     value drawn
    //-----------------------------------------------------------------------------
    virtual double GetLogNormalDistributed(double mean, double stdDeviation) = 0;

    //-----------------------------------------------------------------------------
    //! Used for a user specific distribution.
    //!
    //! @param[in]     distributionName Name of distribution which is used
    //! @param[in]     args             vector of arguments for distribution
    //! @return                         value drawn
    //-----------------------------------------------------------------------------
    virtual double GetSpecialDistributed(std::string distributionName,
                                         std::vector<double> args) = 0;

    //-----------------------------------------------------------------------------
    //! Get the cdf value of a random draw from log-normal distribution.
    //!
    //! @param[in]     mean         mean value
    //! @param[in]     stdDeviation standard deviation from mean value
    //! @return                     value cdf
    //-----------------------------------------------------------------------------
    virtual double GetRandomCdfLogNormalDistributed(double mean,
                                                 double stdDeviation) = 0;

    //-----------------------------------------------------------------------------
    //! Draw value from log-normal distribution according to percentile / quantile.
    //!
    //! @param[in]     mean         mean value
    //! @param[in]     stdDeviation standard deviation from mean value
    //! @param[in]     probability  percentile / quantile probability
    //! @return                     value drawn
    //-----------------------------------------------------------------------------
    virtual double GetPercentileLogNormalDistributed(double mean, double stdDeviation,
                                                  double probability) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves seed.
    //!
    //! @return                     seed
    //-----------------------------------------------------------------------------
    virtual std::uint32_t GetRandomSeed() const = 0;

    //-----------------------------------------------------------------------------
    //! Reinitializes the generator
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void ReInit() = 0;

    //-----------------------------------------------------------------------------
    //! Initializes the generator with given seed
    //! @param[in]     seed         seed for generator to reproduce results
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void InitGenerator(std::uint32_t seed) = 0;

    virtual bool Instantiate(std::string) {return false;}
};


