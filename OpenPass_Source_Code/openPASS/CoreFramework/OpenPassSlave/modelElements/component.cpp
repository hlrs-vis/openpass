/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <algorithm>
#include <iostream>
#include <cassert>
#include <climits>
#include <stdexcept>

#include "component.h"
#include "CoreFramework/CoreShare/log.h"
#include "modelLibrary.h"
#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "observationModule.h"
#include "channelBuffer.h"

namespace SimulationSlave {

Component::~Component()
{
    LOG_INTERN(LogLevel::DebugCore) << "deleting component " << name;

    // each component is only responsible for its own output channel buffers
    for (auto& item : outputChannelBuffers)
    {
        delete item.second;
    }
    outputChannelBuffers.clear();
}

void Component::SetImplementation(ModelInterface* implementation)
{
    this->implementation = implementation;
}

Agent* Component::GetAgent() const
{
    return agent;
}

std::string Component::GetName() const
{
    return name;
}

bool Component::AddInputLink(Channel* input, int linkId)
{
    if (!inputs.insert({linkId, input}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "input must be unique";
        return false;
    }

    return true;
}

bool Component::AddOutputLink(Channel* output, int linkId)
{
    if (!outputs.insert({linkId, output}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "output must be unique";
        return false;
    }

    return true;
}

void Component::SetObservations(const std::map<int, ObservationModule*>& observations)
{
    for (const auto& item : observations)
    {
        this->observations.insert({item.first, item.second->GetImplementation()});
    }
}

std::map<int, Channel*>& Component::GetInputLinks()
{
    return inputs;
}

std::map<int, Channel*>& Component::GetOutputLinks()
{
    return outputs;
}

const std::map<int, ObservationInterface*>& Component::GetObservations() const
{
    return observations;
}

bool Component::TriggerCycle(int time)
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "no implementation available";
        return false;
    }

    LOG_INTERN(LogLevel::DebugCore) << "enter trigger";

    try
    {
        if (!modelLibrary->Trigger(implementation, time))
        {
            LOG_INTERN(LogLevel::Error) << "an error occurred during trigger (component " << GetName() << ")";
            return false;
        }
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during trigger (component " << GetName() << "): " << ex.what();
        return false;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during trigger (component " << GetName() << ")";
        return false;
    }

    LOG_INTERN(LogLevel::DebugCore) << "exit trigger";

    return true;
}

bool Component::AcquireOutputData(int linkId, int time)
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "no implementation available";
        return false;
    }

    ChannelBuffer* buffer;
    try
    {
        buffer = outputChannelBuffers.at(linkId);
    }
    catch (const std::out_of_range&)
    {
        LOG_INTERN(LogLevel::Error) << "no channel buffer given for output link: " << linkId;
        return false;
    }

    std::shared_ptr<SignalInterface const> data;

    LOG_INTERN(LogLevel::DebugCore) << "enter update output";

    try
    {
        if (!modelLibrary->UpdateOutput(implementation, linkId, data, time))
        {
            LOG_INTERN(LogLevel::Error) << "an error occurred during update of output data for link " << linkId << " (component " <<
                                        GetName() << ")";
            return false;
        }
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during update of output data for link " << linkId << " (component " <<
                                    GetName() << "): " << ex.what();
        return false;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during update of output data for link " << linkId << " (component " <<
                                    GetName() << ")";
        return false;
    }

    LOG_INTERN(LogLevel::DebugCore) << "exit update output";

    buffer->SetData(data);

    return true;
}

bool Component::ReleaseOutputData(int linkId)
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "no implementation available";
        return false;
    }

    ChannelBuffer* buffer;
    try
    {
        buffer = outputChannelBuffers.at(linkId);
    }
    catch (const std::out_of_range&)
    {
        LOG_INTERN(LogLevel::Error) << "no channel buffer given for output link: " << linkId;
        return false;
    }

    // release resource
    buffer->ReleaseData();

    return true;
}

bool Component::UpdateInputData(int linkId, int time)
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "no implementation available";
        return false;
    }

    ChannelBuffer* buffer;
    try
    {
        buffer = inputChannelBuffers.at(linkId);
    }
    catch (const std::out_of_range&)
    {
        LOG_INTERN(LogLevel::Error) << "no channel buffer given for input link: " << linkId;
        return false;
    }

    LOG_INTERN(LogLevel::DebugCore) << "enter update input";

    try
    {
        if (!modelLibrary->UpdateInput(implementation, linkId, buffer->GetDataPtr(), time))
        {
            LOG_INTERN(LogLevel::Error) << "an error occurred during update of input data for link " << linkId << " (component " <<
                                        GetName() << ")";
            return false;
        }
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during update of input data for link " << linkId << " (component " <<
                                    GetName() << "): " << ex.what();
        return false;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during update of input data for link " << linkId << " (component " <<
                                    GetName() << ")";
        return false;
    }

    LOG_INTERN(LogLevel::DebugCore) << "exit update input";

    return true;
}

ChannelBuffer* Component::CreateOutputBuffer(int linkId)
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "no implementation available";
        return nullptr;
    }

    LOG_INTERN(LogLevel::DebugCore) << "create buffer for output link: " << linkId;

    ChannelBuffer* buffer = new (std::nothrow) ChannelBuffer(linkId);
    if (!buffer)
    {
        return nullptr;
    }

    if (!outputChannelBuffers.insert({linkId, buffer}).second)
    {
        LOG_INTERN(LogLevel::Error) << "links must be unique";
        delete buffer;
        return nullptr;
    }

    return (ChannelBuffer*)buffer;
}

bool Component::SetInputBuffer(int linkId, ChannelBuffer* buffer)
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "no implementation available";
        return false;
    }

    LOG_INTERN(LogLevel::DebugCore) << "set buffer (" << buffer->GetId() << ") for input link: " << linkId;

    if (!inputChannelBuffers.insert({linkId, buffer}).second)
    {
        LOG_INTERN(LogLevel::Error) << "channel buffers must be unique";
        return false;
    }

    return true;
}

bool Component::GetInit() const
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve init flag";
        return false;
    }

    try
    {
        return implementation->GetInit();
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve init flag: " << ex.what();
        return false;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve init flag";
        return false;
    }
}

int Component::GetPriority() const
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve priority";
    }

    try
    {
        return implementation->GetPriority();
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve priority: " << ex.what();
        return std::numeric_limits<int>::min();
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve priority";
        return std::numeric_limits<int>::min();
    }
}

int Component::GetOffsetTime() const
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve offset time";
    }

    try
    {
        return implementation->GetOffsetTime();
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve offset time: " << ex.what();
        return 0;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve offset time";
        return 0;
    }
}

int Component::GetResponseTime() const
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve response time";
    }

    try
    {
        return implementation->GetResponseTime();
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve response time: " << ex.what();
        return 0;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve response time";
        return 0;
    }
}

int Component::GetCycleTime() const
{
    if (!implementation)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve cycle time";
    }

    try
    {
        return implementation->GetCycleTime();
    }
    catch (std::runtime_error const& ex)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve cycle time: " << ex.what();
        return 0;
    }
    catch (...)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve cycle time";
        return 0;
    }
}

bool Component::SetModelLibrary(ModelLibrary* modelLibrary)
{
    if (this->modelLibrary)
    {
        return false;
    }

    this->modelLibrary = modelLibrary;
    return true;
}

bool Component::ReleaseFromLibrary()
{
    assert(nullptr != modelLibrary);

    return modelLibrary->ReleaseComponent(this);
}

ModelInterface* Component::GetImplementation() const
{
    return implementation;
}

} // namespace SimulationSlave
