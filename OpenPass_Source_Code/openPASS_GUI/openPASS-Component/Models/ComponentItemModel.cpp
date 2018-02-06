/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentItemModel.h"

#include "Models/ComponentInputMapModel.h"
#include "Models/ComponentMapModel.h"
#include "Models/ComponentOutputMapModel.h"
#include "Models/ComponentParameterMapModel.h"
#include "Models/ComponentScheduleModel.h"
#include "Models/ComponentXMLLoadModel.h"
#include "Models/ComponentXMLSaveModel.h"

ComponentItemInterface::Type const ComponentItemModel::DefaultType = ComponentItemInterface::Type::Algorithm;
ComponentItemInterface::Title const ComponentItemModel::DefaultTitle = QStringLiteral("");

ComponentItemModel::ComponentItemModel(QObject * const parent)
    : ComponentItemInterface(parent)
    , type(DefaultType)
    , title(DefaultTitle)
    , schedule(new ComponentScheduleModel(this))
    , parameters(new ComponentParameterMapModel(this))
    , inputs(new ComponentInputMapModel(this))
    , outputs(new ComponentOutputMapModel(this))
{
}

bool ComponentItemModel::clear()
{
    setType(DefaultType);
    setTitle(DefaultTitle);
    schedule->clear();
    parameters->clear();
    inputs->clear();
    outputs->clear();
    Q_EMIT cleared();
    return true;
}

bool ComponentItemModel::load(QString const & filepath)
{
    clear();
    if (ComponentXMLLoadModel::load(filepath, this))
    {
        Q_EMIT loaded();
        return true;
    }
    return false;
}

bool ComponentItemModel::save(QString const & filepath) const
{
    if (ComponentXMLSaveModel::save(filepath, this))
    {
        Q_EMIT saved();
        return true;
    }
    return false;
}

ComponentItemInterface::Name ComponentItemModel::getName() const
{
    ComponentMapModel const * const map =
            qobject_cast<ComponentMapModel const * const>(parent());
    return ((map) ? map->getName(const_cast<ComponentItemModel *>(this))
                  : ComponentItemInterface::Name());
}

bool ComponentItemModel::setType(ComponentItemInterface::Type const & _type)
{
    type = _type;
    Q_EMIT modifiedType();
    return true;
}

ComponentItemInterface::Type ComponentItemModel::getType() const
{
    return type;
}

bool ComponentItemModel::setTitle(ComponentItemInterface::Title const & _title)
{
    title = _title;
    Q_EMIT modifiedTitle();
    return true;
}

ComponentItemInterface::Title ComponentItemModel::getTitle() const
{
    return title;
}

ComponentItemInterface::Schedule * ComponentItemModel::getSchedule() const
{
    return schedule;
}

ComponentItemInterface::ParameterMap * ComponentItemModel::getParameters() const
{
    return parameters;
}

ComponentItemInterface::InputMap * ComponentItemModel::getInputs() const
{
    return inputs;
}

ComponentItemInterface::OutputMap * ComponentItemModel::getOutputs() const
{
    return outputs;
}
