/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#pragma once

#include <map>
#include <string>

enum class ComponentState
{
    Acting,
    Armed,
    Disabled,
    Undefined
};

const std::map<std::string, ComponentState> ComponentStateMapping = { { "Acting",   ComponentState::Acting   },
                                                                      { "Armed",    ComponentState::Armed    },
                                                                      { "Disabled", ComponentState::Disabled } };

//-----------------------------------------------------------------------------
//! This interface provides access to common signal operations
//-----------------------------------------------------------------------------
class SignalInterface
{
public:
    SignalInterface() = default;
    SignalInterface(const SignalInterface &) = delete;
    SignalInterface(SignalInterface &&) = delete;
    SignalInterface &operator=(const SignalInterface &) = delete;
    SignalInterface &operator=(SignalInterface &&) = delete;
    virtual ~SignalInterface() = default;

    //-----------------------------------------------------------------------------
    //! String conversion of signal
    //-----------------------------------------------------------------------------
    explicit virtual operator std::string() const = 0;

    int senderID = -1;
private:
    friend std::ostream &operator<<(std::ostream &,
                                    const SignalInterface &);
};

//-----------------------------------------------------------------------------
//! This interface provides access to a component's state
//-----------------------------------------------------------------------------
class ComponentStateSignalInterface : public SignalInterface
{
public:
    ComponentStateSignalInterface() = default;
    ComponentStateSignalInterface(const ComponentStateSignalInterface&) = delete;
    ComponentStateSignalInterface(ComponentStateSignalInterface&&) = delete;
    ComponentStateSignalInterface& operator=(const ComponentStateSignalInterface&) = delete;
    ComponentStateSignalInterface& operator=(ComponentStateSignalInterface&&) = delete;
    virtual ~ComponentStateSignalInterface() = default;

    ComponentState componentState;
};
