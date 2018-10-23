/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentScheduleModel.h"

ComponentScheduleInterface::Offset const ComponentScheduleModel::DefaultOffset = 0;
ComponentScheduleInterface::Cycle const ComponentScheduleModel::DefaultCycle = 1;
ComponentScheduleInterface::Response const ComponentScheduleModel::DefaultResponse = 0;

ComponentScheduleModel::ComponentScheduleModel(QObject * const parent)
    : ComponentScheduleInterface(parent)
    , offset(DefaultOffset)
    , cycle(DefaultCycle)
    , response(DefaultResponse)
{
}

bool ComponentScheduleModel::clear()
{
    setOffset(DefaultOffset);
    setCycle(DefaultCycle);
    setResponse(DefaultResponse);
    Q_EMIT cleared();
    return true;
}

bool ComponentScheduleModel::setOffset(ComponentScheduleInterface::Offset const & _offset)
{
     offset = _offset;
     Q_EMIT modifiedOffset();
     return true;
}

ComponentScheduleInterface::Offset ComponentScheduleModel::getOffset() const
{
    return offset;
}

bool ComponentScheduleModel::setCycle(ComponentScheduleInterface::Cycle const & _cycle)
{
    cycle = _cycle;
    Q_EMIT modifiedCycle();
    return true;
}

ComponentScheduleInterface::Cycle ComponentScheduleModel::getCycle() const
{
    return cycle;
}

bool ComponentScheduleModel::setResponse(ComponentScheduleInterface::Response const & _response)
{
    response = _response;
    Q_EMIT modifiedResponse();
    return true;
}

ComponentScheduleInterface::Response ComponentScheduleModel::getResponse() const
{
    return response;
}
