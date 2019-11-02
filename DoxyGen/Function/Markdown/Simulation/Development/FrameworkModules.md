\page dev_framework_modules Framework Modules

\tableofcontents

[//]: <> (Please sort by alphabet.)

The modules of openPASS are split the two major categories `Framework Modules` and `Agent Modules` and defined within [SystemConfigBlueprint](\ref io_input_systemconfigblueprint).
`Framework Modules` are instantiated once for the entire simulation  and **all of them are necessary**.
`Agent Modules` are instantiated individually for each agent and as such, it depends on the agents configuration which are necessary.

This section covers the **Framework Modules**, the **Agent Modules** can be found [here](@ref dev_agent_modules).

\section dev_framework_modules_agentfactory AgentFactory

The AgentFactory instantiates and manages all Agent instances.
When the [SpawnControl](\ref dev_framework_modules_spawnControl) tries to create a new Agent it first calls the SpawnPoint and the AgentBlueprintProvider to generate an AgentBlueprint.
This AgentBlueprint is then forwarded to the AgentFactory, which instantiates the Agent and all its components.
In order to instantiate an Agent, all required Channels and Components which are stored in the AgentBlueprint, need to be instantiated.
After the Channels and Components are created, the AgentFactory establishes the links between the different components.

\section dev_framework_modules_agentBlueprintProvider AgentBlueprintProvider

The AgentBlueprintProvider is responsible for configuring agents and gathering the required components based on the [ProfilesCatalog](\ref io_input_profilescatalog) and stochastic values.

When creating a new Agent the [SpawnPoint](\ref dev_framework_modules_spawnpoint) is responsible for filling out the first part of the AgentBlueprint.
Initially the SpawnPoint determines in which lane the agent is supposed to be spawned. 
Via the "roadId" specified in the [Scenario.xml](\ref io_input_scenario) each ego or scenario agent can be placed on a different road.
Spawning of common traffic only considers the ego road.
After that, the SpawnPoint calls the AgentBlueprintProvider to fill out the second part of the AgentBlueprint.

\subsection Sequence
First the AgentProfilesSampler samples the name of the AgentProfile. An AgentProfile can either be static or dynamic. In the case of a static AgentProfile the components and channels of the agent are defined
by a system config. In this cause the AgentBlueprintProvider loads the specified system via fileName and system id. Additionally the AgentProfile defines the VehicleModel.

In the case of a dynamic AgentProfile additional profiles need to be sampled. This is handled by the DynamicProfileSampler. After that the DynamicParameterSampler samples probabilistic parameters like sensor latencies.
Then the AgentType with the components and channels is build by the DynamicAgentTypeGenerator. The VehicleModel is in this case defined via the VehicleProfile.

![AgentBlueprintProviderSequenceDiagram](AgentBlueprintProviderSequence.svg)

\section dev_framework_modules_eventdetectors EventDetectors

[//]: <> (Please refer to each section!)
* [TimeTriggerDetector](\ref dev_framework_modules_eventdetectors_timetrigger)
* [CollisionDetector](\ref dev_framework_modules_eventdetectors_collision)

EventDetectors are used to trigger Manipulators. They are waiting for specific behaviors or settings during the simulation. Once an Event occurs it is forwarded to the related Manipulator.

EventDetectors can be specified to only trigger for certain Agents in the Scenario.xosc.

```xml
    <Actors>
        <Entity name="Ego"/>
    </Actors>
```

The "Actors"-tag is mandatory, but the "Entity"-tags are optional.
If no Actors are specified, the Event is triggered for all Agents.

With the "name"-Attribute of an "Entity" a specific Agent can be targeted by the event. This attribute relates to the Ego and Scenario Agents defined in the Scenario.xosc.

The class structure of the EventDetectors is showed in the following diagram:

![EventDetectors](EventDetectors.svg)

\subsection dev_framework_modules_eventdetectors_timetrigger TimeTriggerDetector
* **Description:**
    Triggers only once at a specified time value, configured by the detector parameters in the [Scenario File](\ref
    io_input_scenario).
    Time specified in seconds as a double.
    The rule is required and only "greater_than" is accepted as valid.

```xml
<StartConditions>
    <ConditionGroup>
        <Condition name="Conditional">
            <ByValue>
                <SimulationTime value="5.0" rule="greater_than" />
            </ByValue>
        </Condition>
    </ConditionGroup>
</StartConditions>
```

* **Event:** TimeTriggerEvent (AgentBasedEvent)
  Parameter:

    |Name|Description|
    |----|------------|
    |AgentId|Id of the acting agent.|

---

\subsection dev_framework_modules_eventdetectors_collision CollisionDetector
* **Description:**
    Checks if a collision occured. It is always activated and **cannot be deactivated**.

* **Trigger:** 
    Always active, with cycle time 100 [ms]

* **Configuration:**

    |ParameterName|Type|Description|
    |-----------|------|-----------|
    |-|-|-|

* **Event:** CollisionEvent (CollisionEvent)
  Parameter:

    |Name|Description|
    |-----|------------|
    |CollisionWithAgent|Determines whether this collision involved two agents or one agent and one traffic object.|
    |CollisionAgentId|Agent id of the first agent.|
    |CollisionOpponentId|Id of either the second agent or the traffic object.|

---

\section dev_framework_modules_eventnetwork EventNetwork

The EventNetwork is the central module which manages all Events. It makes the Events available for multiple other framework modules.
All new Events are passed to the EventNetwork where they are stored as active events.
After each time step the EventNetwork archives the currently active events.
Additionally the EventNetwork offers several methods and filters to access the events.


List of Modules which use the EventNetwork:
1. ObservationLog
2. Respawner
3. EventDetector
4. Manipulator
5. VehicleControlUnit


![EventNetwork](EventNetworkClassdiagram.svg)

---

\section dev_framework_modules_manipulators Manipulators

[//]: <> (Please refer to each section!)
* [CollisionManipulator](\ref dev_framework_modules_manipulators_collision)
* [ComponentStateChangeManipulator](\ref dev_framework_modules_manipulators_componentstatechange)

The class structure of the Manipulators is showed in the following diagram:

![Manipulators](ManipulatorsOS.svg)

---

\subsection dev_framework_modules_manipulators_collision CollisionManipulator
* **Description:**
    This manipulator notifies the two collided agents and recursively all of their other collisionspartners about the new collision

* **Trigger:** This manipulator is always triggered by CollisionDetector.

* ** Event: ** -

* **Configuration:**

    |ParameterName|Type|Description|
    |-----------|------|-----------|
    |-|-|-|

---

\subsection dev_framework_modules_manipulators_componentstatechange ComponentStateChangeManipulator
* **Description:**
    This manipulator modifies the state of a vehicle component.

* **Trigger:** This manipulator can be triggered by any detector. This has to be parameterized in scenario configuration (see [Input/Output documentation](\ref io_input_scenario)).

* ** Event: ** Creates and event of type ComponentChangeEvent for the specified component name.

* **Configuration:**

```xml
<Action name="ExampleComponentStateChangeManipulator">
	<UserDefined>
		<Command>SetComponentState DynamicsTrajectoryFollower Acting</Command>
	</UserDefined>
</Action>
```

|ElementName|Attribute|Type  |Description                                                   |
|-----------|---------|------|--------------------------------------------------------------|
|Command    |-        |string|This string must be formatted as "SetComponentState <TargetComponent> <DesiredState>", where <TargetComponent> is the target component name and <DesiredState> is one of "Acting", "Armed", or "Disabled".|

---

\section dev_framework_modules_observationlog ObservationLog

The ObservationLog is responsible for generating the [simulation output](\ref io_output_simout). This module handles the file stream to the output file.
Which parameters are logged in the cyclics tag is specified by the [LoggingGroups tag](\ref io_input_slaveconfig_experimentconfig_logginggroups) in the SlaveConfig

The output is initially written into a temporary simulation output file. The temporary output is updated at the end of every invocation. Once all invocations completed successfully the temporary file is renamed to SimulationOutput.xml.

All information which needs to be written into the log is forwarded to an instance of ObservationInterface. This includes RunStatistics, Events and Agent parameters.
Additionally to the logging the ObservationLog is also used by other modules to add their Events to the EventNetwork.

---

\section dev_framework_modules_sampler Sampler

The Sampler contains the entire stochastic functionality of the AgentBlueprintProvider and the SpawnPoint. Additionally it prepares the World- and SpawnPoint-Parameters, which are defined based on probabilities in the SlaveConfig.
The AgentBlueprintProvider uses this module to pick values based on a set of probabilities, which are defined in the SlaveConfig.
The SpawnPoint uses this module to generate values based on different distributions.

The Sampler uses the Stochastics module to generate random values.

![Sampler](SamplerClassdiagram.svg)

---

\section dev_framework_modules_spawnControl SpawnControl

The SpawnControl triggers the [SpawnPoint](\ref dev_framework_modules_spawnpoint), which returns an AgentBlueprint. After the SpawnControl received an AgentBlueprint, it checks whether the TTB-Criteria are fulfilled. Those criteria are only checked for CommonAgents. If the criteria are not met the new CommonAgent would be too fast compared to the agents in its full braking distance or the end of the lane. In this case, the agent is held back for a calculated hold back time such that there won't be a crash, if the fronts agent drive with constant velocity during the holdback time. After the holdback time the new agent is again checked for TTB violations. If the TTB is again violated, the agent is still spawned, but its velocity is reduced in steps of 10 km/h until the TTB-Criteria are met. If the velocity can't be reduced enough to prevent a collision, the AgentBlueprint is discarded.
The TTB-Criteria are defined as followed:

TTB is set to 1 second.
AssumedDeceleration of the new agent is set to `-6 m/s²` und of the front agent to `-10 m/s²`

The fullBrakingDistance is calculated with this formula: `Velocity * TTB + Velocity² / 2 * | AssumedDeceleration |`

The freeSpace is measured from the front of the new agent to the rear of the agents in its fullBrakingDistance and the end of the lane.

The TTB-criteria are then being calculated with the [WillCrash and WillCrashDuringBrake](\ref dev_tools_commonTools_ttcttb) methods using the above parameters.

Under the following two cases the agent is not held back but immediatly spawned with reduced velocity:
* The remaining distance of the spawning lane is smaller than the fullBrakingDistance.
* The front agent has velocity 0.

The maximum holdbackTime is `5000ms`.

---

\section dev_framework_modules_spawnpoint SpawnPoint

The SpawnPoint is responsible for spawning all agents of the simulation.
We currently have 3 different agent types:
- Ego-Agent: Single entity which is the focus of the simulation run.
- Scenario-Agent: Specifically placed and defined agents to enforce certain situations or behaviors 
- Common-Agent: Random generated agents, which enable stochastic variety in the simulation


In the SpawnPoint the Position and Dynamics of an agent are initially defined.
After the SpawnPoint an additional module the [SpawnControl](\ref dev_framework_modules_spawnControl) tests whether all CommonAgents are fulfilling the TTB requirements. If the TTB requirements are not met the dynamics of the agent are adjusted.

The Ego-Agent and the Scenario-Agents can have predefined parameters in the [Scenario.xosc](\ref io_input_scenario).
The parameters of the Common Agents are defined by stochastic probabilities.
The following chart shows the order in which the the spawn parameters are picked at agent creation. If parameters of Ego or Scenario are predefined values, several steps can be skipped.

![Spawn Parameter Order ](SpawnPointStochastics.svg)

The Position of a new agent always depends on existing agents. During forward spawning the furthest agent in the lane is considered and the new agent is placed at the distance of the furthest agent plus a specific separation. During backward spawning the last agent in the lane is considered and the new agent is then placed at the distance of the last agent minus the separation.
The separation between agents is calculated based on the gap and the velocity of the rear agent, as shown in the following figure.

![Spawn Separation ](SpawningSeparation.png)

The spawning behavior is split into 7 states. 5 of those states are part of the initial spawning before the simulation starts. The other 2 states can be performed during the simulation.

\subsection dev_framework_modules_spawnpoint_egoplacement EgoPlacement

In the first phase the Ego Agent is spawned according to its definition in the [Scenario.xosc](\ref io_input_scenario). There can only be a single Ego Agent. If the Ego Agent can not be spawned because of invalid spawn parameters, the simulation will terminate with an error.
![Initial Ego placement ](spawnpoint_egoplacement.png)

\subsection dev_framework_modules_spawnpoint_scenarioplacement ScenarioPlacement

In the second phase all Scenario Agents are spawned according to their specifications in the [Scenario.xosc](\ref io_input_scenario). Scenario Agents are optional, there can exist an arbitrary number of them. Scenario Agents are being used to enforce specific situations and behaviors. In case a Scenario Agent is in the same lane as an Ego Agent the space between them is not being filled with other agents.
![Initial Scenario placement ](spawnpoint_scenarioplacement.png)

\subsection dev_framework_modules_spawnpoint_commonspecificfrontplacement CommonSpecificFrontPlacement

In the third phase, Common Agents are spawned in front of all ego or scenery agents. For each lane, which contains either Ego or Scenario Agents, the furthest agent is determined. Then said lanes are filled with Common Agents starting from the furthest agent until the end of the road or until the maximum radius around the Ego is reached. 

![Initial Common specific front placement ](spawnpoint_commonfrontplacement.png)

\subsection dev_framework_modules_spawnpoint_commonspecificrearplacement CommonSpecificRearPlacement

In the fourth phase, Common Agents are spawned behind all ego or scenery agents. For each lane, which contains either Ego or Scenario Agents, the closest vehicle is determined. Then said lanes are filled with Common Agents starting from the closest Agent to the beginning of the road. After this phase all Ego and Scenario lanes are completely filled with agents.

![Initial Common specific rear placement ](spawnpoint_commonrearplacement.png)

\subsection dev_framework_modules_spawnpoint_commonprerunplacement CommonPreRunPlacement

In the last phase of the initial pre-run spawning all lanes, which are neither Ego or Scenario lanes, are filled with agents. Those lanes are then filled from the front to the beginning. The starting point for spawning is at the end of the road or at the maximum radius around the Ego Agent, which ever comes first. The end point is then either the start of the road or again the maximum radius around the Ego Agent. The maximum radius around the Ego Agent is currently set to 1000 meter. After this phase all lanes are initially filled with agents and the simulation can start.

![Initial Common Pre Run placement ](spawnpoint_commonprerunplacement.png)

\subsection dev_framework_modules_spawnpoint_commonspawning CommonSpawning

The CommonSpawning is active during the simulation run. In this mode Common Agents are continuously placed at the beginning of the road. Since agents can not be placed outside of the road the SpawnPoint waits until a timely gap between agents has passed before placing the next on at the start of the road.

\subsection dev_framework_modules_spawnpoint_respawning Respawning

The Respawning mode can only be triggered through Manipulators. A random velocity is rolled for the new agent. If the new agent is faster than the Ego Agent it is spawned behind the ego at the maximum radius of the ego. If the new agent is slower than the ego it is spawned in front of the ego at maximum radius around the ego.

\subsection dev_framework_modules_spawnpoint_classdiagram Classdiagram

The class structure of the SpawnPoint is as follows:

![Spawn Point Class Diagram](SpawnPointClassDiagram.svg)

\subsection dev_framework_modules_spawnpoint_placement_egoscenario Placement of ego and scenario vehicles

The Positions of the ego car and the scenario cars is specified inside the [Scenario.xosc](\ref io_input_scenario) file.

These are some examples:
```xml
<Position>
  <Lane roadId="1" s="0.0" laneId="-1" offset="-0.2">
    <Orientation type="absolute" h="0.5"/>
  </Lane>
</Position>
```

s defines the s-coordinate of the vehicle on the road.
laneId specifies the id of the lane on which the car will be placed.
roadId specifies the openDrive road on which the car will be placed.
offset defines the offset of the vehicle from the middle of the lane.
A negative offset moves the vehicle to the right and positive offset to the left.

It is possible to add stochastics to the placement of ego and scenario vehicles.
The optional xml-tag Stochastics below the lane tag specifies the probablities.

```xml
<Position>
  <Lane roadId="1" s="250.0" laneId="-1" offset="-0.2">
    <Stochastics value="s" stdDeviation="10" lowerBound="70" upperBound="150"/>
    <Stochastics value="offset" stdDeviation="0.2" lowerBound="-1" upperBound="1"/>
    <Orientation type="absolute" h="0.5"/>
  </Lane>
</Position>
```

value: attribute which should use stochastics (s or offset)
The mean value is the value of the s (respectively offset) attribute inside the lane tag
stdDeviation: standard deviation
lowerBound: Lower bound
upperBound: Upper bound


The placement of ego and scenario vehicles will be rejected if the vehicle is partly on an invalid lane or more than 50% outside of the lane.
After a rejected placement the spawn point returns an error and a new trial will be started (max 5 trials).


![OK](SpawnPoint_ok.png) 
OK: Vehicle completely inside lane

![Middle](SpawnPoint_Middle.png)
OK: More than half of the vehicle is inside the lane

![MoreThanHalf](SpawnPoint_MoreThanHalf.png)
Error: More than half of the vehicle is not inside the lane

![Invalid](SpawnPoint_Invalid.png)
Error: The vehicle is partly outside of the world

\subsection dev_framework_modules_spawnpoint_placement_common Placement of common vehicles

Three of the spawn parameters are being stochastically rolled.
1. lane id
2. velocity
3. tgap to next agent

After that, the SpawnPoint validates s-Coordinate, offset and distance to end of lane and also checks if any agent intersect the new agent.
If one of the conditions result in invalid spawn parameters the agentblueprint will be skipped and a new agentblueprint will be created.

---

\section dev_framework_modules_stochastics Stochastics

The Stochastics Module is used to created random numbers. A random seed is used to initialize a Mersenne Twister 19937 generator.
Afterwards numbers are drawn from said generator.

The following types of random numbers are currently available:

[//]: <> (Please refer to each section!)
* Uniform: Expects upper and lower boundary
* Normal: Expects means and standard deviation
* Binomial: Expects upper range number and probability
* Exponential: Expects lambda
* Gamma: Expects mean and standard deviation
* LogNormal: Expects mean and standard deviation
* RandomCdfLogNormal: Expects mean and standard deviation
* PercentileLogNormal: Expects mean, standard deviation and probability

![Stochastics](StochasticsClassdiagram.svg)

---

\section dev_framework_modules_world World
\subsection dev_framework_modules_world_interfacesadapter Interfaces and Adapter

![World Interfaces and Adapter](World.svg)
