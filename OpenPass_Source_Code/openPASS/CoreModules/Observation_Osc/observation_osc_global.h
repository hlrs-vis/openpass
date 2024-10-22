/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  observation_osc_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef OBSERVATION_OSC_GLOBAL_H
#define OBSERVATION_OSC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OBSERVATION_OSC_LIBRARY)
#  define OBSERVATION_OSCSHARED_EXPORT Q_DECL_EXPORT   //! Export of the dll-functions
#else
#  define OBSERVATION_OSCSHARED_EXPORT Q_DECL_IMPORT   //! Import of the dll-functions
#endif

#endif // OBSERVATION_OSC_GLOBAL_H
