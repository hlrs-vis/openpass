/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include "Common/globalDefinitions.h"
#include "Interfaces/spawnPointInterface.h"
#include "Interfaces/signalInterface.h"

/**
* \brief Respawn Interface.
* \details This interface allows the respawner to be triggered.
*/
class RespawnInterface
{
public:
    RespawnInterface() = default;
    RespawnInterface(const RespawnInterface&) = delete;
    RespawnInterface(RespawnInterface&&) = delete;
    RespawnInterface& operator=(const RespawnInterface&) = delete;
    RespawnInterface& operator=(RespawnInterface&&) = delete;
    virtual ~RespawnInterface() = default;

    virtual void RespawnAgent(int time) = 0;
};


