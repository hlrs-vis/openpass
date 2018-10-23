/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
