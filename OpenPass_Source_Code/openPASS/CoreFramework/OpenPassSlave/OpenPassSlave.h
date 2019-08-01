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

/** \addtogroup OpenPassSlave
* @{
*
* \section collDesc Detailed Description
* This is the slave module of the framework and it contains the main entry point of
* the simulation process.
*
* It is started by a master module with or without arguments. Following this the slave reads
* all configuration files and sets up the program parameters and the framework
* infrastructure.
* After that the simulation starts and stores its results in a prespecified file.
*
* \section Input arguments
* arguments | meaning
* -----|---------
* -libraryPath | Path to all libraries. Working directory by default.
* -observationResultPath | Path where the simulation results are stored. Working directory by default.
* -agentConfiguration | Path and name of agent configuration file. "agentConfiguration.xml" by default.
* -sceneryConfiguration | Path and name of scenery configuration file. "sceneryConfiguration.xml" by default.
* -runConfiguration | Path and name of run configuration file. "runConfiguration.xml" by default.
* -logFile | Path and name were the log is stored. Working directory and "logSlave.txt" by default.
* -logLevel | Level at which the log messages are stored. "0" by default.
*
* \section Output files
* file | description
* -----|---------
* logSlave.txt | Default name for file containing all log messages (depending on the logLevel). Path and name are specified in slaveConfig.xml.
* simulationOutput.xml | This file contains the simulation results. Path is specified in frameworkConfig.xml and name in runConfig.xml.
*
* @} */
