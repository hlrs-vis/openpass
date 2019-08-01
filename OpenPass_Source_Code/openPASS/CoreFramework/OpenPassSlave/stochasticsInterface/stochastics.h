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
//! @file  Stochastics.h
//! @brief This file contains the internal representation of the stochastics.
//!        For more information see stochastics.dll documentation.
//-----------------------------------------------------------------------------

#pragma once

#include "CoreFramework/CoreShare/log.h"
#include "Interfaces/stochasticsInterface.h"
#include "stochasticsBinding.h"
#include "stochasticsLibrary.h"

namespace SimulationSlave
{

class Stochastics: public StochasticsInterface
{
public:
    Stochastics(StochasticsBinding *stochasticsBinding) :
        stochasticsBinding(stochasticsBinding)
    {}

    Stochastics(const Stochastics&) = delete;
    Stochastics(Stochastics&&) = delete;
    Stochastics& operator=(const Stochastics&) = delete;
    Stochastics& operator=(Stochastics&&) = delete;

    virtual ~Stochastics() = default;

    int GetBinomialDistributed(int upperRangeNum, double probSuccess){
        return implementation->GetBinomialDistributed(upperRangeNum, probSuccess);
    }

    double GetUniformDistributed(double a, double b){
        return implementation->GetUniformDistributed(a, b);
    }

    double GetNormalDistributed(double mean, double stdDeviation){
        return implementation->GetNormalDistributed(mean, stdDeviation);
    }

    double GetExponentialDistributed(double lambda){
        return implementation->GetExponentialDistributed(lambda);
    }

    double GetGammaDistributed(double mean, double stdDeviation){
        return implementation->GetGammaDistributed(mean, stdDeviation);
    }

    double GetLogNormalDistributed(double mean, double stdDeviation){
        return implementation->GetLogNormalDistributed(mean, stdDeviation);
    }

    double GetSpecialDistributed(std::string distributionName, std::vector<double> args){
        return implementation->GetSpecialDistributed(distributionName, args);
    }

    double GetRandomCdfLogNormalDistributed(double mean, double stdDeviation){
        return implementation->GetRandomCdfLogNormalDistributed(mean, stdDeviation);
    }

    double GetPercentileLogNormalDistributed(double mean, double stdDeviation, double probability){
        return implementation->GetPercentileLogNormalDistributed(mean, stdDeviation, probability);
    }

    std::uint32_t GetRandomSeed() const{
        return implementation->GetRandomSeed();
    }

    void ReInit(){
        return implementation->ReInit();
    }

    void InitGenerator(std::uint32_t seed){
        return implementation->InitGenerator(seed);
    }

    bool Instantiate(std::string libraryPath)
    {
        if(!stochasticsBinding){
            return false;
        }else if(!implementation){
            implementation = stochasticsBinding->Instantiate(libraryPath);
            if(!implementation){
                return false;
            }
        }
        return true;
    }

private:
    StochasticsBinding *stochasticsBinding = nullptr;
    StochasticsInterface *implementation = nullptr;   
};

} // namespace SimulationSlave

