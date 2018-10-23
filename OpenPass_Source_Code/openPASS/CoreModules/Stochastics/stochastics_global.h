/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef STOCHASTICS_GLOBAL_H
#define STOCHASTICS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STOCHASTICS_LIBRARY)
#  define STOCHASTICSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STOCHASTICSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STOCHASTICS_GLOBAL_H
