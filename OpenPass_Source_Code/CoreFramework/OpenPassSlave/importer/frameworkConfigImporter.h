/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
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

namespace SimulationSlave
{

class FrameworkConfig;

//-----------------------------------------------------------------------------
//! Class handles importing an framework configuration.
//-----------------------------------------------------------------------------
class FrameworkConfigImporter
{
public:    
    FrameworkConfigImporter() = default;
    FrameworkConfigImporter(const FrameworkConfigImporter&) = delete;
    FrameworkConfigImporter(FrameworkConfigImporter&&) = delete;
    FrameworkConfigImporter& operator=(const FrameworkConfigImporter&) = delete;
    FrameworkConfigImporter& operator=(FrameworkConfigImporter&&) = delete;
    virtual ~FrameworkConfigImporter() = default;

    //-----------------------------------------------------------------------------
    //! Imports the XML framework configuration from the provided file path and returns
    //! an object with the imported infos.
    //!
    //! @param[in]  filename            Path to the file with the XML framework config
    //! @return                         Object containing the framework config
    //-----------------------------------------------------------------------------
    static FrameworkConfig *Import(const std::string &filename);
    //-----------------------------------------------------------------------------
    //! Imports the framework configuration from the commandline arguments of the application
    //! returns an object with the imported infos.
    //!
    //! @param[in]  application         application containing the cmdline arguments
    //! @return                         Object containing the framework config
    //-----------------------------------------------------------------------------
    static FrameworkConfig *Import(QCoreApplication &application);
};

} // namespace SimulationSlave

#endif // FRAMEWORDCONFIGIMPORTER_H
