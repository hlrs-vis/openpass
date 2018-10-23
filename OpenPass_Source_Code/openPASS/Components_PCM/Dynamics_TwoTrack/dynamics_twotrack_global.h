/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  dynamics_twotrack_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef DYNAMICS_TWOTRACK_GLOBAL_H
#define DYNAMICS_TWOTRACK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DYNAMICS_TWOTRACK_LIBRARY)
#  define DYNAMICS_TWOTRACKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DYNAMICS_TWOTRACKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DYNAMICS_TWOTRACK_GLOBAL_H
