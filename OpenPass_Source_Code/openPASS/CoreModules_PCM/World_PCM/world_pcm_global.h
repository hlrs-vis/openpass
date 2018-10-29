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
//! @file  world_pcm_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef WORLD_PCM_GLOBAL_H
#define WORLD_PCM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WORLD_PCM_LIBRARY)
#  define WORLD_PCMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WORLD_PCMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WORLD_PCM_GLOBAL_H
