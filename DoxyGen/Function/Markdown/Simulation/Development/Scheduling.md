\page scheduling Scheduling

\tableofcontents

\section scheduler Scheduler

The Scheduler handles the sequence of agent based and common tasks before, during and after simulation.

\subsection scheduler_phases Executing phases

Each simulation run splits up into 6 phases: Bootstrap, Common, NonRecurring, Recurring, FinalizeRecurring and Finalize tasks.
With the exception of recurring and non-recurring tasks the phases are immutable, once set up.

![Scheduler phases](Scheduler_phases.svg)

1. Initial task are grouped within the bootstrap tasks and are executed first and only once.

2. After this Common, NonRecurring, Recurring and FinalizeRecurring Tasks are processed at each timestamp. 
Thereby, the common phase is a recurring initialization phase for the whole simulator, such as spawning, event detection and observation logging.  
Whenever a new agent is spawned, all its components are parsed to *trigger* and *update* tasks, grouped and sorted by execution time as non-recurring and recurring tasks. 
Non-recurring tasks are executed just once and will be deleted afterwards (`init` flag in ComponentRepository).
Whenever an agent is spawned or removed, the non-recurring and recurring tasks are updated.  
Right after execution of all agent-based tasks, the FinalizeRecurring phase synchronizes all changes to the world.

3. The last phase is finalizing task, executed when an end condition is reached, such as end of simulation duration. Within this phase all event detectors and manipulators are triggered and the observation module is updated one last time.

\subsection scheduler_tasks Task type description

The scheduler handles 8 different task types:

* **Spawning** - triggers agent spawning while spawntime and parse agent tasks
* **EventDetector** - execute event detector
* **Manipulator** - execute manipulator
* **Observation** - update observation modules
* **Trigger** - execute trigger function of connected component
* **Update** - execute update output of connected component and resulting input
* **UpdateGlobalDrivingView** - create global driving view
* **SyncGlobalData** - update global data

The following table gives an overview to all phases.

| **Phase**  | **Changeable**| **Task types**| **Notes**|
| ------------- |-------------|-------------|-------------|
| Bootstrap  | no | Observation | |
| Common  | no | Spawning, EventDetector, Manipulator, Observation| |
| Non recurring | yes | Trigger, Update | |
| Recurring  | yes | Trigger, Update | |
| Finalize recurring | no | SyncGlobalData | Update the state of the agents and the virtual world (e.g. due to agent movements).|
| Finalize | no | EventDetector, Manipulator, Observation | |

\subsection scheduler_priorities Task priorities

The order of the tasks within one phase depends on component priority and task type.
All tasks for a given timestamp are ordered by the SchedulerTasks priority, and divided into the aformentioned phases.

The following table shows the priorities for the non-component task types:

| **Task type**  | **Priority**| **Notes** |
| ------------- |-------------|-------------|
| Spawning  | 4 | Highest priority, nothing can happen before any agent is instantiated. |
| EventDetector  | 3 |  |
| Manipulator | 2 | The manipulator task uses the event detector tasks as input. Therefore it is called afterwards. |
| SyncGlobalData  | 1 | |
| Observation | 0 | Observation tasks have to be execute at the end.|

The priority of trigger and update tasks depend on the priority within the ComponentRepository. 
They are independent of non-component priorities.
Since trigger tasks prepare output signals, output tasks are be called after the corresponding trigger tasks.

\subsection scheduler_callsdiagram Class diagram

The scheduler contains the logical process of each simulation. At the beginning of each run the task builder will create all fix tasks.

![Class diagram Scheduler](ClassdiagramScheduler.svg)

The class SchedulerTasks manages all existing tasks and returns filtered tasks for each timestamp. (Except Bootstrap and Finalize tasks, they are executed once and do not have to be filtered.)
SchedulerTasks calculates with the help of cycletime and delay if the task has to be execute in an given timestamp.
All tasks are managed in multisets to order them automatically by component priority and task type.

The scheduler requests all tasks for the current timestamp and execute them.

\subsection scheduler_procedure Scheduler Procedure

For each simulation run the following steps are made:

1. Instantiate TaskBuilder and create Bootstrap, Common, FinaliuzeRecurring and Finalize tasks.

2. Instantiate and fill SchedulerTasks with built task lists.

3. Initial spawning

4. Execute bootstrap tasks

5. Execute common tasks

6. Update SchedulerTasks (spawning/removing change component tasks)

7. execute component tasks (non recurring, recurring, finalize recurring)

8. make timestep

9. Repeat steps 5 till 8 until end condition is reached

10. Execute finalize tasks

\subsection scheduler_spawncontrol SpawnControl
SpawnControl triggers for each SpawnPoint spawning of given next spawn time. Spawning includes generating an AgentBlueprint, which is processed in [SpawnPoint](@ref dev_framework_modules_spawnpoint), and instantiate the given agent.

Before setting a common agent into the world, SpawnControl checks the distance to the next front agent and validates time to brake. If this violates ttb regulation, the velocity of the agent will be adapted, until ttb criterion is met.

