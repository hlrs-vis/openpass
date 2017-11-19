/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  observationtypes.h
//! @brief This file defines all types of data which can be observed.
//-----------------------------------------------------------------------------

#ifndef OBSERVATIONTYPES
#define OBSERVATIONTYPES

#define OBSERVATION_OSC_VALUE_UNDEFINED ""

typedef enum
{
    Observation_Osc_Periodic_PositionRoute = 0,
    Observation_Osc_Periodic_Lane,
    Observation_Osc_Periodic_BrakeLight,
    Observation_Osc_Periodic_XPosition,
    Observation_Osc_Periodic_YPosition,
    Observation_Osc_Periodic_YawAngle,
    Observation_Osc_Periodic_Alpha,
    Observation_Osc_Periodic_TBrake,
    Observation_Osc_Periodic_VelocityEgo,
    Observation_Osc_Periodic_VelocityFront,
    Observation_Osc_Periodic_AccelerationEgo,
    Observation_Osc_Periodic_AccelerationFront,
    Observation_Osc_Periodic_DriverOscEgo,
    Observation_Osc_Periodic_DistanceToVehicleFront,
    Observation_Osc_Periodic_TTCToVehicleFront,
    Observation_Osc_Periodic_TimeToVehicleFront,
    Observation_Osc_Periodic_DeltaVToVehicleFront,
    Observation_Osc_Periodic_Attentive
} Observation_Osc_Periodic_Type;

#endif // OBSERVATIONTYPES

