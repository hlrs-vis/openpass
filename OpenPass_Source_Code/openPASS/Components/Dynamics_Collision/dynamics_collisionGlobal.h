/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  dynamics_collisionGlobal.h
*	@brief This file contains DLL export declarations
**/
//-----------------------------------------------------------------------------

#pragma once

#include <QtCore/qglobal.h>

#if defined(DYNAMICS_COLLISION_LIBRARY)
#  define DYNAMICS_COLLISION_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define DYNAMICS_COLLISION_SHARED_EXPORT Q_DECL_IMPORT
#endif


