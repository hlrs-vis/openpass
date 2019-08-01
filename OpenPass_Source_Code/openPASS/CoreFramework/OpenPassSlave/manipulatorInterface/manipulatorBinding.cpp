/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "manipulatorBinding.h"
#include "manipulatorLibrary.h"
#include "CoreFramework/CoreShare/log.h"

namespace SimulationSlave
{

ManipulatorBinding::ManipulatorBinding(CallbackInterface *callbacks) :
    callbacks(callbacks)
{}

ManipulatorBinding::~ManipulatorBinding()
{
    Unload();
}

std::vector<const Manipulator*> ManipulatorBinding::Instantiate(const std::string libraryPath,
                                                          ScenarioInterface *scenario,
                                                          EventNetworkInterface* eventNetwork,
                                                          WorldInterface *world)
{
    library = new ManipulatorLibrary(libraryPath,
                                     callbacks);

    if(!library)
    {
        LOG_INTERN(LogLevel::Error) << "Manipulator library could not be created";
        throw std::runtime_error("Manipulator library could not be created");
    }

    if(!library->Init())
    {
        delete library;
        LOG_INTERN(LogLevel::Error) << "Manipulator library could not be initalized";
        throw std::runtime_error("Manipulator library could not be initalized");
    }

    std::vector<const Manipulator*> manipulators;

    //Instantiates all default eventDetectors
    for(auto manipulatorType : defaultManipulatorTypes)
    {
        const auto manipulator = library->CreateManipulator(nullptr,
                                                            manipulatorType,
                                                            nullptr,
                                                            eventNetwork,
                                                            world);
        manipulators.push_back(manipulator);
    }

    //Instantiates a manipulators
    for(auto manipulatorParameters : scenario->GetManipulators())
    {
        const auto manipulator = library->CreateManipulator(nullptr,
                                                            manipulatorParameters.GetType(),
                                                            &manipulatorParameters,
                                                            eventNetwork,
                                                            world);
        manipulators.push_back(manipulator);
    }
    for(const auto& action : scenario->GetActions())
    {
        const auto manipulator = library->CreateManipulator(action,
                                                            "",
                                                            nullptr,
                                                            eventNetwork,
                                                            world);
        manipulators.push_back(manipulator);
    }

    return manipulators;
}

void ManipulatorBinding::Unload()
{
    if (library != nullptr)
    {
        delete library;
    }
    library = nullptr;
}

} // namespace SimulationSlave
