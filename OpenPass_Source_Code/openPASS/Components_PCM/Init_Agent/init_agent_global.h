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
//! @file  init_agent_global.h
//! @brief This file contains DLL export declarations
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#ifndef INIT_AGENT_GLOBAL_H
#define INIT_AGENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(INIT_AGENT_LIBRARY)
#  define INIT_AGENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define INIT_AGENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // INIT_AGENT_GLOBAL_H
