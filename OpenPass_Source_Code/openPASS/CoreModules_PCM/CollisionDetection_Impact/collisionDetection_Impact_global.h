/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef COLLISIONDETECTION_IMPACT_GLOBAL_H
#define COLLISIONDETECTION_IMPACT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COLLISIONDETECTIONIMPACT_LIBRARY)
#  define COLLISIONDETECTIONIMPACTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COLLISIONDETECTIONIMPACTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COLLISIONDETECTION_IMPACT_GLOBAL_H
