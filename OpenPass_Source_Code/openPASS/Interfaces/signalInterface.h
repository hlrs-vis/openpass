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
