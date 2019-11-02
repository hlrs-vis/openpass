/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "eventDetectorBinding.h"
#include "eventDetectorLibrary.h"
#include "CoreFramework/CoreShare/log.h"

namespace SimulationSlave
{

EventDetectorBinding::EventDetectorBinding(CallbackInterface *callbacks) :
    callbacks(callbacks)
{}

EventDetectorBinding::~EventDetectorBinding()
{
    Unload();
}

std::vector<const EventDetector*> EventDetectorBinding::Instantiate(const std::string libraryPath,
                                                              ScenarioInterface *scenario,
                                                              EventNetworkInterface* eventNetwork,
                                                              WorldInterface *world,
                                                              StochasticsInterface *stochastics)
{
    library = new EventDetectorLibrary(libraryPath,
                                       callbacks);

    if(!library)
    {
        LOG_INTERN(LogLevel::Error) << "EventDetector library could not be created";
        throw std::runtime_error("EventDetector library could not be created");
    }

    if(!library->Init())
    {
        delete library;
        LOG_INTERN(LogLevel::Error) << "EventDetector library could not be initalized";
        throw std::runtime_error("EventDetector library could not be initilized");
    }

    std::vector<const EventDetector*> eventDetectors;

    //Instantiates all default eventDetectors
    for(auto eventDetectorType : defaultEventDetectorTypes)
    {
        const auto eventDetector = library->CreateEventDetector(eventDetectorType,
                                                                nullptr,
                                                                eventNetwork,
                                                                world,
                                                                stochastics);
        eventDetectors.push_back(eventDetector);
    }

    //Instantiates an eventdetector for each true flag
    for(auto eventDetectorParameters : scenario->GetEventDetectors())
    {
        const auto eventDetector = library->CreateEventDetector(eventDetectorParameters.GetType(),
                                                                &eventDetectorParameters,
                                                                eventNetwork,
                                                                world,
                                                                stochastics);
        eventDetectors.push_back(eventDetector);
    }

    return eventDetectors;
}

void EventDetectorBinding::Unload()
{
    delete library;
    library = nullptr;
}

} // namespace SimulationSlave
