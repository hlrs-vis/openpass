/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentItemModel.h"

#include "Models/SystemComponentInputMapModel.h"
#include "Models/SystemComponentMapModel.h"
#include "Models/SystemComponentOutputMapModel.h"
#include "Models/SystemComponentParameterMapModel.h"
#include "Models/SystemComponentScheduleModel.h"

SystemComponentItemModel::SystemComponentItemModel(ComponentItemInterface const * const component,
                                                   SystemComponentItemInterface::Position const & position,
                                                   QObject * const parent)
    : SystemComponentItemInterface(parent)
    , component(component)
    , title(component->getTitle())
    , schedule(new SystemComponentScheduleModel(component->getSchedule(), this))
    , parameters(new SystemComponentParameterMapModel(component->getParameters(), this))
    , inputs(new SystemComponentInputMapModel(component->getInputs(), this))
    , outputs(new SystemComponentOutputMapModel(component->getOutputs(), this))
    , position(position)
{
}

SystemComponentItemModel::SystemComponentItemModel(SystemComponentItemModel const & component,
                                                   SystemComponentItemInterface::Position const & position,
                                                   QObject * const parent)
    : SystemComponentItemModel(component.component, position, parent)
{
    title = component.title;
    schedule->setPriority(component.schedule->getPriority());
    schedule->setOffset(component.schedule->getOffset());
    schedule->setCycle(component.schedule->getCycle());
    schedule->setResponse(component.schedule->getResponse());
    for (SystemComponentParameterItemInterface const * const parameter : *component.parameters)
    {
        parameters->getItem(parameter->getID())->setValue(parameter->getValue());
    }
}

bool SystemComponentItemModel::duplicate(SystemComponentItemInterface::Position const & position)
{
    SystemComponentMapModel * const components =
            qobject_cast<SystemComponentMapModel * const>(parent());
    return ((components) ? components->duplicate(this, position) : false);
}

bool SystemComponentItemModel::remove()
{
    SystemComponentMapModel * const components =
            qobject_cast<SystemComponentMapModel * const>(parent());
    return ((components) ? components->remove(components->getID(this)) : false);
}

SystemComponentItemInterface::ID SystemComponentItemModel::getID() const
{
    SystemComponentMapModel const * const components =
            qobject_cast<SystemComponentMapModel const * const>(parent());
    return ((components) ? components->getID(const_cast<SystemComponentItemModel *>(this))
                         : SystemComponentItemInterface::ID());
}

SystemComponentItemInterface::Library SystemComponentItemModel::getLibrary() const
{
    return component->getName();
}

SystemComponentItemInterface::Type SystemComponentItemModel::getType() const
{
    return component->getType();
}

bool SystemComponentItemModel::setTitle(SystemComponentItemInterface::Title const & _title)
{
    title = _title;
    Q_EMIT modifiedTitle();
    return true;
}

SystemComponentItemInterface::Title SystemComponentItemModel::getTitle() const
{
    return title;
}

SystemComponentItemInterface::Schedule * SystemComponentItemModel::getSchedule() const
{
    return schedule;
}

SystemComponentItemInterface::ParameterMap * SystemComponentItemModel::getParameters() const
{
    return parameters;
}

SystemComponentItemInterface::InputMap * SystemComponentItemModel::getInputs() const
{
    return inputs;
}

SystemComponentItemInterface::OutputMap * SystemComponentItemModel::getOutputs() const
{
    return outputs;
}

bool SystemComponentItemModel::setPosition(SystemComponentItemInterface::Position const & _position)
{
    position = _position;
    Q_EMIT modifiedPosition();
    return true;
}

SystemComponentItemInterface::Position SystemComponentItemModel::getPosition() const
{
    return position;
}

// Get access to the components contained in the parent map
SystemComponentMapModel * SystemComponentItemModel::getComponents() const
{
    SystemComponentMapModel * components = nullptr;
    if (parent())
    {
        components = qobject_cast<SystemComponentMapModel * const>(parent());
    }
    return components;
}
