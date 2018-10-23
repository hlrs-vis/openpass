/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  dynamics_copyTrajectory_global.h
//! @brief contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef DYNAMICS_COPYTRAJECTORY_GLOBAL_H
#define DYNAMICS_COPYTRAJECTORY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DYNAMICS_COPYTRAJECTORY_LIBRARY)
#  define DYNAMICS_COPYTRAJECTORYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DYNAMICS_COPYTRAJECTORYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DYNAMICS_COPYTRAJECTORY_GLOBAL_H
