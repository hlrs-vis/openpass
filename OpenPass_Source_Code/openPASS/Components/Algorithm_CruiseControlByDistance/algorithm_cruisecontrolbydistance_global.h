/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
