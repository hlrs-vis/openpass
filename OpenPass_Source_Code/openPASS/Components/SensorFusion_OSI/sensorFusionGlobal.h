/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <QtCore/qglobal.h>

#if defined(SENSOR_FUSION_LIBRARY)
#  define SENSOR_FUSION_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SENSOR_FUSION_SHARED_EXPORT Q_DECL_IMPORT
#endif


