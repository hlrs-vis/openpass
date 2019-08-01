/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

namespace SimulationSlave {

/// Helper class for simplifaction of invocations in a simulation (progress, retry, abort)
///
/// Set up with number of total invocations allowed.
///
/// Typical usage: generate instance of class and then while(instance.Progress){ do stuff };
/// Whenever a invocation failes, call retry. The instance then progresses up to maxRetries
/// without increasing invocation counter. If an error occurs, call abort.
/// Progress will then always return false.
class InvocationControl
{
    static constexpr int maxRetries = 5;
    const int totalInvocations;
    int currentInvocation = -1;
    bool abort = false;
    bool retry = false;
    int retryCount = 0;

public:
    InvocationControl(int invocations) : totalInvocations{invocations} { }

    /// Increase current invocation (unless retry or abort)
    /// \return true as long as current invocation  < total invocations
    bool Progress();

    /// Stop progress
    void Abort();

    /// Call retry to repeat current invocation without progressing
    void Retry();

    /// Get current invocation number
    /// \return -1 at start, 0 after first call of Progress (use in while)
    int CurrentInvocation() const;

    /// Returns abort flag
    /// \return true if simulation is aborted
    bool GetAbortFlag();

    // uncopyable class
    InvocationControl(const InvocationControl&) = delete;
    InvocationControl(InvocationControl&&) = delete;
    InvocationControl& operator=(const InvocationControl&) = delete;
    InvocationControl& operator=(InvocationControl&&) = delete;
};

}
