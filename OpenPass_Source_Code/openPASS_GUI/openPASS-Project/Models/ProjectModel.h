/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include "openPASS-Project/ProjectInterface.h"

#include <QDir>

class ProjectModel : public ProjectInterface
{
    Q_OBJECT

public:
    explicit ProjectModel(QObject *parent = nullptr);
    virtual ~ProjectModel() = default;

public:
    virtual bool clear() override;
    virtual bool load(QString const &filepath) override;
    virtual bool save(QString const &filepath) const override;
    virtual bool simulate();

public:
    virtual void setDirectory(ProjectInterface::Directory const & directory) override;
    virtual ProjectInterface::Directory getDirectory() const override;

public:
    virtual bool setLibraryPath(ProjectInterface::Library const &library) override;
    virtual ProjectInterface::Library getLibraryPath() const override;
    virtual QDir getLibrary() override;

public:
    virtual bool setResultPath(ProjectInterface::Result const &result) override;
    virtual ProjectInterface::Result getResultPath() const override;

public:
    virtual bool setAgentConfigFile(ProjectInterface::AgentConfig const &agentConfig) override;
    virtual ProjectInterface::AgentConfig getAgentConfigFile() const override;

public:
    virtual bool setRunConfigFile(ProjectInterface::RunConfig const &runConfig) override;
    virtual ProjectInterface::RunConfig getRunConfigFile() const override;

public:
    virtual bool setSceneryConfigFile(ProjectInterface::SceneryConfig const &sceneryConfig) override;
    virtual ProjectInterface::SceneryConfig getSceneryConfigFile() const override;

public:
    virtual bool setLogMaster(ProjectInterface::LogMaster const &logMaster) override;
    virtual ProjectInterface::LogMaster getLogMaster() const override;

public:
    virtual bool setLogSlave(ProjectInterface::LogSlave const &logSlave) override;
    virtual ProjectInterface::LogSlave getLogSlave() const override;

public:
    virtual bool setLogLevel(ProjectInterface::LogLevel const &logLevel) override;
    virtual ProjectInterface::LogLevel getLogLevel() const override;

public:
    virtual bool setSlaveExe(ProjectInterface::Slave const &slave) override;
    virtual ProjectInterface::Slave getSlaveExe() const override;

public:
    virtual bool getProjectStatus() const override;

protected:
    static ProjectInterface::Directory const DefaultDirectory;
    static ProjectInterface::Library const DefaultLibrary;
    static ProjectInterface::Result const DefaultResult;
    static ProjectInterface::AgentConfig const DefaultAgent;
    static ProjectInterface::RunConfig const DefaultRunConfig;
    static ProjectInterface::SceneryConfig const DefaultScenery;
    static ProjectInterface::LogMaster const DefaultLogMaster;
    static ProjectInterface::LogSlave const DefaultLogSlave;

protected:
    ProjectInterface::Directory _directory;
    ProjectInterface::Library _library;
    ProjectInterface::Result _result;
    ProjectInterface::AgentConfig _agent;
    ProjectInterface::RunConfig _runConfig;
    ProjectInterface::SceneryConfig _scenery;
    ProjectInterface::LogMaster _logMaster;
    ProjectInterface::LogSlave _logSlave;
    ProjectInterface::LogLevel _logLevel;
    ProjectInterface::Slave _slave;
};

#endif // PROJECTMODEL_H
