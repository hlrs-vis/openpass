/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTITEMMODEL_H
#define COMPONENTITEMMODEL_H

#include "openPASS-Component/ComponentItemInterface.h"

class ComponentItemModel : public ComponentItemInterface
{
    Q_OBJECT

public:
    explicit ComponentItemModel(QObject * const parent = nullptr);
    virtual ~ComponentItemModel() = default;

public:
    virtual bool clear() override;
    virtual bool load(QString const & filepath) override;
    virtual bool save(QString const & filepath) const override;

public:
    virtual ComponentItemInterface::Name getName() const override;

public:
    virtual bool setType(ComponentItemInterface::Type const & type) override;
    virtual ComponentItemInterface::Type getType() const override;

public:
    virtual bool setTitle(ComponentItemInterface::Title const & title) override;
    virtual ComponentItemInterface::Title getTitle() const override;

public:
    virtual ComponentItemInterface::Schedule * getSchedule() const override;
    virtual ComponentItemInterface::ParameterMap * getParameters() const override;
    virtual ComponentItemInterface::InputMap * getInputs() const override;
    virtual ComponentItemInterface::OutputMap * getOutputs() const override;

protected:
    static ComponentItemInterface::Type const DefaultType;
    static ComponentItemInterface::Title const DefaultTitle;

protected:
    ComponentItemInterface::Type type;
    ComponentItemInterface::Title title;
    ComponentItemInterface::Schedule * const schedule;
    ComponentItemInterface::ParameterMap * const parameters;
    ComponentItemInterface::InputMap * const inputs;
    ComponentItemInterface::OutputMap * const outputs;
};

#endif // COMPONENTITEMMODEL_H
