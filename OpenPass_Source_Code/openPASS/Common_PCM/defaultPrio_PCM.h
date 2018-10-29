/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  default_Prio.h
//! @brief This file contains default priorities for PCM components.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! Enumeration class defining default priorities of PCM components
//-----------------------------------------------------------------------------
enum class PCMdefaultPrio
{
    Algorithm_Selector = 100, //100
    Algorithm_Trajectory = 150, //105
    Dynamics_Collision = 1, //0
    Dynamics = 3, //1
    EgoSensor = 203, //203
    Init_Agent = 0, //0
    Sensor_Collision = 201 //201
};
