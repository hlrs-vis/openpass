/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  MasterConfigImporter.h
//! @brief This file contains the importer of the master configuration.
//-----------------------------------------------------------------------------

#pragma once

#include <QFile>
#include <QDomElement>
#include <QString>
#include <vector>
#include "slaveConfig.h"

namespace SimulationMaster {
namespace Configuration {

class MasterConfig;
using SlaveConfigs = std::vector<SlaveConfig>;

class MasterConfigImporter
{
public:
    MasterConfigImporter() = delete;
    MasterConfigImporter(const MasterConfigImporter&) = delete;
    MasterConfigImporter(MasterConfigImporter&&) = delete;
    MasterConfigImporter& operator=(const MasterConfigImporter&) = delete;
    MasterConfigImporter& operator=(MasterConfigImporter&&) = delete;
    virtual ~MasterConfigImporter() = delete;

    //-------------------------------------------------------------------------
    //! \brief Imports the master configuration details from the master
    //! 	   configuration xml
    //! \param[in] filename The name of the master configuration xml file to
    //! 		   parse
    //! \returns A MasterConfig object containing the details of the master
    //! 		 configuration as detailed in the master configuration xml
    //-------------------------------------------------------------------------
    static MasterConfig Import(const QString& filename);

private:
    //-------------------------------------------------------------------------
    //! \brief Parses slave configuration details from the master configuration
    //! 	   xml
    //! \param[in] element The Xml Element containing the slave configuration
    //! 		   data
    //! \returns A SlaveConfig object containing the details of the slave
    //! 		 configuration as detailed in the xml element
    //-------------------------------------------------------------------------
    static SlaveConfig ParseSlaveConfig(const QDomElement& element);

    //-------------------------------------------------------------------------
    //! \brief Converts a filename to an absolute file path, if necessary.
    //! 	   Throws if no file exists at the location specified by the
    //! 	   absolute file path.
    //! \param[in] filename The filename to convert to an absolute file path.
    //! \returns A QString representing the absolute filepath to filename.
    //-------------------------------------------------------------------------
    static QString GetAbsoluteFilePath(const QString& filename);

    //-------------------------------------------------------------------------
    //! \brief Prepares an XML file for parsing using QDomElements
    //! \param[in] xmlFile The QFile object to be parsed with QDomElements
    //! \param[in] file The path of the file xmlFile refers to
    //! \returns A QDomElement referring to the root element of the XML file
    //-------------------------------------------------------------------------
    static QDomElement ReadDocument(QFile& xmlFile, const std::string& file);

    //-------------------------------------------------------------------------
    //! \brief Parses multiple slaves' configuration details from the master
    //! 	   configuration xml
    //! \param[in] element The parent of the xml elements containing the
    //! 		   slaves' configuration data
    //! \returns A std:vector<SlaveConfig> containing the details of the
    //! 		 slaves' configurations as detailed in the xml element
    //-------------------------------------------------------------------------
    static SlaveConfigs ParseSlaveConfigs(const QDomElement& element);
};

} // namespace Configuration
} // namespace SimulationMaster
