/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ProjectModel.h"

#include "Models/ProjectXMLLoadModel.h"
#include "Models/ProjectXMLSaveModel.h"

#include <QCoreApplication>
#include <QDir>
#include <QList>
#include <QProcess>
#include <QStandardPaths>
#include <QString>

ProjectInterface::Directory const ProjectModel::DefaultDirectory =
        QCoreApplication::applicationDirPath();
ProjectInterface::Library const ProjectModel::DefaultLibrary =
        QCoreApplication::applicationDirPath();
ProjectInterface::Result const ProjectModel::DefaultResult = QStringLiteral("");
ProjectInterface::AgentConfig const ProjectModel::DefaultAgent = QStringLiteral("");
ProjectInterface::RunConfig const ProjectModel::DefaultRunConfig = QStringLiteral("");
ProjectInterface::SceneryConfig const ProjectModel::DefaultScenery = QStringLiteral("");
ProjectInterface::LogMaster const ProjectModel::DefaultLogMaster = QStringLiteral("");
ProjectInterface::LogSlave const ProjectModel::DefaultLogSlave = QStringLiteral("");

ProjectModel::ProjectModel(QObject *parent)
    : ProjectInterface(parent)
    , _directory(DefaultDirectory)
    , _library(DefaultLibrary)
    , _result(DefaultResult)
    , _agent(DefaultAgent)
    , _runConfig(DefaultRunConfig)
    , _scenery(DefaultScenery)
    , _logMaster(DefaultLogMaster)
    , _logSlave(DefaultLogSlave)
    , _logLevel(0)
{
    // Get the path of the application "OpenPass"
    QDir const root = QDir(QCoreApplication::applicationDirPath());

    // Set a default path to the SLAVE
    _slave = root.canonicalPath().append("/OpenPassSlave.exe");

    // Lookup Projects directory
    QList<QDir> const directories = {
        root.filePath(QStringLiteral("Projects")),
        root.filePath(QStringLiteral("Projects.lnk")),
    };
    for (QDir const & directory : directories)
    {
        if (directory.exists())
        {
            _directory = directory.canonicalPath();
        }
    }

    // Lookup Components library
    QList<QDir> const libraries = {
        root.filePath(QStringLiteral("Components")),
        root.filePath(QStringLiteral("Components.lnk")),
    };
    for (QDir const & library : libraries)
    {
        if (library.exists())
        {
            _library = library.canonicalPath();
        }
    }
}

bool ProjectModel::clear()
{
    setLibraryPath(DefaultLibrary);
    setResultPath(DefaultResult);
    setAgentConfigFile(DefaultAgent);
    setRunConfigFile(DefaultRunConfig);
    setSceneryConfigFile(DefaultScenery);
    setLogMaster(DefaultLogMaster);
    setLogSlave(DefaultLogSlave);
    setLogLevel(0);
    Q_EMIT update();
    return true;
}

bool ProjectModel::load(const QString &filepath)
{
    clear();
    if (ProjectXMLLoadModel::load(filepath, this))
    {
        Q_EMIT loaded();
        return true;
    }
    return false;
}

bool ProjectModel::save(const QString &filepath) const
{
    if (ProjectXMLSaveModel::save(filepath, this))
    {
        Q_EMIT saved();
        return true;
    }
    return false;
}


bool ProjectModel::simulate()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString master = (root.canonicalPath()).append("/OpenPassMaster.exe");
    QProcess *process = new QProcess(this);
    process->start(master);

    return true;
}

void ProjectModel::setDirectory(const ProjectInterface::Directory &directory)
{
    // Update Project directory
    _directory = directory;
}

ProjectInterface::Directory ProjectModel::getDirectory() const
{
    // Get Project directory
    return _directory;
}

bool ProjectModel::setLibraryPath(ProjectInterface::Library const &library)
{
    // Update the path to the directory containing the components
    _library = library;
    Q_EMIT update();
    return true;
}

ProjectInterface::Library ProjectModel::getLibraryPath() const
{
    // Get the path of the directory containing the components
    return _library;
}

QDir ProjectModel::getLibrary()
{
    // Get the directory containing the components
    return QDir(_library);
}

bool ProjectModel::setResultPath(ProjectInterface::Result const &result)
{
    // Update the path to the directory containing the simulation results
    _result = result;
    Q_EMIT update();
    return true;
}

ProjectInterface::Result ProjectModel::getResultPath() const
{
    // Get the path of the directory containing the simulation results
    return _result;
}

bool ProjectModel::setAgentConfigFile(ProjectInterface::AgentConfig const &agentConfig)
{
    // Update the path to the directory containing the Agent Configuration File
    _agent = agentConfig;
    Q_EMIT update();
    return true;
}

ProjectInterface::AgentConfig ProjectModel::getAgentConfigFile() const
{
    // Get the path of the directory containing the Agent Configuration File
    return _agent;
}

bool ProjectModel::setRunConfigFile(ProjectInterface::RunConfig const &runConfig)
{
    // Update the path to the directory containing the Run Configuration File
    _runConfig = runConfig;
    Q_EMIT update();
    return true;
}

ProjectInterface::RunConfig ProjectModel::getRunConfigFile() const
{
    // Get the path of the directory containing the Run Configuration File
    return _runConfig;
}

bool ProjectModel::setSceneryConfigFile(ProjectInterface::SceneryConfig const &sceneryConfig)
{
    // Update the path to the directory containing the Scenery Configuration File
    _scenery = sceneryConfig;
    Q_EMIT update();
    return true;
}

ProjectInterface::SceneryConfig ProjectModel::getSceneryConfigFile() const
{
    // Get the path of the directory containing the Scenery Configuration File
    return _scenery;
}

bool ProjectModel::setLogMaster(ProjectInterface::LogMaster const &logMaster)
{
    // Update the path to the directory containing the Log File of the Master
    _logMaster = logMaster;
    Q_EMIT update();
    return true;
}

ProjectInterface::LogMaster ProjectModel::getLogMaster() const
{
    // Get the path of the directory containing the Log File of the Master
    return _logMaster;
}

bool ProjectModel::setLogSlave(ProjectInterface::LogSlave const &logSlave)
{
    // Update the path to the directory containing the Log File of the Slave
    _logSlave = logSlave;
    Q_EMIT update();
    return true;
}

ProjectInterface::LogSlave ProjectModel::getLogSlave() const
{
    // Get the path of the directory containing the Log File of the Slave
    return _logSlave;
}

bool ProjectModel::setLogLevel(ProjectInterface::LogLevel const &logLevel)
{
    // Set the log level
    _logLevel = logLevel;
    Q_EMIT update();
    return true;
}

ProjectInterface::LogLevel ProjectModel::getLogLevel() const
{
    // Get the log lavel
    return _logLevel;
}

bool ProjectModel::setSlaveExe(ProjectInterface::Slave const &slave)
{
    _slave = slave;
    return true;
}

ProjectInterface::Slave ProjectModel::getSlaveExe() const
{
    // Get the path of the application "OpenPassSlave"
    return _slave;
}

bool ProjectModel::getProjectStatus() const
{
    if ((!_logSlave.isEmpty())&&(!_logMaster.isEmpty())
            &&(!_result.isEmpty())&&(!_agent.isEmpty())
            &&(!_runConfig.isEmpty())&&(!_scenery.isEmpty()))
        return true;
    return false;
}
