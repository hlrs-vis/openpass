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
/** \file  TaskBuilder.h
*	\brief This file contains the logic of unchangeable tasks
*	\details bootstrap, common and finalize tasks have their fix order
*           The taskBuilder generates task lists and fill them with necessary tasks
*/
//-----------------------------------------------------------------------------

#include <functional>
#include <vector>
#include <list>

#include "tasks.h"
#include "Interfaces/spawnControlInterface.h"
#include "Interfaces/worldInterface.h"
#include "runResult.h"
#include "observationNetwork.h"
#include "eventDetectorNetwork.h"
#include "manipulatorNetwork.h"

using namespace SimulationSlave;
using namespace Scheduling;

//-----------------------------------------------------------------------------
/** \brief interface of taskBuilder
*
* 	\ingroup OpenPassSlave
*/
//-----------------------------------------------------------------------------

class TaskBuilderInterface
{
public:
    virtual std::list<TaskItem> CreateBootstrapTasks()=  0;
    virtual std::list<TaskItem> CreateCommonTasks() = 0;
    virtual std::list<TaskItem> CreateFinalizeRecurringTasks() = 0;
    virtual std::list<TaskItem> CreateFinalizeTasks() = 0;    
};

//-----------------------------------------------------------------------------
/** \brief handles fix tasks of scheduler
*
* 	\ingroup OpenPassSlave
*/
//-----------------------------------------------------------------------------

class TaskBuilder : public TaskBuilderInterface
{
private:
    const int &currentTime;
    RunResult &runResult;
    const int frameworkUpdateRate;
    const int ScheduleAtEachCycle = 0;


    WorldInterface* const world;
    SpawnControlInterface* const spawnControl;
    ObservationNetworkInterface* const observationNetwork;
    EventDetectorNetworkInterface* const eventDetectorNetwork;
    ManipulatorNetworkInterface* const manipulatorNetwork;

    std::list<TaskItem> eventDetectorTasks;
    std::list<TaskItem> manipulatorTasks;

    /*!
    * \brief BuildEventDetectorTasks
    *
    * \details In constructor all eventDetectors are read out and
    *           stored in eventDetector Tasks to fill them later in
    *           tasks.
    */
    void BuildEventDetectorTasks();
    /*!
    * \brief BuildManipulatorTasks
    *
    * \details In constructor all manipulators are read out and
    *           stored in eventDetector Tasks to fill them later in
    *           tasks.
    */
    void BuildManipulatorTasks();

public:
    TaskBuilder(const int &currentTime, RunResult &runResult,
                const int frameworkUpdateRate,
                WorldInterface* const world,
                SpawnControlInterface* const spawnControl,
                ObservationNetworkInterface * const observationNetwork,
                EventDetectorNetworkInterface* const eventDetectorNetwork,
                ManipulatorNetworkInterface* const manipulatorNetwork);

    virtual ~TaskBuilder() = default;
    /*!
    * \brief CreateBootstrapTasks
    *
    * \details Creates tasks with all tasks of bootstrap phase.
    *          Bootstrap tasks can not be changed.
    *
    * @return	tasks   final tasks of bootstrap phase
    */
    std::list<TaskItem> CreateBootstrapTasks() override;

    /*!
    * \brief CreateCommonTasks
    *
    * \details Creates tasks with all tasks of common phase.
    *          Common tasks can not be changed.
    *
    * @return	tasks   final tasks of common phase
    */
    std::list<TaskItem> CreateCommonTasks() override;

    /*!
    * \brief CreateFinalizeRecurringTasks
    *
    * \details Creates tasks with all tasks of finalizeRecurring phase.
    *          FinalizeRecurring tasks can not be changed.
    *
    * @return	tasks   final tasks of finalizeRecurring phase
    */
    std::list<TaskItem> CreateFinalizeRecurringTasks() override;

    /*!
    * \brief CreateFinalizeTasks
    *
    * \details Creates tasks with all tasks of finalize phase.
    *          Finalize tasks can not be changed.
    *
    * @return	tasks   final tasks of finalize phase
    */
    std::list<TaskItem> CreateFinalizeTasks() override;

};
