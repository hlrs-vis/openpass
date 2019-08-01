/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  EventDetectorExport.cpp */
//-----------------------------------------------------------------------------

#include "EventDetectorExport.h"

#include "Common/observationTypes.h"
#include "CoreFramework/CoreShare/parameters.h"

//Different detectors
#include "CollisionDetector.h"
#include "ConditionalEventDetector.h"

const std::string version = "0.0.1";
static const CallbackInterface* Callbacks = nullptr;

extern "C" EVENT_DETECTOR_SHARED_EXPORT const std::string& OpenPASS_GetVersion()
{
    return version;
}

extern "C" EVENT_DETECTOR_SHARED_EXPORT EventDetectorInterface* OpenPASS_CreateInstance(
    WorldInterface* world,
    SimulationCommon::EventDetectorParameters* parameters,
    std::string eventType,
    SimulationSlave::EventNetworkInterface* eventNetwork,
    const CallbackInterface* callbacks,
    StochasticsInterface* stochastics)
{
    Callbacks = callbacks;

    try
    {
        switch (EventDefinitions::eventTypesMap.at(eventType))
        {
            case EventDefinitions::EventType::Collision:
                return static_cast<EventDetectorInterface*>(new CollisionDetector(
                            world,
                            parameters,
                            eventNetwork,
                            callbacks,
                            stochastics));

            case EventDefinitions::EventType::Conditional:
                return static_cast<EventDetectorInterface*>(new ConditionalEventDetector(
                                                 world,
                                                 parameters,
                                                 eventNetwork,
                                                 callbacks,
                                                 stochastics));
            default:
                if (Callbacks != nullptr)
                {
                    Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "Invalid eventDetectorType. Can not be instantiated.");
                }

                return nullptr;
        }
    }
    catch (const std::bad_alloc&)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "Failed to allocate memory.");
        }

        return nullptr;
    }
    catch (...)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return nullptr;
    }
}

extern "C" EVENT_DETECTOR_SHARED_EXPORT void OpenPASS_DestroyInstance(EventDetectorInterface* implementation)
{
    delete implementation;
}

