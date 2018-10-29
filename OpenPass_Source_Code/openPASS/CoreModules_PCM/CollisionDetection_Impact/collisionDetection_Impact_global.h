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
