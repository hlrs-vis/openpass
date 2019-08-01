/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/** \addtogroup OpenPassMaster
* @{
*
* \section masterDesc Detailed Description
* This is the master module of the framework and it contains the main entry point
* of the framework.
*
* These executeable is started by the GUI with or without an argument to set the
* masterConfig.xml file.
* It reads the masterConfig.xml and saves all options and information.
* Following this a slave executable is started with the options and information
* as agrument.
*
* \section Input arguments
* | arguments | meaning                                                                                 |
* |-----------|-----------------------------------------------------------------------------------------|
* | --config  | Path to master configuration file. Working directory and "masterConfig.xml" by default. |
*
* \section Output files
* | file               | description                                                                                                                     |
* |--------------------|---------------------------------------------------------------------------------------------------------------------------------|
* | OpenPassMaster.log | Default name for file containing all log messages (depending on the logLevel). Path and name are specified in masterConfig.xml. |
*
* @} */
