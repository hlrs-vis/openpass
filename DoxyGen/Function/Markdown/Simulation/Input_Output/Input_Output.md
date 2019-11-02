\page io Input & Output

\tableofcontents

\section io_input Input

In this section all input files and their contents are described. All input files need to be placed in the "/configs" subfolder.
Several tags of some input files are optional. But all attributes related to a tag are required, if this tag is used.

Obligatory input files are:
systemConfigBlueprint.xml, slaveConfig.xml, ProfilesCatalog.xml, Scenario.xosc, SceneryConfigruation.xodr, VehicleModelsCatalog.xosc and PedestrianModelsCatalog.xosc.

Optional input files are:
SystemConfig files and Trajectory files (only required if specified in the slaveConfig.xml).

For demonstration purposes all files of an AEB scenario have been added as an example.

[//]: <> (Please refer to each section!)
* [systemConfigBlueprint.xml](\ref io_input_systemconfigblueprint)
* [slaveConfig.xml](\ref io_input_slaveconfig)
* [ProfilesCatalog.xml](\ref io_input_profilescatalog)
* [Scenario.xosc](\ref io_input_scenario)
* [SceneryConfiguration.xodr](\ref io_input_scenery)
* [VehicleModelsCatalog.xosc](\ref io_input_vehiclemodels)
* [PedestrianModelsCatalog.xosc](\ref io_input_pedestrianmodels)
* [Trajectory](\ref io_input_trajectory)
* [System Configuration](\ref io_input_systemconfig)

\subsection io_input_systemconfigblueprint systemConfigBlueprint.xml

This file contains the possible agent modules and channels of a dynamically generated agent. The content of this file should only be adjusted by experienced users with knowledge of the simulation architecture. The SystemConfigBlueprint is a special SystemConfig and has the same schema. Only the system with id 0 is used for generating dynamic agents.

_AgentComponents_
All components are listed here. An agent consists of a subset of this components.

|Attribute|Description|
|---------|-----------|
|Id|Used as key by the simulation to find the component|
|Priority|The module with the highest priority value gets executed first by the scheduler|
|Offset|Delay for the trigger method of each component in ms|
|Cycle|Interval in which the component gets triggered by the scheduler in ms|
|Response|Delay for the UpdateOutput method of each component in ms|
|Library|Library name of this component|
|Parameters|Parameters used by the component|

Example:
This example describes the Sensor_Driver module.

```xml
<component>
    <id>Sensor_Driver</id>
        <schedule>
            <priority>490</priority>
            <offset>0</offset>
            <cycle>100</cycle>
            <response>0</response>
        </schedule>
        <library>Sensor_Driver</library>
        <parameters/>
</component>
```

\subsubsection io_input_modulepriorities Priorities

Please refer to the [Components and channel communication diagram]]\ref dev_concepts_modulecomposition) for assignment of a proper priority.
Based on the signal flow, input relevant components like sensors need to be executed first. They provide data for consuming components (algorithms) like ADAS and drivers.
Data is then handled by algorithms like Algorithm_Lateral.
Output-relevant modules like dynamics and actions are executed last.

Prioritizer can be applied on different levels depending on the modules/data they need to handle levels as following can be defined:
Level 1 describes data produced by ADAS and drivers. 
Level 2 describes data output by vehicle dynamic controllers.
Level 3 describes data delivered by dynamics.

Priorities can be grouped (coarse) as following:
Highest number indicates highest priority.

| Scope                      | Range     |
|----------------------------|-----------|
| Parameters                 | 500       |
| Sensor                     | 350...399 |
| Event                      | 330       |
| DriverModels               | 310       |
| ADAS                       | 250...299 |
| ComponentController        | 200       |
| Prioritizer (Lvl. 1)       | 150...199 |
| VehicleDynamicsControllers | 100...149 |
| Prioritizer (Lvl. 2)       | 75...99   |
| Dynamics                   | 50...74   |
| Prioritizer (Lvl. 3)       | 25...49   |
| Updater                    | 0...24    |

The table below can be used as orientation when a new module is introduced.

|Name|Library|Priority|Scope|Note|
|---|-----------|-----------|-----------|-----------|
| ParametersAgentModules| ParametersAgent| 500 | Parameters | Sets all init-data and is updated cyclically |
| AlgorithmAgentFollowingDriverModel | AlgorithmAgentFollowingDriverModel | 310 | DriverModels | - |
| AEB | AlgorithmAutonomousEmergencyBraking | 250 | ADAS | - |
| ComponentController | ComponentController | 200 | ADAS | Manages vehicle component states with regard to other vehicle component states and conditions and in response to events. |
| PrioritizerLaterDriver | SignalPrioritizer | 150 | Prioritizer | - |
| PrioritizerAccelerationDriver | SignalPrioritizer | 150 | Prioritizer | - |
| PrioritizerTurningIndicator | SignalPrioritizer | 150 | Prioritizer | - |
| PrioritizerLateralVehicleComponents | SignalPrioritizer | 150 | Prioritizer | - |
| PrioritizerAccelerationVehicleComponents | SignalPrioritizer | 150 | Prioritizer | - |
| LimiterAccelerationVehicleComponents | LimiterAccelerationVehicleComponents | 120 | VehicleDynamicsControllers | - |
| AlgorithmLateralVehicleComponents | AlgorithmLateralVehicleComponents | 100 | VehicleDynamicsControllers | - |
| AlgorithmLateralDriver | AlgorithmLateralDriver | 100 | VehicleDynamicsControllers | - |
| AlgorithmLongitudinalVehicleComponents | AlgorithmLongitudinalVehicleComponents | 100 | VehicleDynamicsControllers | - |
| AlgorithmLongitudinalDriver | AlgorithmLongitudinalDriver | 100 | VehicleDynamicsControllers | - |
| PrioritizerSteering | SignalPrioritizer | 75 | Prioritizer | - |
| PrioritizerLongitudinal | SignalPrioritizer | 75 | Prioritizer | - |
| DynamicsCollision | DynamicsCollision | 50 | Dynamics | - |
| DynamicsRegularDriving | DynamicsRegularDriving | 50 | Dynamics | - |
| DynamicsTrajectoryFollower | DynamicsTrajectoryFollower | 50 | Dynamics | - |
| ProritizerDynamics | SignalPrioritizer | 25 | Prioritizer | - |
| SensorRecordStateModule | SensorRecordState | 2 | Updater | Since values are "frozen" f. current time step, logging can be placed anywhere |
| ActionLongitudinalDriverModules | ActionLongitudinalDriver | 3 | Updater | Will be expanded to ActionPrimaryDriverTasks |
| ActionSecondaryDriverTasksModules | ActionSecondaryDriverTasks | 3 | Updater | - |
| AgentUpdater | AgentUpdater | 1 | Updater | - |


