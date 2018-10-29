/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


//-----------------------------------------------------------------------------
//! @file  spawnpoint_start_global.h
//! @brief This file contains DLL export declarations.
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINT_START_GLOBAL_H
#define SPAWNPOINT_START_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPAWNPOINT_START_LIBRARY)
#  define SPAWNPOINT_STARTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPAWNPOINT_STARTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPAWNPOINT_START_GLOBAL_H
