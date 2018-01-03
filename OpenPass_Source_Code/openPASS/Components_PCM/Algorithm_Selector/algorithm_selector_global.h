/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  algorithm_selector_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef ALGORITHM_SELECTOR_GLOBAL_H
#define ALGORITHM_SELECTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ALGORITHM_SELECTOR_LIBRARY)
#  define ALGORITHM_SELECTOR_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALGORITHM_SELECTOR_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ALGORITHM_SELECTOR_GLOBAL_H
