/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  action_scopeLogger_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef ACTION_SCOPELOGGER_GLOBAL_H
#define ACTION_SCOPELOGGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ACTION_SCOPELOGGER_LIBRARY)
#  define ACTION_SCOPELOGGER_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACTION_SCOPELOGGER_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ACTION_SCOPELOGGER_GLOBAL_H
