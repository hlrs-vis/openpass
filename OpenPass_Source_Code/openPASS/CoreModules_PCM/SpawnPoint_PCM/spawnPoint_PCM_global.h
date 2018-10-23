/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
