/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  stochastics.h
//! @brief This file contains the internal representation of the stochastics.
//!        For more information see stochastics.dll documentation.
//-----------------------------------------------------------------------------

#ifndef STOCHASTICS_H
#define STOCHASTICS_H

#include "log.h"
#include "stochasticsInterface.h"
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

    double GetUniformDistributed(double a, double b){
        return implementation->GetUniformDistributed(a,b);
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

    std::uint32_t GetRandomSeed() const{
        return implementation->GetRandomSeed();
    }

    void ReInit(){
        return implementation->ReInit();
    }

    void InitGenerator(std::uint32_t seed){
        return implementation->InitGenerator(seed);
    }

    bool Instantiate(SimulationCommon::RunConfig::StochasticsInstance *stochasticsInstance)
    {
        if((!stochasticsBinding) || (!stochasticsInstance)){
            return false;
        }else if(!implementation){
            implementation = stochasticsBinding->Instantiate(stochasticsInstance);
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
#endif // STOCHASTICS_H
