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
/** @file  ObservationLogGlobal.h
* @brief This file contains DLL export declarations
*
* This class contains all functionality of the module.
*/
//-----------------------------------------------------------------------------

#pragma once

#include <QtCore/qglobal.h>

#if defined(OBSERVATION_LOG_LIBRARY)
#  define OBSERVATION_LOG_SHARED_EXPORT Q_DECL_EXPORT   //! Export of the dll-functions
#else
#  define OBSERVATION_LOG_SHARED_EXPORT Q_DECL_IMPORT   //! Import of the dll-functions
#endif


