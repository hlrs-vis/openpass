/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

//-----------------------------------------------------------------------------
//! @file  ProjectInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef PROJECTINTERFACE_H
#define PROJECTINTERFACE_H

#include "openPASS/ServiceInterface.h"

#include <QDir>
#include <QObject>
#include <QString>

static ServiceManagerInterface::ID const ProjectInterfaceID =
        ServiceManagerInterfaceID("openPASS.Project.ProjectInterface");

class ProjectInterface : public QObject,
        public ServiceInterface<ProjectInterface, ProjectInterfaceID>
{
    Q_OBJECT

public:
    using Directory = QString;
    using Library = QString;
    using Result = QString;
    using AgentConfig = QString;
    using RunConfig = QString;
    using SceneryConfig = QString;
    using LogMaster = QString;
    using LogSlave = QString;
    using LogLevel = unsigned int;
    using Slave = QString;

public:
    explicit ProjectInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~ProjectInterface() = default;

Q_SIGNALS: // update actions from the Menu-bar
    void cleared() const;
    void loaded() const;
    void saved() const;

Q_SIGNALS: // update actions from the Browse Buttons
    void update();

public:
    virtual bool clear() = 0;
    virtual bool load(QString const &filepath) = 0;
    virtual bool save(QString const &filepath) const = 0;

public:
    virtual void setDirectory(ProjectInterface::Directory const & directory) = 0;
    virtual ProjectInterface::Directory getDirectory() const = 0;

public:
    virtual bool setLibraryPath(ProjectInterface::Library const &library) = 0;
    virtual ProjectInterface::Library getLibraryPath() const = 0;
    virtual QDir getLibrary() = 0;

public:
    virtual bool setResultPath(ProjectInterface::Result const &result) = 0;
    virtual ProjectInterface::Result getResultPath() const = 0;

public:
    virtual bool setAgentConfigFile(ProjectInterface::AgentConfig const &agentConfig) = 0;
    virtual ProjectInterface::AgentConfig getAgentConfigFile() const = 0;

public:
    virtual bool setRunConfigFile(ProjectInterface::RunConfig const &runConfig) = 0;
    virtual ProjectInterface::RunConfig getRunConfigFile() const = 0;

public:
    virtual bool setSceneryConfigFile(ProjectInterface::SceneryConfig const &sceneryConfig) = 0;
    virtual ProjectInterface::SceneryConfig getSceneryConfigFile() const = 0;

public:
    virtual bool setLogMaster(ProjectInterface::LogMaster const &logMaster) = 0;
    virtual ProjectInterface::LogMaster getLogMaster() const = 0;

public:
    virtual bool setLogSlave(ProjectInterface::LogSlave const &logSlave) = 0;
    virtual ProjectInterface::LogSlave getLogSlave() const = 0;

public:
    virtual bool setLogLevel(ProjectInterface::LogLevel const &logLevel) = 0;
    virtual ProjectInterface::LogLevel getLogLevel() const = 0;

public:
    virtual bool setSlaveExe(ProjectInterface::Slave const &slave) = 0;
    virtual ProjectInterface::Slave getSlaveExe() const = 0;

public:
    virtual bool getProjectStatus() const = 0;
};

#endif // PROJECTINTERFACE_H
