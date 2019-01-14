/*********************************************************************
* This Example Content is intended to demonstrate usage of
* Eclipse technology. It is provided to you under the terms
* and conditions of the Eclipse Distribution License v1.0
* which is available at http://www.eclipse.org/org/documents/edl-v10.php
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  dynamics_Comp2_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef DYNAMICS_COMP2_GLOBAL_H
#define DYNAMICS_COMP2_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DYNAMICS_COMP2_LIBRARY)
#  define DYNAMICS_COMP2_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define DYNAMICS_COMP2_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DYNAMICS_COMP2_GLOBAL_H
