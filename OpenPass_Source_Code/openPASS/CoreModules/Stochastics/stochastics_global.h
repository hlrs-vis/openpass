/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef STOCHASTICS_GLOBAL_H
#define STOCHASTICS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STOCHASTICS_LIBRARY)
#  define STOCHASTICSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STOCHASTICSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STOCHASTICS_GLOBAL_H
