/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  AgentParser.cpp */
//-----------------------------------------------------------------------------

#include "agentParser.h"
#include "Interfaces/componentInterface.h"
#include "channel.h"


namespace SimulationSlave {
namespace Scheduling {

AgentParser::AgentParser(const int &currentTime) : currentTime{currentTime}
{}

void AgentParser::Parse(const Agent &agent)
{
    for(const auto &componentMap : agent.GetComponents())
    {
        std::list<TaskItem> taskItems;

        ComponentInterface *component = componentMap.second;

        auto priority = component->GetPriority();
        auto cycleTime = component->GetCycleTime();
        auto triggerDelay = component->GetOffsetTime();
        auto updateDelay = component->GetResponseTime();
        auto agentId = agent.GetId();

        std::function<bool()> triggerFunc = std::bind(&ComponentInterface::TriggerCycle, component, std::ref(currentTime));
        taskItems.push_back(
        {
             TriggerTaskItem(agentId, priority, cycleTime, triggerDelay, triggerFunc)
        });
//        std::cout << "Trigger Task: AgentId: " << agentId << " prio: " << priority <<
//                      " cycleTime: " << cycleTime <<  " delay: " << triggerDelay << std::endl;

        for(auto& channel : component->GetOutputLinks())
        {
            int outputLinkId = channel.first;

            std::function<bool()> updateFunc = std::bind(&ComponentInterface::AcquireOutputData, component, outputLinkId, std::ref(currentTime));
            taskItems.push_back(
            {
               UpdateTaskItem(agentId, priority, cycleTime, updateDelay, updateFunc)
            });
//            std::cout << "UpdateOUT Task: AgentId: " << agentId << " prio: " << priority <<
//                          " cycleTime: " << cycleTime <<  " delay: " << updateDelay << std::endl;

            for(auto& target : channel.second->GetTargets())
            {
                int targetLinkId = std::get<static_cast<size_t>(Channel::TargetLinkType::LinkId)>(target);
                ComponentInterface *targetComponent = std::get<static_cast<size_t>(Channel::TargetLinkType::Component)>(target);

                std::function<bool()> updateInFunc = std::bind(&ComponentInterface::UpdateInputData, targetComponent, targetLinkId, std::ref(currentTime));
                taskItems.push_back(
                {
                    UpdateTaskItem(agentId, priority, cycleTime, updateDelay, updateInFunc)
                });
//                std::cout << "UpdateIN Task: AgentId: " << agentId << " prio: " << priority <<
//                              " cycleTime: " << cycleTime <<  " delay: " << updateDelay << std::endl;
            }
        }

        if(component->GetInit()) {
            nonRecurringTasks.insert(nonRecurringTasks.end(),
                                     std::make_move_iterator(taskItems.begin()),
                                     std::make_move_iterator(taskItems.end()));
        }
        else
        {
            recurringTasks.insert(recurringTasks.end(),
                                  std::make_move_iterator(taskItems.begin()),
                                  std::make_move_iterator(taskItems.end()));
        }
    }
}

std::list<TaskItem> AgentParser::GetNonRecurringTasks()
{
    return nonRecurringTasks;
}

std::list<TaskItem> AgentParser::GetRecurringTasks()
{
    return recurringTasks;
}

} //namespace Scheduler
} //namespace SimulationSlave
