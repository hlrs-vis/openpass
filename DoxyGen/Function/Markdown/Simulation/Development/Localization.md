\page localization Localization

\tableofcontents

This page describes the translation of coorindates (x,y) of an agent into RoadCoordinate (s,t), whereas the notion of (s,t) comes from the [OpenDRIVE](www.opendrive.org) standard.  
For more information, please refer to the [OpenDRIVE Format Specification](http://www.opendrive.org/docs/OpenDRIVEFormatSpecRev1.4H.pdf).

\section dev_localization_basics Basics

The following image depics the basic principes of the localization which is rooted on the specifics of the OSI World Layer (aka OWL).

![Localization Basics](LocalizationBasics.svg)

Given is a point P in cartesian coordinates (x/y). 
The task is to assign the point to a lane, defined by a general road geometry and calculate the transformed Point P' in road coordinates (s/t).

Road geometry (based on OpenDRIVE):
 - A road consists of several sections
 - Each section consists of several lanes
 - Within a section, the number of lanes is constant
 - Lanes can have one predecessor and one successor
 - The road follows a reference line, which is the reference for the _s_-coordinate.
   The _t_-coordinate is perpendicular to this line.

OWL specifics:
 - All lanes are sampled, generating a stream of generic quadrilaterals (quad).
 - Within a section, the number of quads per lane is equal, and all lanes have the same length in s.
 - This is realized by a variable sampling width, determined by a constant sampling width along the longest arc.
 - Consequently, points perpendicular to the reference line (_t_-axis) have the same _s_-coordinate.

Note, that the final _t_-coorindate is calculated with respect to a virtual reference line for each lane.
This means, that points on the center of a lane have _t_-coordinate of 0.

\section dev_localization_overview Overview

The following diagram shows a simplified interaction diagram of the major classes used during localization:

![Localization Class Diagram](LocalizationClassDiagram.svg)

Workflow:
- AgentAdapter (implements AgentInterface): Periodically queries \ref World::Localization::BaseTrafficObjectLocator "BaseTrafficObjectLocator" for a \ref World::Localization::Result "Result"
- \ref World::Localization::BaseTrafficObjectLocator "BaseTrafficObjectLocator": Controller for the localization, which orchestrates the individual classes.
  1. Sample bounding box by the \ref World::Localization::PolygonSampler "PolygonSampler" (_not shown in diagram_)
  2. Create new \ref World::Localization::PointAggregator "PointAggregator"
  3. Inject last \ref World::Localization::SearchInitializer "SearchInitializer" into \ref World::Localization::PointLocator "PointLocator"
  4. Query  \ref World::Localization::PointLocator "PointLocator" (having a reference to the \ref World::Localization::PointAggregator "PointAggregator") to locate and store the sampled bounding box points
  5. Store \ref World::Localization::SearchInitializer "SearchInitializer" for next cycle
  6. Use results from \ref World::Localization::PointAggregator "PointAggregator" (see description below) to assign an agent to all touched lanes and vice versa.

\section dev_localization_pointlocator Locating individual points

See also \ref World::Localization::PointLocator "PointLocator":

- Localization starts with the first section at the beginning of a road.
- Each lane of a section is wrapped by a \ref World::Localization::LaneWalker "LaneWalker" object, allowing the Localizer to iterate the quad-stream of a single lane.  
  _Note, that \ref World::Localization::LaneWalker "LaneWalkers" are aware of the road direction._
- LWs are iterated in parallel, meaning that the search starts at s=0 for all lanes and then progress in parallel on all lanes.
- The quads are wrapped by a \ref World::Localization::GeometryProcessor "GeometryProcessor", which executes a fast match check and finally calculates the corresponding road coordiantes (see also \ref World::Localization::PointQuery "PointQuery")
- All found points and their road coordinates a relayed to a \ref World::Localization::PointAggregator "PointAggregator": for further processing.
- If all points are located, the search is stopped, otherwise the search is continued - also on the next logical section if necessary.

__Optimization:__  

Several aspects are taken into account:
- Agents, which stand still, hold their position. No localization is executed.
- Agents follow a continous motion, so their position will be close to their last one.
  Consequently, the first search hit is stored by a \ref SearchInitializer object and used for faster _relocalization_ in consecutive time steps.
- Agents have limited size. Once the first point is found, the localization will stop after reaching the agents diameter (approximated by length + width).

__Remarks:__  

- The continous motion of an agent can abruptly end by a collision, which means that the localizer always take _negative_ motions into consideration.

\section dev_localization_pointaggregation Point Aggregation

See also \ref World::Localization::PointAggregator "PointAggregator":

The task of the PointAggregator is to collect all located points and allow queries on the meta-info given by the type of the points stored (\ref OWL::PointType).
This allows, e.g. to query for the MainLaneId given by the _MainLaneLocator_ point, but also if an agent can be localized at all.

For the lane based queries (e.g. \ref AgentInterface::GetAgentInFront), the agent __must__ be registered with every touched lane.
As agents are sampled as individual points, there is a chance that narrow lanes are covered by the bounding box of the agent, but not by any point (placed left and right of the lane).
Additionally, the bounding box can span over several sections, which means that the number of lanes can vary over _s_.
In order to get a complete list of the touched lanes, framed by the given points, the PointAggregator uses another class called \ref World::Localization::StreamCollector "StreamCollector".

