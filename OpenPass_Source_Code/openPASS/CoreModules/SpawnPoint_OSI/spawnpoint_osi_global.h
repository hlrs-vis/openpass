/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnpoint_osi_global.h
//! @brief This file contains DLL export declarations.
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINT_OSI_GLOBAL_H
#define SPAWNPOINT_OSI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPAWNPOINT_OSI_LIBRARY)
#  define SPAWNPOINT_OSISHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPAWNPOINT_OSISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPAWNPOINT_OSI_GLOBAL_H
