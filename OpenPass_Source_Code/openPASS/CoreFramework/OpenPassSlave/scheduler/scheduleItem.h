/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  scheduleItem.h
//! @brief This file contains the internal representation of an entry in the
//!        schedule table.
//-----------------------------------------------------------------------------

#ifndef SCHEDULEITEM_H
#define SCHEDULEITEM_H

#include <list>
#include "spawnItemParameter.h"

namespace SimulationSlave
{

enum class ScheduleItemType
{
    ScheduleSpawnItemType,
    ScheduleTriggerItemType,
    ScheduleUpdateItemType
};

class Component;
class Agent;
class SchedulePolicy;
class Stochastics;
class SpawnPoint;

//-----------------------------------------------------------------------------
//! Interface for items that will be added to the scheduler.
//-----------------------------------------------------------------------------
class ScheduleItem
{
public:
    ScheduleItem(ScheduleItemType type, WorldInterface *world) :
        type(type),
        world(world)
    {}
    ScheduleItem(const ScheduleItem&) = delete;
    ScheduleItem(ScheduleItem&&) = delete;
    ScheduleItem& operator=(const ScheduleItem&) = delete;
    ScheduleItem& operator=(ScheduleItem&&) = delete;
    virtual ~ScheduleItem() = default;

    //-----------------------------------------------------------------------------
    //! Executes a scheduled item.
    //!
    //! @param[in]  schedule            Policy of the scheduler
    //! @return                         Flag if scheduled item was executed successfully
    //-----------------------------------------------------------------------------
    virtual bool Execute(SchedulePolicy *schedule) = 0;

    //-----------------------------------------------------------------------------
    //! Compares the priority of this item against the one provided via parameter.
    //!
    //! @param[in]  other               Schedule item against which the priority is
    //!                                 compared
    //! @return                         1, if this items priority is higher, 0, if
    //!                                 both are equal, -1 otherwise
    //-----------------------------------------------------------------------------
    virtual int CompareToPriority(ScheduleItem *other) const = 0;

    //-----------------------------------------------------------------------------
    //! Returns if the item is an init item.
    //!
    //! @return                         Flag if the item is an init item
    //-----------------------------------------------------------------------------
    virtual bool GetInit() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the time after which the item is rescheduled.
    //!
    //! @return                         Time after which the item is rescheduled
    //-----------------------------------------------------------------------------
    virtual int GetNextTime() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the content ID of the item.
    //!
    //! @return                         Content ID
    //-----------------------------------------------------------------------------
    virtual int GetContentId() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the schedule item type.
    //!
    //! @return                         Schedule item type
    //-----------------------------------------------------------------------------
    ScheduleItemType GetType() const;

    //-----------------------------------------------------------------------------
    //! Declare this item as invalid.
    //-----------------------------------------------------------------------------
    void Invalidate();

    //-----------------------------------------------------------------------------
    //! Returns the status of the valid flag.
    //!
    //! @return                         Flag if item is valid
    //-----------------------------------------------------------------------------
    bool GetValid() const;

protected:
    bool valid = true;
    ScheduleItemType type;
    WorldInterface *world;
};

class ScheduleSpawnItem : public ScheduleItem
{
public:
    ScheduleSpawnItem(SpawnPoint *spawnPoint, WorldInterface *world) :
        ScheduleItem(ScheduleItemType::ScheduleSpawnItemType, world),
        spawnPoint(spawnPoint)
    {}
    ScheduleSpawnItem(const ScheduleSpawnItem&) = delete;
    ScheduleSpawnItem(ScheduleSpawnItem&&) = delete;
    ScheduleSpawnItem& operator=(const ScheduleSpawnItem&) = delete;
    ScheduleSpawnItem& operator=(ScheduleSpawnItem&&) = delete;
    virtual ~ScheduleSpawnItem() = default;

    //-----------------------------------------------------------------------------
    //! Compares the priority of this item against the one provided via parameter.
    //! Will return 1 as a schedule spawn item always has highest priority.
    //!
    //! @param[in]  other               Schedule item against which the priority is
    //!                                 compared
    //! @return                         1
    //-----------------------------------------------------------------------------
    virtual int CompareToPriority(ScheduleItem *other) const;

    //-----------------------------------------------------------------------------
    //! Returns false as a spawn item is no init item.
    //!
    //! @return                         False
    //-----------------------------------------------------------------------------
    virtual bool GetInit() const;

    //-----------------------------------------------------------------------------
    //! Returns the ID of the stored agent of the schedule spawn item.
    //!
    //! @return                         Content ID
    //-----------------------------------------------------------------------------
    virtual int GetContentId() const;

    //-----------------------------------------------------------------------------
    //! Creates a new agent using a random agent spawn item from the list of agent
    //! spawn items in the stored spawn point and the stored spawn item parameters
    //! and schedule to spawn this agent on the provided schedule policy.
    //!
    //! @param[in]  schedule            Policy of the scheduler
    //! @return                         Flag if scheduled item was executed successfully
    //-----------------------------------------------------------------------------
    virtual bool Execute(SchedulePolicy *schedule);

