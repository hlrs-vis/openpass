/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  egoSensor_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef EGO_SENSOR_GLOBAL_H
#define EGO_SENSOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EGO_SENSOR_LIBRARY)
#  define EGO_SENSOR_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define EGO_SENSOR_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EGO_SENSOR_GLOBAL_H
