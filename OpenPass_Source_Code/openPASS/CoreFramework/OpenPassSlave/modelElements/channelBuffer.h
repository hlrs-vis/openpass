/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  channelBuffer.h
//! @brief This file contains a buffer which stores the data of a channel.
//-----------------------------------------------------------------------------

#ifndef CHANNELBUFFER_H
#define CHANNELBUFFER_H

#include <memory>
#include "modelInterface.h"
#include "log.h"

namespace SimulationSlave
{

class ChannelBuffer
{
public:
    ChannelBuffer(int id) :
        id(id),
        data(nullptr)
    {}
    ChannelBuffer(const ChannelBuffer&) = delete;
    ChannelBuffer(ChannelBuffer&&) = delete;
    ChannelBuffer& operator=(const ChannelBuffer&) = delete;
    ChannelBuffer& operator=(ChannelBuffer&&) = delete;
    virtual ~ChannelBuffer();

    int GetId();
    void SetData(const std::shared_ptr<SignalInterface const> &data);
    std::shared_ptr<SignalInterface const> &GetDataPtr();
    void ReleaseData();

private:
    int id;
    std::shared_ptr<SignalInterface const> data; // to be released explicitely by ReleaseData()
};

inline ChannelBuffer::~ChannelBuffer()
{
    LOG_INTERN(LogLevel::DebugCore) << "destroying channel buffer: " << id;
}

inline int ChannelBuffer::GetId()
{
    return id;
}

inline void ChannelBuffer::SetData(const std::shared_ptr<SignalInterface const> &data)
{
    this->data = data;
}

inline std::shared_ptr<SignalInterface const> &ChannelBuffer::GetDataPtr()
{
    return data;
}

inline void ChannelBuffer::ReleaseData()
{
    data = std::move(std::shared_ptr<SignalInterface const>());
}

} // namespace SimulationSlave

#endif // CHANNELBUFFER_H