\subsubsection io_input_channelids Channel-Ids 


Channels allow components to communicate with each other. 
The signalflow is set explicitly via a channel-Id of 4 digits (see also [Components and channels communication diagram](/ref dev_concepts_modulecomposition)).

The first two numbers define the sending module (XX 00).
The other two digits define the type of signal that is sent (00 XX).

Signals as well as modules can be grouped to allow explicit numbering (see tables below).

Channel-Ids between Sensor and SensorFusion are an exception to this rule. For sensor/sensor fusion communication channel-ids are 9900 + x (incremented for every new sensor)

Example:
PrioritizerAccelerationDriver -> AlgorithmLongitudinalDriver with signal of type AccelerationSignal: 1813.


**Ids for Modules (first two digits)**


Index range for module groups:

| Group             | Id      |
|-------------------|---------|
| Dynamics          | 1...10  |
| Algorithm         | 11...30 |
| DriverTasks       | 31...40 |
| Driver            | 41...50 |
| VehicleComponent  | 51...80 |
| Special           | 81...89 |
| Sensor            | 91...99 |

With corresponding defined indices :

|Module                                     |Group              |Id  |
|-------------------------------------------|-------------------|----|
| AgentUpdater                              | Dynamics          | 1  | 
| Dynamics_TrajectoryFollower               | Dynamics          | 2  | 
| Dynamics_RegularDriving                   | Dynamics          | 3  | 
| Dynamics_Collision                        | Dynamics          | 4  | 
| PrioritizerDynamics                       | Dynamics          | 5  | 
| Algorithm_LongitudinalVehicleComponent    | Algorithm         | 11 | 
| Algorithm_LongitudinalAfdm                | Algorithm         | 12 |
| Algorithm_LateralVehicleComponent         | Algorithm         | 14 | 
| Algorithm_LateralVehicleAfdm              | Algorithm         | 15 | 
| LimiterVehicleLongitudinal                | Algorithm         | 17 | 
| PrioritizerLongitudinal                   | Algorithm         | 21 | 
| PrioritizerSteering                       | Algorithm         | 22 | 
| PrioritizerAccelerationVehicleComponents  | Algorithm         | 23 | 
| PrioritizerLateralVehicleComponents       | Algorithm         | 24 | 
| Action_LongitudinalDriver                 | DriverTasks       | 31 | 
| Action_SecondaryDriverTasks               | DriverTasks       | 32 | 
| PrioritizerTurningIndicator               | DriverTasks       | 33 | 
| AlgorithmAgentFollowingDriver             | Driver            | 41 |
| AEB                                       | VehicleComponent  | 52 | 
| VehicleControlUnit                        | Special           | 83 |
| Parameter_Vehicle                         | Sensor            | 92 | 
| Sensor_Driver                             | Sensor            | 95 | 

**Ids for Signals (last two digits)**

Index range for signal groups:

| Group         | Id      |
|---------------|---------|
| Dynamics      | 1...10  |
| Algorithm     | 11...30 |
| Special       | 71...80 |
| Sensor        | 81...90 |
| Parameters    | 91...99 |

With corresponding defined indices :

| Signal                        | Group      | Id      |
|-------------------------------|------------|---------|
| Dynamics                      | Dynamics   | 01      | 
| Longitudinal                  | Algorithm  | 11      | 
| Steering                      | Algorithm  | 12      | 
| Acceleration                  | Algorithm  | 13      | 
| Lateral                       | Algorithm  | 14      |
| SecondaryDriverTasks          | Algorithm  | 19      |
| SensorDriver                  | Sensor     | 81      |
| ParametersVehicle             | Parameters | 92      |

---

\subsection io_input_slaveconfig slaveConfig.xml

This file is necessary for the simulation to work. It describes all user configurable parameters of the experiment. 
Several parameters depend on probabilities. Each invocation then rolls for said probabilities. All probabilities need to add up to 1.0.

The slaveConfig.xml consists of the following parts:

