/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  world_basic_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef WORLD_GLOBAL_H
#define WORLD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WORLD_BASIC_LIBRARY)
#  define WORLD_BASICSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WORLD_BASICSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WORLD_GLOBAL_H
