/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentScheduleModel.h"

#include "Models/SystemComponentItemModel.h"
#include "Models/SystemComponentMapModel.h"

SystemComponentScheduleModel::SystemComponentScheduleModel(ComponentScheduleInterface const * const schedule,
                                                           QObject * const parent)
    : SystemComponentScheduleInterface(parent)
    , priority(0)
    , offset(schedule->getOffset())
    , cycle(schedule->getCycle())
    , response(schedule->getResponse())
{
}

bool SystemComponentScheduleModel::setPriority(SystemComponentScheduleInterface::Priority const & _priority)
{
//    if (checkValidPriority(_priority))
//    {
        priority = _priority;
//    }
//    else
//    {
//        priority = 0;
//    }
    Q_EMIT modifiedPriority();
    return true;
}

bool SystemComponentScheduleModel::checkValidPriority(SystemComponentScheduleInterface::Priority const &priority)
{
    // Get access to the components contained in the parent map
    SystemComponentMapModel * components = nullptr;
    if (parent())
    {
        components = (qobject_cast<SystemComponentItemModel * const>(parent()))->getComponents();
    }

    // Check the unicity of the value of the priority compared to the components in the map
    if (components)
    {
        for (SystemComponentItemInterface * const component : * components)
        {
            if (priority == (component->getSchedule()->getPriority()))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

SystemComponentScheduleInterface::Priority SystemComponentScheduleModel::getPriority() const
{
    return priority;
}

bool SystemComponentScheduleModel::setOffset(SystemComponentScheduleInterface::Offset const & _offset)
{
    offset = _offset;
    Q_EMIT modifiedOffset();
    return true;
}

SystemComponentScheduleInterface::Offset SystemComponentScheduleModel::getOffset() const
{
    return offset;
}

bool SystemComponentScheduleModel::setCycle(SystemComponentScheduleInterface::Cycle const & _cycle)
{
    cycle = _cycle;
    Q_EMIT modifiedCycle();
    return true;
}

SystemComponentScheduleInterface::Cycle SystemComponentScheduleModel::getCycle() const
{
    return cycle;
}

bool SystemComponentScheduleModel::setResponse(SystemComponentScheduleInterface::Response const & _response)
{
    response = _response;
    Q_EMIT modifiedResponse();
    return true;
}

SystemComponentScheduleInterface::Response SystemComponentScheduleModel::getResponse() const
{
    return response;
}
