/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef STOCHASTIC_PCM_H
#define STOCHASTIC_PCM_H

#include <random>
#include <functional>

class StochasticsPCM
{
public:
    StochasticsPCM(std::uint32_t seed);
    StochasticsPCM(const StochasticsPCM &) = delete;
    StochasticsPCM(StochasticsPCM &&) = delete;
    StochasticsPCM &operator=(const StochasticsPCM &) = delete;
    StochasticsPCM &operator=(StochasticsPCM &&) = delete;
    virtual ~StochasticsPCM() = default;

    double GetUniformDistributed(double a, double b);
    std::uint32_t GetRandomSeed() const;

private:
    std::uint32_t randomSeed = 0;

    std::mt19937 baseGenerator;
    std::uniform_real_distribution<double> uniformDistribution;
    std::function<double(void)> uniformGenerator;
};

#endif // STOCHASTIC_PCM_H
