/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "Interfaces/manipulatorNetworkInterface.h"
#include "Interfaces/worldInterface.h"

namespace SimulationSlave
{

//Forward declarations
class Manipulator;
class ManipulatorBinding;
class EventNetworkInterface;

class ManipulatorNetwork : public ManipulatorNetworkInterface
{
public:
    ManipulatorNetwork(ManipulatorBinding *manipulatorBinding,
                       WorldInterface *world);
    ManipulatorNetwork(const ManipulatorNetwork&) = delete;
    ManipulatorNetwork(ManipulatorNetwork&&) = delete;
    ManipulatorNetwork& operator=(const ManipulatorNetwork&) = delete;
    ManipulatorNetwork& operator=(ManipulatorNetwork&&) = delete;
    ~ManipulatorNetwork();

    //-----------------------------------------------------------------------------
    //! Instantiates the manipulator network.
    //!
    //! @param[in]  libraryPath         Path to the library
    //! @param[in]  scenario            Scenario
    //! @param[in]  eventNetwork        Interface of the eventNetwork
    //! @return                         true, if successful
    //-----------------------------------------------------------------------------
    bool Instantiate(const std::string libraryPath,
                     ScenarioInterface *scenario,
                     EventNetworkInterface* eventNetwork);

    //-----------------------------------------------------------------------------
    //! Clears all manipulators
    //-----------------------------------------------------------------------------
    void Clear();

    //-----------------------------------------------------------------------------
    //! Returns all manipulators
    //!
    //! @return      Vector of created manipulators from the library
    //-----------------------------------------------------------------------------
    std::vector<const Manipulator*> GetManipulators();

private:

    ManipulatorBinding *manipulatorBinding;
    std::vector<const Manipulator*> manipulators;
    WorldInterface *world;
};

}// namespace SimulationSlave


