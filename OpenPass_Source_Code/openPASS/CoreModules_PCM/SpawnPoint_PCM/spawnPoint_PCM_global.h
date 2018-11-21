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
//! @file  spawnPoint_PCM_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINT_PCM_GLOBAL_H
#define SPAWNPOINT_PCM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPAWNPOINT_PCM_LIBRARY)
#  define SPAWNPOINT_PCMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPAWNPOINT_PCMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPAWNPOINT_PCM_GLOBAL_H
