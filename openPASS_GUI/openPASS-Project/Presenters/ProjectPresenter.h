/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef PROJECTPRESENTER_H
#define PROJECTPRESENTER_H

#include "Models/ProjectModel.h"

#include <QObject>
#include <QString>

class ProjectPresenter : public QObject
{
    Q_OBJECT

public:
    explicit ProjectPresenter(ProjectModel *projectModel,
                              QObject *parent = nullptr);
    virtual ~ProjectPresenter() = default;

public:
    virtual bool createNewProject();
    virtual bool loadProject(QString const &filepath) const;
    virtual bool saveProject(QString const &filepath) const;
    virtual bool simulate() const;

public:
    virtual bool setLibraryPath(ProjectInterface::Library const &library);
    virtual ProjectInterface::Library getLibraryPath() const;

public:
    virtual bool setResultPath(ProjectInterface::Result const &result);
    virtual ProjectInterface::Result getResultPath() const;

public:
    virtual bool setLogMaster(ProjectInterface::LogMaster const &logMaster);
    virtual ProjectInterface::LogMaster getLogMaster() const;

public:
    virtual bool setLogSlave(ProjectInterface::LogSlave const &logSlave);
    virtual ProjectInterface::LogSlave getLogSlave() const;

public:
    virtual bool setLogLevel(ProjectInterface::LogLevel const &logLevel);
    virtual ProjectInterface::LogLevel getLogLevel() const;

public:
    virtual bool setSlave(ProjectInterface::Slave const &slave);
    virtual ProjectInterface::Slave getSlave() const;

private:
    ProjectModel * _projectModel;

Q_SIGNALS: // Actions from the menu bar
    void reseted() const;
    void loaded() const;
    void saved() const;

Q_SIGNALS: // Actions from the Browse Buttons
    void updateView() const;
};

#endif // PROJECTPRESENTER_H
