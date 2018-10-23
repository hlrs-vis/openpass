/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "ProjectXMLBaseModel.h"

QString const ProjectXMLBaseModel::KeyFrameworkConfiguration = QStringLiteral("FrameworkConfiguration");
QString const ProjectXMLBaseModel::KeyLibraryPath = QStringLiteral("LibraryPath");
QString const ProjectXMLBaseModel::KeyResultPath = QStringLiteral("ResultPath");
QString const ProjectXMLBaseModel::KeyAgentConfigFile = QStringLiteral("AgentConfigFile");
QString const ProjectXMLBaseModel::KeyRunConfigFile = QStringLiteral("RunConfigFile");
QString const ProjectXMLBaseModel::KeySceneryConfigFile = QStringLiteral("SceneryConfigFile");
QString const ProjectXMLBaseModel::KeyLogFileMaster = QStringLiteral("LogFileMaster");
QString const ProjectXMLBaseModel::KeyLogFileSlave = QStringLiteral("LogFileSlave");
QString const ProjectXMLBaseModel::KeyLogLevel = QStringLiteral("LogLevel");
QString const ProjectXMLBaseModel::KeySlavePath = QStringLiteral("SlavePath");

QList<QString> const ProjectXMLBaseModel::KeyListFrameworkConfig = {
    ProjectXMLBaseModel::KeyLibraryPath,
    ProjectXMLBaseModel::KeyResultPath,
    ProjectXMLBaseModel::KeyAgentConfigFile,
    ProjectXMLBaseModel::KeyRunConfigFile,
    ProjectXMLBaseModel::KeySceneryConfigFile,
    ProjectXMLBaseModel::KeyLogFileMaster,
    ProjectXMLBaseModel::KeyLogFileSlave,
    ProjectXMLBaseModel::KeyLogLevel,
    ProjectXMLBaseModel::KeySlavePath,
};
