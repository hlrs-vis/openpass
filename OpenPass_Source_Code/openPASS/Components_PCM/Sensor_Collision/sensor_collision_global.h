/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  sensor_collision_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef SENSOR_COLLISION_GLOBAL_H
#define SENSOR_COLLISION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SENSOR_COLLISION_LIBRARY)
#  define SENSOR_COLLISION_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SENSOR_COLLISION_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SENSOR_COLLISION_GLOBAL_H
