/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  action_secondaryDriverTasksGlobal.h
*	@brief This file contains DLL export declarations
**/
//-----------------------------------------------------------------------------

#ifndef ACTION_SECONDARY_DRIVER_TASKS_GLOBAL_H
#define ACTION_SECONDARY_DRIVER_TASKS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ACTION_SECONDARY_DRIVER_TASKS_LIBRARY)
#  define ACTION_SECONDARY_DRIVER_TASKS_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACTION_SECONDARY_DRIVER_TASKS_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ACTION_SECONDARY_DRIVER_TASKS_GLOBAL_H
