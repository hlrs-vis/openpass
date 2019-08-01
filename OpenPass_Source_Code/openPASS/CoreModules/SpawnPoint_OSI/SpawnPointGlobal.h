/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  SpawnpointGlobal.h
*	@brief This file contains DLL export declarations
**/
//-----------------------------------------------------------------------------

#pragma once

#include <QtCore/qglobal.h>

#if defined(SPAWNPOINT_LIBRARY)
#  define SPAWNPOINT_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPAWNPOINT_SHARED_EXPORT Q_DECL_IMPORT
#endif

