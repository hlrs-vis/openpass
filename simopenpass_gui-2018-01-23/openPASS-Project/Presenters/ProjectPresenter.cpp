/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Presenters/ProjectPresenter.h"

#include "Models/ProjectModel.h"

ProjectPresenter::ProjectPresenter(ProjectModel *projectModel,
                                   QObject *parent)
    : QObject(parent)
    , _projectModel(projectModel)
{
    // Signals-Signals connection declaration for the Menu-bar Actions
    connect(projectModel, &ProjectModel::cleared,
            this, &ProjectPresenter::reseted);
    connect(projectModel, &ProjectModel::loaded,
            this, &ProjectPresenter::loaded);
    connect(projectModel, &ProjectModel::saved,
            this, &ProjectPresenter::saved);

    // Signals-Signals connection declaration for actions from the Browser Buttons
    connect(projectModel, &ProjectModel::update,
            this, &ProjectPresenter::updateView);
}

bool ProjectPresenter::createNewProject()
{
    return _projectModel->clear();
}

bool ProjectPresenter::loadProject(QString const &filepath) const
{
    return _projectModel->load(filepath);
}

bool ProjectPresenter::saveProject(QString const &filepath) const
{
    return _projectModel->save(filepath);
}

bool ProjectPresenter::simulate() const
{
    return _projectModel->simulate();
}

bool ProjectPresenter::setLibraryPath(ProjectInterface::Library const &library)
{
    return _projectModel->setLibraryPath(library);
}

ProjectInterface::Library ProjectPresenter::getLibraryPath() const
{
    return _projectModel->getLibraryPath();
}

bool ProjectPresenter::setResultPath(ProjectInterface::Result const &result)
{
    return _projectModel->setResultPath(result);
}

ProjectInterface::Result ProjectPresenter::getResultPath() const
{
    return _projectModel->getResultPath();
}

bool ProjectPresenter::setLogMaster(ProjectInterface::LogMaster const &logMaster)
{
    return _projectModel->setLogMaster(logMaster);
}

ProjectInterface::LogMaster ProjectPresenter::getLogMaster() const
{
    return _projectModel->getLogMaster();
}

bool ProjectPresenter::setLogSlave(ProjectInterface::LogSlave const &logSlave)
{
    return _projectModel->setLogSlave(logSlave);
}

ProjectInterface::LogSlave ProjectPresenter::getLogSlave() const
{
    return _projectModel->getLogSlave();
}

bool ProjectPresenter::setLogLevel(ProjectInterface::LogLevel const &logLevel)
{
    return _projectModel->setLogLevel(logLevel);
}

ProjectInterface::LogLevel ProjectPresenter::getLogLevel() const
{
    return _projectModel->getLogLevel();
}

bool ProjectPresenter::setSlave(ProjectInterface::Slave const &slave)
{
    return _projectModel->setSlaveExe(slave);
}

ProjectInterface::Slave ProjectPresenter::getSlave() const
{
    return _projectModel->getSlaveExe();
}
