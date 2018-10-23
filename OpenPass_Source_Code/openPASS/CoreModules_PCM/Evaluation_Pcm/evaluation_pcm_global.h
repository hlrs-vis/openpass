/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
