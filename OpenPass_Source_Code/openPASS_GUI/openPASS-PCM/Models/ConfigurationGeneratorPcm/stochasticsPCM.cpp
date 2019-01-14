/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <qglobal.h>
#include "StochasticsPCM.h"

StochasticsPCM::StochasticsPCM(std::uint32_t seed) : randomSeed(seed), baseGenerator(0), uniformDistribution(0, 1)
{
    baseGenerator.seed(seed);
    uniformGenerator = std::bind(uniformDistribution, baseGenerator);
}

double StochasticsPCM::GetUniformDistributed(double a, double b)
{
    double draw = uniformGenerator();
//    qDebug("GetUniformDistributed: %f", draw);
    return  draw * (b - a) + a;
}

std::uint32_t StochasticsPCM::GetRandomSeed() const
{
    return randomSeed;
}
