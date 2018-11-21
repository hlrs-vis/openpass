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
//! @file  algorithm_cruisecontrolbydistance_global.h
//! @brief This file contains DLL export declarations.
//-----------------------------------------------------------------------------

#ifndef ALGORITHM_CRUISECONTROLBYDISTANCE_GLOBAL_H
#define ALGORITHM_CRUISECONTROLBYDISTANCE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ALGORITHM_CRUISECONTROLBYDISTANCE_LIBRARY)
#  define ALGORITHM_CRUISECONTROLBYDISTANCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALGORITHM_CRUISECONTROLBYDISTANCESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ALGORITHM_CRUISECONTROLBYDISTANCE_GLOBAL_H
