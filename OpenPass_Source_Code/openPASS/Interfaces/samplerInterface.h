/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

//-----------------------------------------------------------------------------
/** \file  SamplerInterface.h */
//-----------------------------------------------------------------------------

#include <string>

#include "Interfaces/slaveConfigInterface.h"
#include "Interfaces/stochasticsInterface.h"

class SamplerInterface
{
public:
    virtual ~SamplerInterface() = default;

    /*!
    * \brief Rolls a probability
    *
    * \details Rolls a uniform distributed number between 0.0 and 1.0
    *
    *
    * @param[in]    chance      Chance the roll gets compared to
    * @return       true if probability is true
    */
    virtual bool RollFor(double chance) const = 0;


    /*!
    * \brief Rolls the gap between two agents
    *
    * \details Rolls exponentially for the gap between two cars
    *
    *
    * @param[in]    carRatePerSecond
    * @return       Gap between cars in s
    */
    virtual double RollGapBetweenCars(double carRatePerSecond) const = 0;

    /*!
    * \brief Rolls the gap extension between two agents
    *
    * \details Rolls uniform distributed the gap extension between two cars
    *           This is used in case agents are platooning.
    *
    * @param[in]    extension       Average extension
    * @return       Gap extension in s
    */
    virtual double RollGapExtension(double extension) const = 0;

    /*!
    * \brief Rolls the velocity of an agent
    *
    * \details Rolls normal distributed the velocity of an agent
    *
    * @param[in]    meanVelocity
    * @param[in]    stdDeviationVelocity
    * @return       Velocity in m/s
    */
    virtual double RollForVelocity(double meanVelocity, double stdDeviationVelocity) const = 0;

    /*!
    * \brief Rolls the a normal distribution
    *
    * @param[in]    mean        mean value
    * @param[in]    stdDev      standard deviation
    * @param[in]    lowerBound  if roll is smaller than lower bound it is set to lowerBoubd
    * @param[in]    upperBound  ir roll is greather than upper bound it is set to upperBound
    * @return       result between lowerBound and upperBound
    */
    virtual double RollForStochasticAttribute(double mean, double stdDev, double lowerBound, double upperBound) const = 0;

    /*!
    * \brief Selects an index for a vector given its size, by using a uniform distribution
    *
    * @param[in]    vectorSize  Size of the vector
    * @return       rolled index
    */
    virtual size_t RollUniformDistributedVectorIndex(size_t vectorSize) const = 0;


    /*!
    * \brief Samples an entry of a probability map
    *
    * \details Determines which entry of the probability map gets selected
    *           according to a uniform distributed roll.
    *
    * @param[in]    probabilities       String probability map
    * @return       Sampled string
    */
    virtual std::string SampleStringProbability(StringProbabilities probabilities) const = 0;

    /*!
    * \brief Samples an entry of a probability map
    *
    * \details Determines which entry of the probability map gets selected
    *           according to a uniform distributed roll.
    *
    * @param[in]    probabilities       Double probability map
    * @return       Sampled double
    */
    virtual double SampleDoubleProbability(DoubleProbabilities probabilities) const = 0;

    /*!
    * \brief Samples an entry of a probability map
    *
    * \details Determines which entry of the probability map gets selected
    *           according to a uniform distributed roll.
    *
    * @param[in]    probabilities       Int probability map
    * @return       Sampled integer
    */
    virtual int SampleIntProbability(IntProbabilities probabilities) const = 0;

    /*!
    * \brief Samples all SpawnPoint parameters
    *
    * \details Samples all parameters for the SpawnPoint defined under the
    *           TrafficConfig according to the specified probabilities
    *
    * @param[in]    trafficConfig       Traffic configuration
    * @param[out]   parameters          Sampled parameters get stored here
    */
    virtual void SampleWorldParameters(EnvironmentConfig& environmentConfig, ParameterInterface* parameters) const = 0;

    /*!
    * \brief Samples all World parameters
    *
    * \details Samples all parameters for the World defined under the
    *           Environment according to the specified probabilities
    *
    * @param[in]    environmentConfig       Environment configuration
    * @param[out]   parameters          Sampled parameters get stored here
    */
    virtual void SampleSpawnPointParameters(TrafficConfig& trafficConfig, ParameterInterface* parameters) const = 0;
};



