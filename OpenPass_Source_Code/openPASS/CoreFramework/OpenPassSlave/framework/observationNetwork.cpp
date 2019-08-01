/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <stdexcept>
#include "CoreFramework/CoreShare/log.h"
#include "observationNetwork.h"
#include "observationBinding.h"
#include "observationModule.h"
#include "runResult.h"

namespace SimulationSlave {

ObservationNetwork::~ObservationNetwork()
{
    Clear();
}

void ObservationNetwork::Clear()
{
    for (auto& items : modules)
    {
        delete items.second;
    }

    modules.clear();

    if (binding != nullptr)
    {
        binding->Unload();
    }
}

bool ObservationNetwork::Instantiate(const std::map<int, ObservationInstance>& observationInstances,
                                     StochasticsInterface* stochastics,
                                     WorldInterface* world,
                                     EventNetworkInterface* eventNetwork)
{
    int id;
    try
    {
        for (const auto& item : observationInstances)
        {
            id = item.first;
            auto observationInstance = item.second;

            auto module = binding->Instantiate(observationInstance.libraryPath,
                                               observationInstance.parameters,
                                               stochastics,
                                               world,
                                               eventNetwork);

            modules.insert({id, module});
        }
    }
    catch (const std::exception& ex)
    {
        LOG_INTERN(LogLevel::Error) << "observation " << id << ", could not be initialized: " << ex.what();
        return false;
    }

    return true;
}

const std::map<int, ObservationModule*>& ObservationNetwork::GetObservationModules()
{
    return modules;
}

bool ObservationNetwork::InitAll(const std::string& path)
{
    for (auto& item : modules)
    {
        auto module = item.second;
        try
        {
            if (!module->GetLibrary()->SlavePreHook(module->GetImplementation(), path))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre hook failed";
                return false;
            }
        }
        catch (const std::runtime_error& ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre hook failed: " << ex.what();
            return false;
        }
        catch (...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre hook failed";
            return false;
        }
    }
    return true;
}

bool ObservationNetwork::InitRun()
{
    for (auto& item : modules)
    {
        auto module = item.second;
        try
        {
            if (!module->GetLibrary()->SlavePreRunHook(module->GetImplementation()))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre run hook failed";
                return false;
            }
        }
        catch (std::runtime_error const& ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre run hook failed: " << ex.what();
            return false;
        }
        catch (...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre run hook failed";
            return false;
        }
    }
    return true;
}

bool ObservationNetwork::UpdateTimeStep(int time, RunResult& runResult)
{
    for (auto& item : modules)
    {
        ObservationModule* module = item.second;
        try
        {
            if (!module->GetLibrary()->SlaveUpdateHook(module->GetImplementation(), time, runResult))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave update hook failed";
                return false;
            }
        }
        catch (std::runtime_error const& ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave update hook failed: " << ex.what();
            return false;
        }
        catch (...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave update hook failed";
            return false;
        }
    }
    return true;
}


bool ObservationNetwork::FinalizeRun(const RunResult& result)
{
    for (auto& item : modules)
    {
        auto module = item.second;
        try
        {
            if (!module->GetLibrary()->SlavePostRunHook(module->GetImplementation(), result))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post run hook failed";
                return false;
            }
        }
        catch (std::runtime_error const& ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post run hook failed: " << ex.what();
            return false;
        }
        catch (...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post run hook failed";
            return false;
        }
    }
    return true;
}

bool ObservationNetwork::FinalizeAll()
{
    for (auto& item : modules)
    {
        auto module = item.second;
        try
        {
            if (!module->GetLibrary()->SlavePostHook(module->GetImplementation()))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post hook failed";
                return false;
            }
        }
        catch (std::runtime_error const& ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post hook failed: " << ex.what();
            return false;
        }
        catch (...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post hook failed";
            return false;
        }
    }
    return true;
}

} // namespace SimulationSlave
