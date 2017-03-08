/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnPointSingle_0_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINTSINGLE_0_GLOBAL_H
#define SPAWNPOINTSINGLE_0_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPAWNPOINTSINGLE_0_LIBRARY)
#  define SPAWNPOINTSINGLE_0SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPAWNPOINTSINGLE_0SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPAWNPOINTSINGLE_0_GLOBAL_H
