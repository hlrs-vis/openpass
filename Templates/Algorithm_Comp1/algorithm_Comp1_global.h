/*********************************************************************
* This Example Content is intended to demonstrate usage of
* Eclipse technology. It is provided to you under the terms
* and conditions of the Eclipse Distribution License v1.0
* which is available at http://www.eclipse.org/org/documents/edl-v10.php
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  algorithm_Comp1_global.h
//! @brief This file contains DLL export declarations
//-----------------------------------------------------------------------------

#ifndef ALGORITHM_COMP1_GLOBAL_H
#define ALGORITHM_COMP1_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ALGORITHM_COMP1_LIBRARY)
#  define ALGORITHM_COMP1_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALGORITHM_COMP1_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ALGORITHM_COMP1_GLOBAL_H
