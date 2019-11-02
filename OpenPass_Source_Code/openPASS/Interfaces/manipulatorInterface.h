/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <vector>
#include "Interfaces/eventNetworkInterface.h"

class ManipulatorInterface
{
public:
    ManipulatorInterface() = default;
    ManipulatorInterface(const ManipulatorInterface&) = delete;
    ManipulatorInterface(ManipulatorInterface&&) = delete;
    ManipulatorInterface& operator=(const ManipulatorInterface&) = delete;
    ManipulatorInterface& operator=(ManipulatorInterface&&) = delete;
    virtual ~ManipulatorInterface() = default;

    virtual void Trigger(int time) = 0;

    virtual int GetCycleTime() = 0;

    virtual int GetPriority()=0;
};


