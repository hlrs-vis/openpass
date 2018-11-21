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
//! @file  evaluation_pcm_global.h
//! @brief This file contains global definitions of the model and defines the
//!        export qualifier.
//-----------------------------------------------------------------------------

#ifndef EVALUATION_PCM_GLOBAL_H
#define EVALUATION_PCM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EVALUATION_PCM_LIBRARY)
#  define EVALUATION_PCM_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define EVALUATION_PCM_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EVALUATION_PCM_GLOBAL_H
