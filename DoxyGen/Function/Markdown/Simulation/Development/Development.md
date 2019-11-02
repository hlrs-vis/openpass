\page dev Development

\tableofcontents

\section dev_tech Technologies

The OpenPASS Slave is written in C++11 and has the following dependencies:

Simulation Core:
- Qt (v5.12.2) -- mainly XML reading, dynamic library access methods, file writing
- Boost Geometry (v1.63)
- Open system interface (v3.0.1)
- Protocol Buffers (v3.6.1)
- ZeroMQ (v5.1.2)

Additional dependencies (Simulation Core Tests):
- Boost system and filesystem (v1.63)
- gtest/gmock (v1.8.1)

\section dev_concepts Concepts

This section gives an overview of the fundamental concepts of the OpenPASS Slave and gives valuable entrypoints to the related chapters of the documentation.

\subsection dev_concepts_modulecomposition Module Composition

An agent in openPASS is composed of multiple modules, which are connected by corresponding signals.
As shown in the next figure, the modules can be roughly divided into the groups drivers, vehicle components, algorithms, dynamic modules, and prioritizers. 
Thereby, the modules themself can consist of several submodules, such as sensor (reading from an interface) and action (writing to an interface).
![Modules for longitudinal and lateral dynamics](DynamicsModules.png)

For information on the content of the signals, please refer to the correspondig developer documentation:
- AccelerationSignal
- DynamicsSignal
- LateralSignal
- LongitudinalSignal
- SteeringSignal


---

By statistic means, based on corresponding probabilites defined in the `slaveConfig.xml`, each individual agent is composed from a superset of all possible (valid) combinations, which is defined by the `systemConfigBlueprint.xml` (see also the corresponding sections within [Input & Output](@ref io)).
This config defines all available framework modules and agent modules and connects them by corresponding channels, which in turn have a specific signal type (refer to section [Channel IDs](@ref io_input_channelids) and [Signal Prioritizer](@ref dev_agent_modules_signalprioritizer) for more information).

The next figure gives an exhautive overview over the current superset:
![Components and channel communication](ComponentsChannelCommunicationDiagram.svg)

For detailed information on the individual modules refer to the sections [systemConfigBlueprint.xml](@ref io_input_systemconfigblueprint), [Framework Modules](@ref dev_framework_modules), and [Agent Modules](@ref dev_agent_modules).

---

\subsection dev_concepts_coordinatesystems Coordinate Systems

OpenPass uses 3 different coordinate systems.
All systems rotate counterclockwise and use radian.

\subsubsection dev_concepts_coordinatesystems_world World Coordinate System

The world coordinate system is absolute. It is the parent coordinate system and all other systems are relative to the world coordinate system.
This system consists of `x`, `y` and `z` (`z` is currently unused).

![World Coordinate System](WorldCoordinateSystem.png)

\subsubsection dev_concepts_coordinatesystems_road Road Coordinate System

The road coordinate system is relative to the position of the road in the world coordinate system. 
It is split into `s` and `t` coordinates, where `s` describes the longitudinal position along the road. 
At the beginning of the road `s=0`, increasing with the downstream direction of the road.
The coordinate `t` describes the lateral position relative to the center of the road, where `t>0` indicates left side and `t<0` right side of the road with respect to the road direction. 

![Road Coordinate System](RoadCoordinateSystem.png)

\subsubsection ddev_concepts_coordinatesystems_agent Agent Coordinate System

The agent coordinate system is relative to the position of the agent in the world coordinate system. 
It uses longitudinal, lateral and height (height currently unused).
The system originates at the `reference point`, which is located at the center of the rear axle. 
The longitudinal-Axis is parallel to the length of the car and the lateral-Axis is parallel to the width of the car.

![Agent Coordinate System](AgentCoordinateSystem.png)

\section dev_tools Tools

OpenPASS offer several common tools to prevent "reinventing the wheel".
Please refer to the inline documentation for an exhaustive list.
In addition, this section gives additional insights on part, which cannot be easily explained.

\subsection dev_tools_commonTools_ttcttb Time to collision / Time to brake

For a given opponent (vehicle in front) with respect to the current ego vehicle, the method `TrafficHelperFunctions::WillCrash` calculates if a collision is imminent.
Thereby, it is assumend that the acceleration of the opponent stays constant and that the ego slows down with a given braking deceleration.  

**Sequence:**  
1. Check for a collision during the reaction time of the ego driver.
2. Call `TrafficHelperFunctions::WillCrashDuringBrake` to check for a collision during brake.  
   This is done by intersecting the braking trajectory of both vehicles, given by `s(t)=s_0+t*v_0+t^2*a/2`. 
   A collision occurs when there is an intersection point between `t=0` and the time until the ego vehicle stops.

![Braking trajectory for v_Ego=30, a_Ego=-8, v_Front=9, a_Front=-3](WillCrashIllustration.png)
