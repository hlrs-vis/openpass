/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  collisionDetection_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef COLLISIONDETECTION_GLOBAL_H
#define COLLISIONDETECTION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COLLISIONDETECTION_LIBRARY)
#  define COLLISIONDETECTIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COLLISIONDETECTIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COLLISIONDETECTION_GLOBAL_H
