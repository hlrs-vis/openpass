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
//! @file  algorithm_trajectoryFollower_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef DYNAMICS_COLLISION_GLOBAL_H
#define DYNAMICS_COLLISION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DYNAMICS_COLLISION_LIBRARY)
#  define DYNAMICS_COLLISIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DYNAMICS_COLLISIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DYNAMICS_COLLISION_GLOBAL_H
