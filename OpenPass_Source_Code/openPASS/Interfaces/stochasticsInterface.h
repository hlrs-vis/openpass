/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  stochasticsInterface.h
//! @brief This file provides the interface of the stochastic algorithms to
//!        custom libraries.
//-----------------------------------------------------------------------------

#ifndef STOCHASTICSINTERFACE_H
#define STOCHASTICSINTERFACE_H

#include <string>
#include <vector>
#include "callbackInterface.h"

//-----------------------------------------------------------------------------
//! Provides access to the stochastics functionality of the framework
//-----------------------------------------------------------------------------
class StochasticsInterface
{
public:
    StochasticsInterface() = default;
    StochasticsInterface(const StochasticsInterface &) = delete;
    StochasticsInterface(StochasticsInterface &&) = delete;
    StochasticsInterface &operator=(const StochasticsInterface &) = delete;
    StochasticsInterface &operator=(StochasticsInterface &&) = delete;
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
};

#endif // STOCHASTICSINTERFACE_H
