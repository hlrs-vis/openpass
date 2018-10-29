/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "callbacks.h"
#include "log.h"

namespace SimulationCommon
{

void Callbacks::Log(CbkLogLevel logLevel,
                    const char *file,
                    int line,
                    const std::string &message) const
{
    if( static_cast<int>(LogLevel::Count) <= static_cast<int>(logLevel) ||
            0 > static_cast<int>(logLevel))
    {
        LOG_INTERN(LogLevel::Warning) << "provided invalid logging level to logging callback";
    }

    LOG_EXTERN(static_cast<LogLevel>(logLevel), file, line) << "CALLBACK: " << message;
}

} // namespace SimulationCommon
