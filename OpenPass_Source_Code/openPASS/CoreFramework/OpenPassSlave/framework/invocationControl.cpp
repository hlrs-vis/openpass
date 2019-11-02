/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "invocationControl.h"

namespace  SimulationSlave {

bool InvocationControl::Progress()
{
    if (abort) {
        return false;
    }

    if (retry)
    {
        retry = false;
        retryCount++;
        return (retryCount <= maxRetries);
    }

    retryCount = 0;
    currentInvocation++;
    return (currentInvocation < totalInvocations);
}

void InvocationControl::Abort() {
    abort = true;
}

void InvocationControl::Retry() {
    retry = true;
}

int InvocationControl::CurrentInvocation() const {
    return currentInvocation;
}

bool InvocationControl::GetAbortFlag()
{
    return abort;
}

}
