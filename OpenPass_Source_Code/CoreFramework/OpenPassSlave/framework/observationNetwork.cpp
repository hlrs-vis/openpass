/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <stdexcept>
#include "observationNetwork.h"
#include "observationModule.h"
#include "observationBinding.h"
#include "runConfig.h"
#include "stochastics.h"
#include "runResult.h"
#include "log.h"

namespace SimulationSlave
{

ObservationNetwork::~ObservationNetwork()
{
    Clear();
}

void ObservationNetwork::Clear()
{
    for(auto &item : modules)
    {
        delete item.second;
    }

    modules.clear();

    binding->Unload();
}

bool ObservationNetwork::Instantiate(const std::map<int, SimulationCommon::RunConfig::ObservationInstance*> &observationInstances,
                                     Stochastics *stochastics,
                                     WorldInterface *world)
{
    for(const std::pair<const int, SimulationCommon::RunConfig::ObservationInstance*> &item : observationInstances)
    {
        ObservationModule *module = binding->Instantiate(item.second, stochastics, world);
        if(!module)
        {
            return false;
        }

        if(!(modules.insert({item.first, module})).second)
        {
            delete module;
            return false;
        }
    }

    return true;
}

std::map<int, ObservationModule*> &ObservationNetwork::GetObservationModules()
{
    return modules;
}

bool ObservationNetwork::InitAll(const std::string &path)
{
    for(auto &item : modules)
    {
        ObservationModule *module = item.second;
        try
        {
            if(!module->GetLibrary()->SlavePreHook(module->GetImplementation(), path))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre hook failed";
                return false;
            }
        }
        catch(std::runtime_error const &ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre hook failed: " << ex.what();
            return false;
        }
        catch(...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre hook failed";
            return false;
        }
    }

    return true;
}

bool ObservationNetwork::InitRun()
{
    for(auto &item : modules)
    {
        ObservationModule *module = item.second;
        try
        {
            if(!module->GetLibrary()->SlavePreRunHook(module->GetImplementation()))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre run hook failed";
                return false;
            }
        }
        catch(std::runtime_error const &ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre run hook failed: " << ex.what();
            return false;
        }
        catch(...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave pre run hook failed";
            return false;
        }
    }

    return true;
}

bool ObservationNetwork::UpdateTimeStep(int time, RunResult &runResult)
{
    for(auto &item : modules)
    {
        ObservationModule *module = item.second;
        try
        {
            if(!module->GetLibrary()->SlaveUpdateHook(module->GetImplementation(), time, runResult))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave update hook failed";
                return false;
            }
        }
        catch(std::runtime_error const &ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave update hook failed: " << ex.what();
            return false;
        }
        catch(...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave update hook failed";
            return false;
        }
    }

    return true;
}

bool ObservationNetwork::FinalizeRun(const RunResult &result)
{
    for(auto &item : modules)
    {
        ObservationModule *module = item.second;
        try
        {
            if(!module->GetLibrary()->SlavePostRunHook(module->GetImplementation(), result))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post run hook failed";
                return false;
            }
        }
        catch(std::runtime_error const &ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post run hook failed: " << ex.what();
            return false;
        }
        catch(...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post run hook failed";
            return false;
        }
    }

    return true;
}

bool ObservationNetwork::FinalizeAll()
{
    for(auto &item : modules)
    {
        ObservationModule *module = item.second;
        try
        {
            if(!module->GetLibrary()->SlavePostHook(module->GetImplementation()))
            {
                LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post hook failed";
                return false;
            }
        }
        catch(std::runtime_error const &ex)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post hook failed: " << ex.what();
            return false;
        }
        catch(...)
        {
            LOG_INTERN(LogLevel::Error) << "observation " << module->GetId() << ", slave post hook failed";
            return false;
        }
    }

    return true;
}

} // namespace SimulationSlave