    //-----------------------------------------------------------------------------
    //! Returns the time after which the item is rescheduled.
    //!
    //! @return                         Time after which the item is rescheduled
    //-----------------------------------------------------------------------------
    virtual int GetNextTime();

protected:
    SpawnPoint *spawnPoint;
    SpawnItemParameter spawnItemParameter;
};

class ScheduleTaskItem : public ScheduleItem
{
public:
    ScheduleTaskItem(Component *component,
                     const Agent *agent,
                     ScheduleItemType type,
                     WorldInterface *world) :
        ScheduleItem(type, world),
        component(component),
        agent(agent)
    {}
    ScheduleTaskItem(const ScheduleTaskItem&) = delete;
    ScheduleTaskItem(ScheduleTaskItem&&) = delete;
    ScheduleTaskItem& operator=(const ScheduleTaskItem&) = delete;
    ScheduleTaskItem& operator=(ScheduleTaskItem&&) = delete;
    virtual ~ScheduleTaskItem() = default;

    //-----------------------------------------------------------------------------
    //! Compares the priority of this item against the one provided via parameter.
    //! If either one is an init item, it will have the higher priority. Otherwise,
    //! first compare their component's and then, if they are equal, their agent's
    //! priorities. If all of these priorities are equal, assign a higher priority
    //! to the ScheduleTriggerItemType, if the other is of type ScheduleUpdateItemType.
    //! Otherwise, they have the same priority
    //!
    //! @param[in]  other               Schedule item against which the priority is
    //!                                 compared
    //! @return                         1, if this items priority is higher, 0, if
    //!                                 both are equal, -1 otherwise
    //-----------------------------------------------------------------------------
    virtual int CompareToPriority(ScheduleItem *other) const;

    //-----------------------------------------------------------------------------
    //! Returns if the item is an init item.
    //!
    //! @return                         Flag if the stored component is an init component
    //-----------------------------------------------------------------------------
    virtual bool GetInit() const;

    //-----------------------------------------------------------------------------
    //! Returns the time after which the item is rescheduled.
    //!
    //! @return                         Cycle time of the stored component
    //-----------------------------------------------------------------------------
    virtual int GetNextTime();

    //-----------------------------------------------------------------------------
    //! Returns the ID of the stored agent of the schedule spawn item.
    //!
    //! @return                         ID of the stored agent
    //-----------------------------------------------------------------------------
    virtual int GetContentId() const;

    //-----------------------------------------------------------------------------
    //! Returns the stored component.
    //!
    //! @return                         Stored component
    //-----------------------------------------------------------------------------
    Component *GetComponent() const;

    //-----------------------------------------------------------------------------
    //! Returns the stored agent.
    //!
    //! @return                         Stored agent
    //-----------------------------------------------------------------------------
    const Agent *GetAgent() const;

protected:
    Component *component;
    const Agent *agent;
};

class ScheduleTriggerItem : public ScheduleTaskItem
{
public:
    ScheduleTriggerItem(Component *component,
                        const Agent *agent,
                        WorldInterface *world) :
        ScheduleTaskItem(component,
                         agent,
                         ScheduleItemType::ScheduleTriggerItemType,
                         world)
    {}
    ScheduleTriggerItem(const ScheduleTriggerItem&) = delete;
    ScheduleTriggerItem(ScheduleTriggerItem&&) = delete;
    ScheduleTriggerItem& operator=(const ScheduleTriggerItem&) = delete;
    ScheduleTriggerItem& operator=(ScheduleTriggerItem&&) = delete;
    virtual ~ScheduleTriggerItem() = default;

    //-----------------------------------------------------------------------------
    //! Executes a scheduled item. If stored agent is invalid, remove it from the
    //! scheduler and from the world's agent network. Finally, invalidate this item.
    //!
    //! @param[in]  schedule            Policy of the scheduler
    //! @return                         Flag if this item schedult should be triggered
    //!                                 at the curent time
    //-----------------------------------------------------------------------------
    virtual bool Execute(SchedulePolicy*schedule);
};

class ScheduleUpdateItem : public ScheduleTaskItem
{
public:
    ScheduleUpdateItem(Component *component,
                       const Agent *agent,
                       WorldInterface *world) :
        ScheduleTaskItem(component,
                         agent,
                         ScheduleItemType::ScheduleUpdateItemType,
                         world)
    {}
    ScheduleUpdateItem(const ScheduleUpdateItem&) = delete;
    ScheduleUpdateItem(ScheduleUpdateItem&&) = delete;
    ScheduleUpdateItem& operator=(const ScheduleUpdateItem&) = delete;
    ScheduleUpdateItem& operator=(ScheduleUpdateItem&&) = delete;
    virtual ~ScheduleUpdateItem() = default;

    //-----------------------------------------------------------------------------
    //! Gets the target components from all output channels of the stored component.
    //! Then, update the input data on all of them and release the output data buffer
    //! for each channel on the component. Then, if stored agent is invalid, remove
    //! it from the scheduler and from the world's agent network. Finally, invalidate
    //! this item.
    //!
    //! @param[in]  schedule            Policy of the scheduler
    //! @return                         Flag if scheduled item was executed successfully
    //-----------------------------------------------------------------------------
    virtual bool Execute(SchedulePolicy*schedule);
};

} // namespace SimulationSlave

#endif // SCHEDULEITEM_H
