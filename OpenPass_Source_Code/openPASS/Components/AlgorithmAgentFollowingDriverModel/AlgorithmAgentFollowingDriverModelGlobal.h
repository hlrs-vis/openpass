/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
/**********************************************
***********************************************/

//! @file  AlgorithmAgentFollowingDriverModelGlobal.h
//! @brief This file contains DLL export declarations
//!
//! This class contains all functionality of the module.

#pragma once

#include <QtCore/qglobal.h>

#if defined(ALGORITHM_AGENTFOLLOWINGDRIVERMODEL_LIBRARY)
#  define ALGORITHM_AGENTFOLLOWINGDRIVERMODEL_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALGORITHM_AGENTFOLLOWINGDRIVERMODEL_SHARED_EXPORT Q_DECL_IMPORT
#endif


