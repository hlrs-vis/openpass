/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
