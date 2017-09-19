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

#define OBSERVATION_STATE_VALUE_UNDEFINED ""

typedef enum
{
    Observation_State_Periodic_PositionRoute = 0,
    Observation_State_Periodic_Lane,
    Observation_State_Periodic_BrakeLight,
    Observation_State_Periodic_XPosition,
    Observation_State_Periodic_YPosition,
    Observation_State_Periodic_YawAngle,
    Observation_State_Periodic_Alpha,
    Observation_State_Periodic_TBrake,
    Observation_State_Periodic_VelocityEgo,
    Observation_State_Periodic_VelocityFront,
    Observation_State_Periodic_AccelerationEgo,
    Observation_State_Periodic_AccelerationFront,
    Observation_State_Periodic_DriverStateEgo,
    Observation_State_Periodic_DistanceToVehicleFront,
    Observation_State_Periodic_TTCToVehicleFront,
    Observation_State_Periodic_TimeToVehicleFront,
    Observation_State_Periodic_DeltaVToVehicleFront,
    Observation_State_Periodic_Attentive
} Observation_State_Periodic_Type;

#endif // OBSERVATIONTYPES

