/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTITEMMODEL_H
#define SYSTEMCOMPONENTITEMMODEL_H

#include "openPASS-System/SystemComponentItemInterface.h"
#include "Models/SystemComponentMapModel.h"

class SystemComponentItemModel : public SystemComponentItemInterface
{
    Q_OBJECT

public:
    explicit SystemComponentItemModel(ComponentItemInterface const * const component,
                                      SystemComponentItemInterface::Position const & position
                                      = SystemComponentItemInterface::Position(0, 0),
                                      QObject * const parent = nullptr);
    explicit SystemComponentItemModel(SystemComponentItemModel const & component,
                                      SystemComponentItemInterface::Position const & position
                                      = SystemComponentItemInterface::Position(0, 0),
                                      QObject * const parent = nullptr);
    virtual ~SystemComponentItemModel() = default;

public:
    virtual bool duplicate(SystemComponentItemInterface::Position const & position) override;
    virtual bool remove() override;

public:
    virtual SystemComponentItemInterface::ID getID() const override;
    virtual SystemComponentItemInterface::Library getLibrary() const override;
    virtual SystemComponentItemInterface::Type getType() const override;

public:
    virtual bool setTitle(SystemComponentItemInterface::Title const & title) override;
    virtual SystemComponentItemInterface::Title getTitle() const override;

public:
    virtual SystemComponentItemInterface::Schedule * getSchedule() const override;
    virtual SystemComponentItemInterface::ParameterMap * getParameters() const override;
    virtual SystemComponentItemInterface::InputMap * getInputs() const override;
    virtual SystemComponentItemInterface::OutputMap * getOutputs() const override;

public:
    virtual bool setPosition(SystemComponentItemInterface::Position const & position) override;
    virtual SystemComponentItemInterface::Position getPosition() const override;

public:
    virtual SystemComponentMapModel *getComponents() const;

protected:
    ComponentItemInterface const * const component;
    SystemComponentItemInterface::Title title;
    SystemComponentItemInterface::Schedule * const schedule;
    SystemComponentItemInterface::ParameterMap * const parameters;
    SystemComponentItemInterface::InputMap * const inputs;
    SystemComponentItemInterface::OutputMap * const outputs;
    SystemComponentItemInterface::Position position;
};

#endif // SYSTEMCOMPONENTITEMMODEL_H
