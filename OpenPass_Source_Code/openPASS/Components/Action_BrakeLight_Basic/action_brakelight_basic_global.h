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
//! @file  action_brakelight_basic_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef ACTION_BRAKELIGHT_BASIC_GLOBAL_H
#define ACTION_BRAKELIGHT_BASIC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ACTION_BRAKELIGHT_BASIC_LIBRARY)
#  define ACTION_BRAKELIGHT_BASICSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACTION_BRAKELIGHT_BASICSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ACTION_BRAKELIGHT_BASIC_GLOBAL_H