[//]: <> (Please refer to each section!)
* [ExperimentConfig](\ref io_input_slaveconfig_experimentconfig)
* [ScenarioConfig](\ref io_input_slaveconfig_scenarioconfig)
* [EnvironmentConfig](\ref io_input_slaveconfig_environmentconfig)
* [TrafficConfig](\ref io_input_slaveconfig_trafficconfig)

\subsubsection io_input_slaveconfig_experimentconfig ExperimentConfig

This section contains information about the general experiment setup. These values are not specific to a single invocation.

| Tag                 | Description                                                                                      | Dependence                      |
|---------------------|--------------------------------------------------------------------------------------------------|---------------------------------|
| ExperimentId        | Id of the experiment                                                                             | Obligatory                      |
| NumberOfInvocations | Number of invocation in the experiment. For each invocation probabilities are rerolled           | Obligatory                      |
| RandomSeed          | Random seed for the entire experiment. The seed must be within the bounds of unsigned integers   | Obligatory                      |
| Libraries           | Name of the core module Libraries to use. If a name is not specified the default name is assumed | Obligatory                      |
| LoggingGroups       | List of logging groups to be activated                                                           | Obligatory (empty list allowed) |

Example:
This experiment has the id 0.
During this experiment the simulation runs 10 invocations and the first invocation starts with the random seed 5327.
It produces output with all possible agent attributes logged for the visualization.

```xml
<ExperimentConfig>
    <ExperimentID>0</ExperimentID>
    <NumberOfInvocations>10</NumberOfInvocations>
    <RandomSeed>5327</RandomSeed>
    <Libraries>
        <WorldLibrary>World_OSI</WorldLibrary>
        <ObservationLibrary>Observation_Log</ObservationLibrary>
        <SpawnPointLibrary>SpawnPoint_OSI</SpawnPointLibrary>
    </Libraries>
    <LoggingGroups>
        <LoggingGroup>Trace</LoggingGroup>
        <LoggingGroup>Visualization</LoggingGroup>
        <LoggingGroup>RoadPosition</LoggingGroup>
        <LoggingGroup>Sensor</LoggingGroup>
        <LoggingGroup>Driver</LoggingGroup>
    </LoggingGroups>
</ExperimentConfig>
```

\paragraph io_input_slaveconfig_experimentconfig_logginggroups LoggingGroups

The LoggingGroups configuration allows to enable or disable predefined sets of values to be logged to the ```simulationOutput.xml ```.
Currently, the following
Groups are defined:

- **Trace**
    - X coordinate
    - Y coordinate
    - Yaw angle

- **Visualization**
    - Brake light
    - Velocity ego
    - Indicator state
    - Acceleration ego
    - Light status

- **RoadPosition**
    - Lane
    - S coordinate
    - T coordinate
    - Secondary lanes

- **Vehicle**
    - Steering angle
    - Steering angle rate
    - Steering torque
    - Side slip angle
    - Yaw rate
    - Lateral acceleration
    - Accelerator pedal position
    - Decelerator pedal position
    - Motor torque
    - RPM
    - Gear
    - Status ADAS

- **Sensor**
    - Number of detected agents
    - List of detected agents

- **Driver**
    Depends on driver module


---

\subsubsection io_input_slaveconfig_scenarioconfig ScenarioConfig

This section contains information about the scenario setup for the experiment. This information does not change between invocations.

| Tag              | Description               | Dependence |
|------------------|---------------------------|------------|
| OpenScenarioFile | Name of the scenario file | Obligatory |

Example:
This experiment uses the "HighwayScenario.xosc" scenario file.
```xml
<ScenarioConfig Name="Default">
    <OpenScenarioFile>HighwayScenario.xosc</OpenScenarioFile>
</ScenarioConfig>
```

---

\subsubsection io_input_slaveconfig_environmentconfig EnvironmentConfig

This section contains information about the world and the general environment inside the simulation. Every invocation re-rolls the environment probabilities.
All probabilities need to add up to 1.0.

|Tag|Description|Dependence|
|---|-----------|----------|
|TimeOfDay|Number representing the time of day in hours ranging from 1-24. Currently not used.|At least one entry is required|
|VisibilityDistance|Range agents can see in m|At least one entry is required|
|Friction|Friction on the road. Currently not used|At least one entry is required|
|Weather|Weather as string. Currently not used|At least one entry is required|

Example:
Every invocation has the time set to 15:00.
In 70% of all invocation drivers can see 125 meter and for the other 30% of invocations the drivers can see 250 meter.
Every invocation has a friction of 0.3.
Every invocation has sunny weather.

```xml
<EnvironmentConfig Name="Default">
    <TimeOfDays>
        <TimeOfDay Value="15" Probability="1.0"/>
    </TimeOfDays>
    <VisibilityDistances>
        <VisibilityDistance Value="125" Probability="0.7"/>
        <VisibilityDistance Value="250" Probability="0.3"/>
    </VisibilityDistances>
    <Frictions>
        <Friction Value="0.3" Probability="1.0"/>
    </Frictions>
    <Weathers>
        <Weather Value="Sunny" Probability="1.0"/>
    </Weathers>
</EnvironmentConfig>
```

---

\subsubsection io_input_slaveconfig_trafficconfig TrafficConfig

This section contains parameters regarding the traffic and distribution of agent profiles for all common vehicles. The traffic parameters are rerolled once for every invocation. The RegularLane and RightMostLane parameters are rolled for each common agent. All probabilities must add up to 1.0.

|Tag|Description|Dependence|
|---|-----------|----------|
|TrafficVolume|This value determines how many cars per hour and per lane are expected|At least one entry is required|
|PlatoonRate|Platoon rate is rolled for every common agent. If the agent is platooning they are spawned closer together|At least one entry is required|
|Velocity|Average velocity of the right most lane in m/s|At least one entry is required|
|Homogenity|Determines how much the average velocity drops per lane in km/h|At least one entry is required|
|RegularLane|Determines the agent profiles used for common agents in regular lanes|At least one entry is required|
|RightMostLane|Determines the agent profiles used for common agents in the right most lane|At least one entry is required|

Example:
In this experiment every invocation spawns one agent per second for each initial lane.
50% of the agents are platooning initially.
In 30% of all invocations the right lane has an average velocity of 30 m/s and in the other 70% the velocity is 40 m/s.
For all invocations the average lane velocity drops by 20% from left to right.
60% of the common agents on regular lanes have the "LuxuryClassCarAgent" AgentProfile. The other 40% have the "MiddleClassCarAgent" AgentProfile.
All common agents on the right most lane have the "TruckAgent" AgentProfile.

```xml
<TrafficConfig Name="Default">
    <TrafficParameter>
        <TrafficVolumes>
            <TrafficVolume Value="3600" Probability="1.0"/>
        </TrafficVolumes>
        <PlatoonRates>
            <PlatoonRate Value="0.5" Probability="1.0"/>
        </PlatoonRates>
        <Velocities>
            <Velocity Value="30.0" Probability="0.3"/>
            <Velocity Value="40.0" Probability="0.7"/>
        </Velocities>
        <Homogenities>
            <Homogenity Value="0.8" Probability="1.0"/>
        </Homogenities>
    </TrafficParameter>
    <RegularLane>
        <AgentProfile Name = "LuxuryClassCarAgent" Probability = "0.6"/>
        <AgentProfile Name = "MiddleClassCarAgent" Probability = "0.4"/>
    </RegularLane>
    <RightMostLane>
        <AgentProfile Name = "TruckAgent" Probability = "1.0"/>
    </RightMostLane>
</TrafficConfig>
```

---

\subsection io_input_profilescatalog ProfilesCatalog.xml

The ProfilesCatalog contains all AgentProfiles referenced by the the Scenario entities and its sub profiles, i.e. DriverProfiles, VehicleProfiles, VehicleProfiles and SensorProfiles. Several parameters depend on probabilities. Each invocation then rolls for said probabilities. All probabilities except ComponentProfiles of an VehicleProfile need to add up to 1.0. For more info see the corresponding section.

* [AgentProfiles](\ref io_input_profilescatalog_agentprofiles)
* [DriverProfiles](\ref io_input_profilescatalog_driverprofiles)
* [VehicleProfiles](\ref io_input_profilescatalog_vehicleprofiles)
* [VehicleComponentProfiles](\ref io_input_profilescatalog_vehiclecomponentprofiles)
* [SensorProfiles](\ref io_input_profilescatalog_sensorprofiles)
  

---

\subsubsection io_input_profilescatalog_agentprofiles AgentProfiles

In this section all AgentProfiles are defined. An AgentProfile is either static or dynamic. A static AgentProfile consists of a SystemConfig and a VehicleModel.
A dynamic AgentProfile specifies the composition of the agent according to certain probabilities.
Here the initial driver and the vehicle profile of an agent get specified.
At least one AgentProfile is required. But every AgentProfile referenced in the used Scenario.xosc file or [TrafficConfig](\ref io_input_slaveconfig_trafficconfig) must exist.
All probabilities must add up to 1.0.

|Tag|Description|Dependence|
|---|-----------|----------|
|DriverProfile|Name of the DriverProfile|At least one entry is required|
|VehicleProfiles|All different ADASProfiles can be referenced here. For further information see [VehicleProfiles](\ref io_input_slaveconfig_vehicleprofiles)|Optional|

Example:
In this experiment the ego agent is defined by the system with Id 0 in systemConfig.xml and the VehicleModel car_bmw_7.
Every LuxuryClassCarAgent has the driver profile "AgentFollowingDriver".
Regarding the vehicle profile 50% have a MINI Cooper and the other 50% drive a BMW 7.

```xml
<AgentProfiles>
    <AgentProfile Name="EgoAgent" Type="Static">
        <System>
            <File>systemConfig.xml</File>
            <Id>0</Id>
        </System>
        <VehicleModel>car_bmw_7</VehicleModel>
    </AgentProfile>
    <AgentProfile Name="LuxuryClassCarAgent" Type="Dynamic">
        <DriverProfiles>
            <DriverProfile Name="AgentFollowingDriver" Probability="1.0"/>
        </DriverProfiles>
        <VehicleProfiles>
            <VehicleProfile Name="MINI Cooper" Probability="0.5"/>
            <VehicleProfile Name="BMW 7" Probability="0.5"/>
        </VehicleProfiles>
    </AgentProfile>
</AgentProfiles>
```

---

\subsubsection io_input_profilescatalog_driverprofiles DriverProfiles

This section contains all driver profiles used by the simulation. At least one driver profile is required.

[//]: <> (Please refer to each section!)
* [AlgorithmAgentFollowingDriverModel](\ref io_input_profilescatalog_profilescatalog_agentfollowingdrivermodel)

\paragraph io_input_slaveconfig_profilescatalog_agentfollowingdrivermodel AlgorithmAgentFollowingDriverModel

This driver type adapts its velocity to an agent in front and holds a desired velocity if there's no front agent available (like adaptive cruise control). The lateral guidance always keeps the agent in the middle of the lane.

```xml
<DriverProfile Name = "AgentFollowingDriver">
    <String Key="Type" Value="AlgorithmAgentFollowingDriverModel"/>
</DriverProfile>
```

\subsubsection io_input_profilescatalog_vehicleprofiles VehicleProfiles

This sections contains all vehicle profiles. Every VehicleProfile used by [AgentProfiles](\ref io_input_profilescatalog_agentprofiles) must be listed here.

```xml
 <VehicleProfiles>
    <VehicleProfile Name="BMW 7">
        <Model Name="car_bmw_7"/>
        <Components>
               <Component Type="AEB">
                <Profiles>
                    <Profile Name="aeb" Probability="0.5"/>
                </Profiles>
                <SensorLinks>
                    <SensorLink SensorId="0" InputId="Camera"/>
                </SensorLinks>
               </Component>
        </Components>
        <Sensors>
            <Sensor Id="0">
                <Position Name="Default" Longitudinal="0.0" Lateral="0.0" Height="0.5" Pitch="0.0" Yaw="0.0" Roll="0.0"/>
                <Profile Type="Geometric2D" Name="Standard"/>
            </Sensor>
        </Sensors>
    </VehicleProfile>
    ...
 </VehicleProfiles>
```

|Attribute|Description|
|---------|-----------|
|Name|Name of the vehicle profile|
|Components|Lists all ADAS and other components in the vehicle|
|Sensors|Lists all sensors in the vehicle|

\paragraph  io_input_profilescatalog_vehicleprofiles_components Components

|Attribute|Description|
|---------|-----------|
|Type|Type of the component. Must be identical to the component name in the SystemConfigBlueprint|
|Profile|All possible profiles of the component with probabilities. Probabilities do not need to add up to 1.|
|SensorLinks|Defines which sensor this component uses as input|

If the probabilities of the profiles do not add up to 1, it means that the vehicle possibly does not have this type of component

\paragraph  io_input_profilescatalog_vehicleprofiles_sensors Sensors

|Attribute|Description|
|---------|-----------|
|Id|Identifier for the sensor used by the SensorLink definition of the components|
|Position|Position of the sensor in the vehicle in relative coordinates|
|Profile|All possible profiles of the sensor with probabilities.|

---

\subsubsection io_input_profilescatalog_vehiclecomponentprofiles VehicleComponentProfiles

This sections contains all driver assistance systems and other vehicle components and their parameter sets. 
Every VehicleComponentProfile used by [AgentProfiles](\ref io_input_profilescatalog_agentprofiles) must be listed here.
Currently there are three driver assistance systems:

[//]: <> (Please refer to each section!)
* [AEB](\ref io_input_profilescatalog_adasprofiles_aeb)
* [DynamicsTrajectoryFollower](\ref io_input_profilescatalog_adasprofiles_trajectoryfollower)

\paragraph io_input_profilescatalog_adasprofiles_aeb AEB

For details about the system go to [AEB - Module](\ref dev_modules_adas_aeb).

|Attribute                             |Description                                                                            |
|--------------------------------------|---------------------------------------------------------------------------------------|
|CollisionDetectionLongitudinalBoundary|Additional length in m added the vehicle boundary when checking for collision detection|
|CollisionDetectionLateralBoundary     |Additional width in m added the vehicle boundary when checking for collision detection |
|TTC                                   |Time to collision which is used to trigger AEB in seconds                              |
|Acceleration                          |Braking acceleration when activated                                                    |

```xml
<VehicleComponentProfiles>
        <VehicleComponentProfile Type="AEB" Name="AEB1">
            <Double Key="CollisionDetectionLongitudinalBoundary" Value="4.0"/>
            <Double Key="CollisionDetectionLateralBoundary" Value="1.5"/>
            <Double Key="TTC" Value="2.0"/>
            <Double Key="Acceleration" Value="-2"/>
         </VehicleComponentProfile>
    ...
</VehicleComponentProfiles>
```

---

\paragraph io_input_profilescatalog_adasprofiles_trajectoryfollower DynamicsTrajectoryFollower

This vehicle component aligns the vehicle to a certain trajectory. Every vehicle component profile of this type needs a [Trajectory](\ref io_input_trajectory) file.

All attributes are required. The profile name may be chosen freely.

|Attribute|Description|
|---------|-----------|
|TrajectoryFile|The file defining the Trajectory to follow|
|AutomaticDeactivation|If true, the trajectory follower relinquishes control of the vehicle after the final instruction in the TrajectoryFile.|
|EnforceTrajectory|If true, the trajectory follower overrides external input related to the vehicle's travel.|

```xml
<VehicleComponentProfiles>
    <VehicleComponentProfile Type="DynamicsTrajectoryFollower" Name="BasicTrajectoryFollower">
            <String Key="TrajectoryFile" Value="Trajectory.xml"/>
            <Bool Key="AutomaticDeactivation" Value="true"/>
            <Bool Key="EnforceTrajectory" Value="true"/>
        </VehicleComponentProfile>
</VehicleComponentProfiles>
```

---

\subsubsection io_input_profilescatalog_sensorprofiles SensorProfiles

This section describes the possible sensor profiles of all vehicle profiles.

```xml
<SensorProfiles>
    <SensorProfile Name="Standard" Type="Geometric2D">
        <Double Key="OpeningAngleH" Value="0.35"/>
        <Double Key="DetectionRange" Value="300"/>
        <Double Key="LatencyMean" Value="0"/>
        <Double Key="LatencySD" Value="0"/>
        <Double Key="LatencyMin" Value="0"/>
        <Double Key="LatencyMax" Value="0"/>
        <Double Key="FailureProbability" Value="0"/>
        <Bool Key="EnableVisualObstruction" Value="false"/>
    </SensorProfile>
    ...
</SensorProfiles>
```

|Attribute|Description|
|---------|-----------|
|Name|Name of the profile|
|Type|Type of the sensor. Currently there is only the Geometric2D|

Sensor profile for different sensor types can have the same name, but the names of profile of the same sensor type must be unique.

The parameters depending on the sensor type. The SensorGeometric2D needs the following:

|Parameter|Description|
|---------|-----------|
|LatencyMean|Mean value of the latency|
|LatencySD|Standard deviation of the latency|
|LatencyMin|Minimum value of the latencyy|
|LatencyMax|Maximum value of the latencyy|
|FailureProbability|How likely the sensor fails to detected an object|
|DetectionRange|Range in which the sensor can detect objects|
|OpeningAngleH|Angle of the circular sector|
|EnableVisualObstruction|Wether this sensor uses [visual obstruction](\ref dev_agent_modules_geometric2d_obstruction)|

---

\subsection io_input_scenario Scenario.xosc

This file is necessary for the simulation to work. It describes the initial setup of the scenario. The initial position and dynamics of ego and scenario agents are listed here. This file also contains the EventDetector and Manipulators used to enforce certain behavior in the scenario. This file is modeled after the OpenSCENARIO standard. But to match the needs of the simulator some additions were made.

**Entities**
This section describes all entity objects participating in the simulation.
An object with the name "Ego" has to exist and is always mapped to the ego agent in the simulation.
Currently, the only reference type is `CatalogReference` which refers to a filename and AgentProfile name within this file.

Example:
```xml
<Entities>
    <Object name="Ego">
        <CatalogReference catalogName="ProfilesCatalog.xml" entryName="MiddleClassCarAgent"/>
    </Object>
    <Object name="ScenarioAgent1">
        <CatalogReference catalogName="ProfilesCatalog.xml" entryName="MiddleClassCarAgent"/>
    </Object>
    <Object name="ScenarioAgent2">
        <CatalogReference catalogName="ProfilesCatalog.xml" entryName="MiddleClassCarAgent"/>
    </Object>
</Entities>
```

**Init**
Here the initial position and dynamics of ego and scenario agents are defined.
One ego agent is required. Scenario agents are optional.

One agent gets defined by a complete _Private_ tag.
The _Position_ tag with a valid _Lane_ tag is Obligatory otherwise the agent can **not** be placed.
The _Longitudinal_ tag is optional.
The _Stochastics_ tag is optional and can be added per agent for up to 4 parameter. If the stochastics tag is used the initial value is being used as mean value.


Current possible stochastic variables: 

|Tag|Description|
|---|-----------|
| "s" | Longitudinal distance in m |
| "offset" | Lateral distance in m |
| "velocity" | start velocity in m/s |
| "rate" | start acceleration in m/s^2 |

|Tag|Attribute|Description|
|---|---------|-----------|
|Private|object|Name of the agent defined. Those need to be unique. In order to define the ego agent set this value to "Ego"|
|Lane|roadId|Id of the road the agent gets spawned on|
|Lane|s|s-coordinate of the agent pivot point in relation to the road|
|Lane|laneId|Id of the lane|
|Lane|offset|t-coordinate of the agent pivot point in relation to the current lane. The pivot points must still be inside the defined laneId otherwise it is invalid|
|Stochastics|value|Here is defined which value has a stochastic variation. So far options are: "s", "offset"|
|Stochastics|stdDeviation|Standard deviation|
|Stochastics|lowerBound|Minimum value|
|Stochastics|upperBound|Maximum value|
|Orientation|h|heading angle in radiant. Rotation is counterclockwise|
|Orientation|type|only relative is allowed|
|Dynamics|time|Per time value in ms used to calculate initial velocity|
|Dynamics|distance|Distance made per time in m used to calculate initial velocity|
|Dynamics|rate|Used to describe acceleration. Currently not used by the simulation|

Example:

```xml
<Init>
    <Actions>
        <Private object="ScenarioAgentLeft1">
            <Action>
                <Position>
                    <Lane roadId="1" s="250.0" laneId="-1" offset="-0.2"> <!-- position  -->
                        <Stochastics value="s" stdDeviation="10" lowerBound="20" upperBound="300"/>
                        <Stochastics value="offset" stdDeviation="0.5" lowerBound="60" upperBound="20"/>
                        <Orientation type="relative" h="-0.1"/>
                    </Lane>
                </Position>
            </Action>
            <Action>
                <Longitudinal>
                    <Speed>
                        <Dynamics time="100" distance="1.0" rate="0.5" shape="linear"/>
                        <Stochastics value="velocity" stdDeviation="2.0" lowerBound="0.0" upperBound="40.0"/>
                        <Stochastics value="acceleration" stdDeviation="0.5" lowerBound="0.0" upperBound="1.0"/>
                    </Speed>
                </Longitudinal>
            </Action>
        </Private>
    </Actions>
</Init>
```

---

**Story**

Here the [EventDetectors](\ref dev_framework_modules_eventdetectors) and [Manipulators](\ref dev_framework_modules_manipulators) used by this scenario are defined.
For each combination of EventDetectors and manipulators an according _Sequence_ must be defined.

The _Actors_ tag is mandatory. This can be used to assign targets for the manipulator. If no _Entity_ tags are included, all agents are selected as targets for the manipulator. Names provided to the _Entity_ tags relate to those from the "Init"-tag.

|Tag|Attribute|Description|
|---|---------|-----------|
|Entity|name|Name of the agent as specified in the _Init_ tag|
|Action|name|Name of the Manipulator|
|Condition|name|Type of the event detector|
|SimulationTime|value|Time with which to compare, using rule, to the current simulation time to determine if the event is triggered in seconds|
|SimulationTime|rule|The rule for which to determine if the event is triggered|

Example:

```xml
<Story name="ActivateTFStory">
    <Act name="Act1">
        <Sequence name="ActivateTFSequence" numberOfExecutions="1">
            <Actors>
                <Entity name="Ego"/>
            </Actors>
            <Maneuver name="ActivateTFManuever">
                <Event name="ActivateTFEvent" priority="overwrite">
                    <Action name="ActivateTFAction">
                        <UserDefined>
                            <Command>SetComponentState DynamicsTrajectoryFollower Acting</Command>
                        </UserDefined>
                    </Action>
                    <StartConditions>
                        <ConditionGroup>
                            <Condition name="Conditional">
                                <ByValue>
                                    <SimulationTime value="5.0" rule="greater_than" />
                                </ByValue>
                            </Condition>
                        </ConditionGroup>
                    </StartConditions>
                </Event>
            </Maneuver>
        </Sequence>
        ...
    </Act>
</Story>
```

---

**EndConditions**

Here the end conditions for the simulation are defined. One SimulationTime condition is required. Any other or additional conditions are not supported.

| Tag            | Attribute | Description                    |
|----------------|-----------|--------------------------------|
| SimulationTime | value     | The time in ms to be simulated |

Example:

```xml
<EndConditions>
    <ConditionGroup>
        <Condition name="EndTime" rule="greater_than" edge="rising">
            <ByValue>
                <SimulationTime value="30.0" rule="greater_than" />
            </ByValue>
        </Condition>
    </ConditionGroup>
</EndConditions>
```

---

\subsection io_input_scenery SceneryConfiguration.xodr
This file is necessary for the simulation to work. It describes the road network and the objects of the road for the simulation run. The file is structured according to the OpenDRIVE standard.
The file name can be adjusted as long as it's referred to in the [ScenarioConfig](\ref io_input_slaveconfig_scenarioconfig) of the slaveConfig.xml. But the Visualization can only load files called "SceneryConfiguration.xodr".
If the objects are not openDrive conform the simulation is aborted. The simulator has some additional requirements on top of the openDrive standard, if objects don´t meet the requirements of the simulator they will be ingored.

Those requirements are:

* length must be greater than 0.
* width must be greater than 0.
* radius must be 0.

---

\subsection io_input_vehiclemodels VehicleModelsCatalog.xosc

This file is necessary for the simulation to work. It contains all currently available vehicle models for the simulation. For each vehicle the physical parameters are stored here.
The name of this file needs has to be specified in the scenario configuration file (see \ref io_input_slaveconfig_scenarioconfig).

\subsection io_input_pedestrianmodels PedestrianModelsCatalog.xosc

This file is necessary for the simulation to work. It contains all currently available pedestrian models for the simulation. For each pedestrian the physical parameters are stored here.
The name of this file needs has to be specified in the scenario configuration file (see \ref io_input_slaveconfig_scenarioconfig).

\note Currently, pedestrian models are internally handled the same way as vehicle models. Vehicle specific parameters are set to their default values.

---

\subsection io_input_trajectory Trajectory

Trajectory files are optional, but each trajectory file specified under the [AgentProfiles](\ref io_input_slaveconfig_agentprofiles) of the slaveConfig.xml is required. There are 3 types of trajectories:

[//]: <> (Please refer to each section!)
* Absolute world coordinates
* Absolute road coordinates
* Relative road coordinates

---

**Absolute world coordinates**
This trajectory type stores the x- and y-coordinates and the yaw angle.

Example:

```xml
<Trajectory>
        <TrajectoryCoordinate Time="0" X="112" Y="10" Yaw="0.0"/>
        <TrajectoryCoordinate Time="100" X="112" Y="11" Yaw="0.0"/>
        <TrajectoryCoordinate Time="200" X="113" Y="12" Yaw="0.0"/>
        <TrajectoryCoordinate Time="300" X="113" Y="13" Yaw="0.0"/>
        <TrajectoryCoordinate Time="400" X="114" Y="14" Yaw="0.0"/>
        <TrajectoryCoordinate Time="500" X="115" Y="15" Yaw="0.0"/>
        <TrajectoryCoordinate Time="600" X="116" Y="16" Yaw="0.05"/>
</Trajectory>
```

---

**Absolute road coordinates**
This trajectory type stores the s- and t-coordinates and the heading.
s-coordinates represent the longitudinal distance on the road from the road start point.
t-coordinates represent the lateral displacement on the road related to the middle of the lane.
A absolute road trajectory has to starts with an s-coordinate other than 0.

Example:

```xml
<Trajectory>
        <TrajectoryCoordinate Time="0" S="1008" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="100" S="1009" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="200" S="1010" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="300" S="1011" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="400" S="1012" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="500" S="1013" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="600" S="1014" T="-5" Hdg="0.05"/>
</Trajectory>
```

**Relative road coordinates**
This trajectory type stores the s- and t-coordinates and the heading.
s-coordinates represent the longitudinal distance on the road from the road start point.
t-coordinates represent the lateral displacement on the road related to the middle of the lane.
A relative road trajectory has to start with an s-coordinate of 0.

Example:

```xml
<Trajectory>
        <TrajectoryCoordinate Time="0" S="0" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="100" S="1" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="200" S="2" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="300" S="3" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="400" S="4" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="500" S="5" T="-6" Hdg="0.0"/>
        <TrajectoryCoordinate Time="600" S="6" T="-5" Hdg="0.05"/>
</Trajectory>
```

---

\subsection io_input_systemconfig systemConfig.xml
SystemConfig files are optional.
They discribe static configurations of agents and are therefore an alternative to the dynamic sampling of an agent during runtime.


\section io_output Output

In this section the simulation output files and their contents are described.

[//]: <> (Please refer to each section!)
1. [simulationOutput.xml](\ref io_output_simout)
1. [LogSlave.txt](\ref io_output_logslave)

\subsection io_output_simout simulationOutput.xml
Every successful run generates a simulation output. The output can contain multiple invocations of the same configuration with different random seeds. For each invocation a RunResult is stored, which contains information about the agents and their parameters. As well as run specific events and parameters.

```xml
<SimulationOutput>
    <RunResults>
        <RunResult RunId="0">
            ...
        </RunResult>
        <RunResult RunId="1">
            ...
        </RunResult>
    </RunResults>
</SimulationOutput>
```

The RunResult consist out of the following parts:

[//]: <> (Please refer to each section!)
* [RunStatistics](\ref io_output_simout_runstatistics)
* [Events](\ref io_output_simout_events)
* [Agents](\ref io_output_simout_agents)
* [Cyclics](\ref io_output_simout_cyclics)

\subsubsection io_output_simout_runstatistics RunStatistics

This section contains the RandomSeed and general statistics of the invocation.

|Tag|Description|
|---|-----------|
|RandomSeed|Random seed used for this invocation|
|StopReason|Displays the reason why the simulation stopped. Currently only due to time out|
|StopTime|Currently not used and set to -1|
|EgoAccident|Flag which shows whether the ego agent was involved in an accident|
|NumberOfAccidentsInFollowers|Tracks how many of the follower cars related to the ego had an accident|
|NumberOfArbitraryAccidents|Number of accidents unrelated to the ego vehicle|
|TotalDistanceTraveled|Total traveled distance of all agents|
|EgoDistanceTraveled|Total traveled distance of ego vehicle only|

\subsubsection io_output_simout_events Events

This section contains all events that occurred during the invocation.
Event can either be triggered by an EventDetector, Manipulator or by certain vehicle components.
They are used to track special behavior in the simulation.
The visualization uses those tags to generate markers on the timeline.

|Attribute|Description|
|---------|-----------|
|Id|Identification number|
|Time|Time in ms when the event occured.|
|Source|Name of the component which created the event.|
|Type|Type of the event.|
|Parameters|Depending on the eventtype different parameters describing the event can exist.|

\subsubsection io_output_simout_agents Agents

This section contains some information on how each agent is configured.

|Attribute|Description|
|---------|-----------|
|Id|Identification number|
|AgentTypeGroupName|The agent category. This can either be Ego, Scenario or Common|
|AgentTypeName|Name of the agent profile|
|VehicleModelType|Name of the vehicle model|
|DriverProfileName|Name of the driver profile|


```xml
<Agent Id="0" AgentTypeGroupName="Ego" AgentTypeName="MiddleClassCarAgent" VehicleModelType="car_bmw_7" DriverProfileName="Regular">
```

The Sensors tag lists all sensors of the agent and their parameters.

|Attribute|Description|
|---------|-----------|
|Name|Name of the component|
|Model|Type of the sensor|
|MountingPosLongitudinal|Relative longitudinal position of the sensor in relation to the agent|
|MountingPosLateral|Relative lateral position of the sensor in relation to the agent|
|MountingPosHeight|Relative height of the sensor in relation to the agent|
|OrientationPitch|Pitch rotation of the sensor in relation to the agent|
|OrientationYaw|Yaw rotation of the sensor in relation to the agent|
|OpeningAngleH|Horizontal opening angle of the sensor|
|OpeningAngleV|Vertical opening angle of the sensor|
|DetectionRange|Range how far the sensor reaches in m|

Note: Calculation of visual obstruction is currently supported by the Geometric2D sensor. See [file](systemConfigBlueprint.xml) for configuration.

```xml
<Sensor Name="Geometric2DFront" Model="Geometric2D" MountingPosLongitudinal="0" MountingPosLateral="0" MountingPosHeight="0.5" OrientationPitch="0" OrientationYaw="0" OpeningAngleH="20" OpeningAngleV="-999" DetectionRange="300"/>
```

\subsubsection io_output_simout_cyclics Cyclics

This section contains all logged parameters of the agents per timestep. The Header tag defines the layout of all samples. Each entry of the Header tag consists of the agent id and the name of the logged value. A Sample is contains all information for one specific time step. The values in Sample are ordered according to the definition in Header. If a value is undefined, because an agent did not exist yet or got removed the value is ' '.

Example:
In this example exist two agents with the ids 0 and 1.
Two timesteps were being tracked, one at 0 seconds and one at 100 ms.
Agent 0 has a constant velocity of 30 m/s and starts at the position X: 100 and Y: 50.
Agent 1 has a initial velocity of 40 m/s and starts at the position X: 200 and Y: 50.

```xml
<Cyclics>
    <Header>00:VelocityEgo, 00:XPosition, 00:YPosition, 00:YawAngle, 01:VelocityEgo, 01:XPosition, 01:YPosition, 01:YawAngle</Header>
    <Samples>
        <Sample Time="0">30, 100, 50, 0, 40, 200, 50, 0</Sample>
        <Sample Time="100">30, 103, 50, 0, 40, 204, 50, 0</Sample>
    </Samples>
</Cyclics>
```

\subsection io_output_logslave LogSlave.txt

In this log file errors and warnings of the slave are noted. In a run without any issues this file should be empty.

