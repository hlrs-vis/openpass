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
/** \file  ManipulatorExport.cpp */
//-----------------------------------------------------------------------------

#include "ManipulatorExport.h"
#include "Interfaces/scenarioActionInterface.h"
#include "Common/openScenarioDefinitions.h"
#include "CoreFramework/CoreShare/parameters.h"
#include "CollisionManipulator.h"
#include "ComponentStateChangeManipulator.h"


const std::string version = "0.0.1";
static const CallbackInterface* Callbacks = nullptr;

extern "C" MANIPULATOR_SHARED_EXPORT const std::string& OpenPASS_GetVersion()
{
    return version;
}

extern "C" MANIPULATOR_SHARED_EXPORT ManipulatorInterface* OpenPASS_CreateInstance(
    WorldInterface* world,
    std::shared_ptr<ScenarioActionInterface> action,
    SimulationCommon::ManipulatorParameters* parameters,
    std::string manipulatorType,
    SimulationSlave::EventNetworkInterface* eventNetwork,
    const CallbackInterface* callbacks)
{
    Callbacks = callbacks;

    try
    {
        if (manipulatorType == "ComponentStateChangeManipulator")
        {
            return static_cast<ManipulatorInterface*>(new (std::nothrow) ComponentStateChangeManipulator(
                                                          world,
                                                          parameters,
                                                          eventNetwork,
                                                          callbacks));
        }
        else if (manipulatorType == "CollisionManipulator")
        {
            return static_cast<ManipulatorInterface*>(new (std::nothrow) CollisionManipulator(
                                                          world,
                                                          parameters,
                                                          eventNetwork,
                                                          callbacks));
        }
        else
        {
            std::shared_ptr<UserDefinedCommandAction> castedUserDefinedCommandAction = std::dynamic_pointer_cast<UserDefinedCommandAction>(action);
            if (castedUserDefinedCommandAction.get() != nullptr)
            {
                std::string command = castedUserDefinedCommandAction->GetCommand();
                std::size_t firstSplitInCommand = command.find(' ');
                std::string commandType = command.substr(0, firstSplitInCommand);

                if (commandType == "SetComponentState")
                {
                    return static_cast<ManipulatorInterface*>(new (std::nothrow) ComponentStateChangeManipulator(
                                                                  world,
                                                                  castedUserDefinedCommandAction,
                                                                  eventNetwork,
                                                                  callbacks
                                                                  ));
                }
            }
        }
    }
    catch (...)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return nullptr;
    }

    return nullptr;
}

extern "C" MANIPULATOR_SHARED_EXPORT void OpenPASS_DestroyInstance(ManipulatorInterface* implementation)
{
    delete implementation;
}
