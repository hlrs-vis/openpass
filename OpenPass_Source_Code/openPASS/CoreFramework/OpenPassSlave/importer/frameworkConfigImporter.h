/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  frameworkConfigImporter.h
//! @brief This file contains the importer of the framework configuration.
//-----------------------------------------------------------------------------

#ifndef FRAMEWORDCONFIGIMPORTER_H
#define FRAMEWORDCONFIGIMPORTER_H

#include <QCoreApplication>
#include <string>

namespace SimulationSlave {

class FrameworkConfig;

//-----------------------------------------------------------------------------
//! Class handles importing an framework configuration.
//-----------------------------------------------------------------------------
class FrameworkConfigImporter
{
public:
    FrameworkConfigImporter() = default;
    FrameworkConfigImporter(const FrameworkConfigImporter &) = delete;
    FrameworkConfigImporter(FrameworkConfigImporter &&) = delete;
    FrameworkConfigImporter &operator=(const FrameworkConfigImporter &) = delete;
    FrameworkConfigImporter &operator=(FrameworkConfigImporter &&) = delete;
    virtual ~FrameworkConfigImporter() = default;

    //-----------------------------------------------------------------------------
    //! Imports the framework configuration from the commandline arguments
    //! returns an object with the imported infos.
    //!
    //! @param[in]  argc                argument count
    //! @param[in]  argv                argument values
    //! @return                         Object containing the framework config
    //-----------------------------------------------------------------------------
    static FrameworkConfig *Import(int argc, char *argv[]);
};

} // namespace SimulationSlave

#endif // FRAMEWORDCONFIGIMPORTER_H
