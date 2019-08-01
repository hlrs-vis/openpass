/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "tasks.h"

//-----------------------------------------------------------------------------
/** \file  Tasks.cpp */
//-----------------------------------------------------------------------------

namespace SimulationSlave {
namespace Scheduling {

void Tasks::AddTask(const TaskItem& newTask)
{
    tasks.insert(newTask);
}

void Tasks::DeleteTasks(int agentId)
{
    auto it = tasks.begin();
    while (it != tasks.end())
    {
        if ((*it).agentId == agentId)
        {
            it = tasks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool TaskItem::operator<(const TaskItem& rhs) const
{
    return (priority > rhs.priority ||
            ((priority == rhs.priority) && (taskType < rhs.taskType)));
}

bool TaskItem::operator==(const TaskItem& rhs) const
{
    return (priority == rhs.priority &&
            cycletime == rhs.cycletime &&
            agentId == rhs.agentId &&
            delay == rhs.delay &&
            taskType == rhs.taskType);
}

} // namespace Scheduling
} // namespace SimulationSlave
