/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  dynamics_longitudinal_basic_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef DYNAMICS_LONGITUDINAL_BASIC_GLOBAL_H
#define DYNAMICS_LONGITUDINAL_BASIC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DYNAMICS_LONGITUDINAL_BASIC_LIBRARY)
#  define DYNAMICS_LONGITUDINAL_BASICSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DYNAMICS_LONGITUDINAL_BASICSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DYNAMICS_LONGITUDINAL_BASIC_GLOBAL_H
