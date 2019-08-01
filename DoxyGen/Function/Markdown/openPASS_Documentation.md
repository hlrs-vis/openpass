\page openpass_documentation openPASS Documentation 
\mainpage

![Official openPASS logo](openPASS_logo.png)

\tableofcontents

\section Introduction

The OpenPASS (Open Platform for Assessment of Safety Systems) tool is a developed framework for the simulation of interaction between traffic participants to evaluate and parametrize active safety systems. 
The simulation is based on a specific situation configuration and can contain several simulation runs, which differ due to random parameters.

The software suite of openPASS started as a set of stand-alone applications, which can be installed and configurated individually.
Over time, especially the graphical user interface evolved to a single entry point, enabling the average user to use openPASS as a "monolitic" tool.

Under the hood, the system consits of the following major components:
- Graphical User Interface (GUI)
- [OpenPASS Slave](@ref sim)
- [Visualization](@ref visu)
- [PostProcessing](@ref pp)

The GUI let the user configure the simulation and generate configuration files from all set parameters.
Based on these the simulation core calculates different simulation runs and compile trace files. 
These files can be used by the Visualisation to review single simulation runs with a 2D or 3D viewer. 
Post Processing finally allows to analyze complex experiments.

The [user guide](@ref userman) explains how to use the software system from the users perspective.
It describes the graphical user interface with its functionality and posibilities to analyze the simulation results within the visualisation and post processing.

\section Glossary

[//]: <> (Please divide in abbreviations and terms.)

\subsection list_of_abbreviations List of abbreviations

[//]: <> (If you have an explanation for abbreviation please write it to a second ":" one line below.)

ADAS
* Advanced Driving Assistance Systems

AEB
* Autonomous Emergency Brake
* preventive emergency braking in danger (support or independent launch)

COG
* Center of gravity

OD
* OpenDrive

openPASS
* Open Platform for Assessment of Safety Systems

OSI
* Open Simulation Interface

TTC
* Time to collision

\subsection list_of_terms List of terms

Channel
* Connects components within an agent (using unique ID)

Component
* A module that is part of an agent's equipment

Dynamics
* Calculation of dynamic parameters, e.g. position of Agent

Model
* An abstract representation of a real object which might omit details e.g. ADAS, driving dynamics, pedestrian, environmental conditions. In the PreCASE framework, a model consists of one or more modules as well as channels connecting them.

Module
* A dynamic library that is loaded and whose interface functions are called by the framework. Modules contain models or other individual functionality necessary for the simulation. Modules are exchangeable and can be fitted to various purposes ensuring ahigh flexibility of the framework.

PreCASE
* Framework for the Simulative Evaluation of Active Safety Systems in Vehicles (OpenPASS predecessor).

Scenario 
* A set of similar traffic situations.

\section About

The openPASS documentation was written by in-tech GmbH.  
![Copyright (c) 2017-2019 in-tech GmbH ](logo_in-tech.jpg)
