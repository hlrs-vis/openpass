/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef OPEXPORT_H
#define OPEXPORT_H

#if defined(_WIN32) && !defined(NODLL)
#define OPIMPORT __declspec(dllimport)
#define OPEXPORT __declspec(dllexport)

#elif(defined(__GNUC__) && __GNUC__ >= 4|| defined(__clang__)
#define OPEXPORT __attribute__((visibility("default")))
#define OPIMPORT OPEXPORT

#else
#define OPIMPORT
#define OPEXPORT
#endif


#if defined(Common_EXPORTS)
#define OPENPASSCOMMONEXPORT OPEXPORT
#else
#define OPENPASSCOMMONEXPORT OPIMPORT
#endif

#endif // OPEXPORT_H

