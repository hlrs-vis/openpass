/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMMODEL_H
#define SYSTEMMODEL_H

#include "openPASS-System/SystemInterface.h"

class ComponentInterface;
class ProjectInterface;
class SystemComponentManagerModel;

class SystemModel : public SystemInterface
{
    Q_OBJECT

public:
    explicit SystemModel(ComponentInterface * const component,
                         ProjectInterface * const project,
                         QObject * const parent = nullptr);
    virtual ~SystemModel() = default;

public:
    virtual bool clear() override;
    virtual bool load(QString const & filepath) override;
    virtual bool load(QIODevice * const device) override;
    virtual bool save(QString const & filepath) const override;
    virtual bool save(QIODevice * const device) const override;

public:
    virtual SystemComponentManagerInterface * getComponents() const override;
    virtual SystemMapInterface * getSystems() const override;

protected:
    SystemComponentManagerInterface * const components;
    ProjectInterface * const _project;
    SystemMapInterface * const systems;
};

#endif // SYSTEMMODEL_H
