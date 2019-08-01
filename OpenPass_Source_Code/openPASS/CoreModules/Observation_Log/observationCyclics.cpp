/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  ObservationCyclics.cpp */
//-----------------------------------------------------------------------------

#include "observationCyclics.h"

std::string ObservationCyclics::GetHeader()
{
    std::string header;
    for(std::map<std::string, std::vector<std::string>>::iterator it = samples.begin(); it != samples.end(); ++it)
    {
        if(it != samples.begin())
        {
            header += ", ";
        }

        header += it->first;
    }
    return header;
}

std::string ObservationCyclics::GetSamplesLine(std::uint32_t timeStepNumber)
{
    std::string sampleLine;
    for (std::map<std::string, std::vector<std::string>>::iterator it = samples.begin(); it != samples.end(); ++it)
    {
        const std::vector<std::string>& values = it->second;

        if (it != samples.begin())
        {
            sampleLine += ", ";
        }

        // not all channels are sampled until end of simulation time
        if (timeStepNumber < values.size())
        {
            sampleLine += values.at(timeStepNumber);
        }
    }

    return sampleLine;
}

void ObservationCyclics::Clear()
{
    timeSteps.clear();
    samples.clear();
}

//-----------------------------------------------------------------------------
//! Called by Observation Log Implementation
//! to log all Sample values in output.xml
//-----------------------------------------------------------------------------
void ObservationCyclics::Insert(
        int time,
        const std::string &key,
        const std::string &value)
{
    if(timeSteps.size() == 0 || timeSteps.back() != time)
    {
        timeSteps.push_back(time);
    }

    if(samples.find(key) == samples.end())
    {
        // key doesn't exist yet
        std::vector<std::string> &values = samples[key];

        // fill up skipped time steps
        for(unsigned int i = 0; i < timeSteps.size() - 1; ++i)
        {
            values.push_back("");
        }

        values.push_back(value);

        return;
    }

    std::vector<std::string> &channel = samples.at(key);

    // fill up skipped time steps (e.g. another agent has been instantiated in between -> inserted new time step in scheduling)
    for(unsigned int i = channel.size(); i < timeSteps.size() - 1; ++i)
    {
        channel.push_back("");
    }

    channel.push_back(value);
}
