/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


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
