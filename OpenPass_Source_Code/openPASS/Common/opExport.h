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

#elif(defined(__GNUC__) && __GNUC__ >= 4|| defined(__clang__))
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

#if defined(Common_PCM_EXPORTS)
#define OPENPASSCOMMONPCMEXPORT OPEXPORT
#else
#define OPENPASSCOMMONPCMEXPORT OPIMPORT
#endif

#if defined(Agent_EXPORTS)
#define OPENPASSAGENTEXPORT OPEXPORT
#else
#define OPENPASSAGENTEXPORT OPIMPORT
#endif

#if defined(Component_EXPORTS)
#define OPENPASSCOMPONENTEXPORT OPEXPORT
#else
#define OPENPASSCOMPONENTEXPORT OPIMPORT
#endif

#if defined(Pcm_EXPORTS)
#define OPENPASSPCMEXPORT OPEXPORT
#else
#define OPENPASSPCMEXPORT OPIMPORT
#endif

#if defined(Pcm_Eval_EXPORTS)
#define OPENPASSPCMEVALEXPORT OPEXPORT
#else
#define OPENPASSPCMEVALEXPORT OPIMPORT
#endif

#if defined(Project_EXPORTS)
#define OPENPASSPROJECTEXPORT OPEXPORT
#else
#define OPENPASSPROJECTEXPORT OPIMPORT
#endif

#if defined(Scenery_EXPORTS)
#define OPENPASSSCENERYEXPORT OPEXPORT
#else
#define OPENPASSSCENERYEXPORT OPIMPORT
#endif

#if defined(Simulation_EXPORTS)
#define OPENPASSSIMULATIONEXPORT OPEXPORT
#else
#define OPENPASSSIMULATIONEXPORT OPIMPORT
#endif

#if defined(System_EXPORTS)
#define OPENPASSSYSTEMEXPORT OPEXPORT
#else
#define OPENPASSSYSTEMEXPORT OPIMPORT
#endif

#if defined(Window_EXPORTS)
#define OPENPASSWINDOWSEXPORT OPEXPORT
#else
#define OPENPASSWINDOWSEXPORT OPIMPORT
#endif

#if defined(openPASSInterfaces_EXPORTS)
#define OPENPASSINTERFACESEXPORT OPEXPORT
#else
#define OPENPASSINTERFACESEXPORT OPIMPORT
#endif


#endif // OPEXPORT_H

