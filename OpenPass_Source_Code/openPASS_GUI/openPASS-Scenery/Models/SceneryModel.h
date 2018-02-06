/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SCENERYMODEL_H
#define SCENERYMODEL_H

#include "openPASS-Project/ProjectInterface.h"

#include <QObject>
#include <QString>

class QIODevice;

class SceneryModel : public QObject
{
    Q_OBJECT

public:
    using Source = QString;
    using Agents = QString;

public:
    explicit SceneryModel(ProjectInterface * const project, QObject *parent = nullptr);
    virtual ~SceneryModel() = default;

public:
    virtual bool setSceneryConfigFile(ProjectInterface::SceneryConfig const &sceneryPath);
    virtual ProjectInterface::SceneryConfig getSceneryConfigFile() const;

public:
    virtual SceneryModel::Source getScenerySource() const;
    virtual SceneryModel::Agents getSceneryAgents() const;

public:
    virtual bool loadFile(QString const & filepath);
    virtual bool readFile(QIODevice * const device);

public:
    virtual bool initialize();

protected:
    ProjectInterface * const _project;

protected:
    static QString const EmptyValue;

protected:
    static QString const PCM;
    static QString const OpenDRIVE;

protected:
    SceneryModel::Source _source;
    SceneryModel::Agents _agents;

Q_SIGNALS: // update actions from the Browse Buttons
    void update();
};

#endif // SCENERYMODEL_H
