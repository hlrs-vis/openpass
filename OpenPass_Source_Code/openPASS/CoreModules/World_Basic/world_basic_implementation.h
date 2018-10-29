/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#ifndef WORLD_IMPLEMENTATION_H
#define WORLD_IMPLEMENTATION_H

#include <QtGlobal>
#include "worldInterface.h"
#include "agentNetwork.h"

/**
* \addtogroup CoreModules_Basic openPASS CoreModules basic
* @{
* \addtogroup World_Basic
* @{
* \brief implementation of a world interface
*
* The World module implements a WorldInterface which is used
* by the framework and the agents.
* It is used to set up a basic simulated world with static objects like
* roads, intersection and curves and dynamic objects like cars, bicycles and
* pedastrians.
*
* The World handles all simulated objects.
*
* \section world_inputs Inputs
* name | meaning
* -----|---------
* callbacks | CallbackInterface to send log information to the framework
*
* \section world_outputs Outputs
 name | meaning
* -----|---------
* *WoldInterface | Provides a reference to an implementation of a WorldInterface to the framework.
*
* \section world_configParameters Parameters to be specified in runConfiguration.xml
* tag | meaning
* -----|---------
* World | Name of World library. "World" by default.
*
* @see WorldInterface
*
* @}
* @} */

/*!
 * \brief Implementation of a WorldInterface
 *
 * This class implements all function of the WorldInterface. It is responsible for all
 * dynamic and static objects in a given scenery.
 *
 * \ingroup World_Basic
 */
class World_Basic_Implementation : public WorldInterface
{
public:
    World_Basic_Implementation(const CallbackInterface *callbacks);
    World_Basic_Implementation(const World_Basic_Implementation &) = delete;
    World_Basic_Implementation(World_Basic_Implementation &&) = delete;
    World_Basic_Implementation &operator=(const World_Basic_Implementation &) = delete;
    World_Basic_Implementation &operator=(World_Basic_Implementation &&) = delete;
    virtual ~World_Basic_Implementation();

    virtual void *GetGlobalDrivingView()
    {
        return nullptr;   //dummy
    }

    virtual void *GetGlobalObjects()
    {
        return nullptr;   //dummy
    }

    bool AddAgent(int id, AgentInterface *agent);
    const AgentInterface *GetAgent(int id) const;
    const std::map<int, const AgentInterface *> &GetAgents() const;

    // framework internal methods to access members without restrictions
    void SetParameter(WorldParameter *worldParameter);
    void Clear();
    bool CreateGlobalDrivingView();

    // model callbacks
    void SetTimeOfDay(int timeOfDay);
    int GetTimeOfDay() const;

    void SetWeekday(Weekday weekday);
    Weekday GetWeekday() const;

    void QueueAgentUpdate(std::function<void(double)> func,
                          double val);
    void QueueAgentRemove(const AgentInterface *agent);
    void SyncGlobalData();

    bool CreateScenery(SceneryInterface &scenery);

    AgentInterface *CreateAgentAdapterForAgent();

    const AgentInterface *GetSpecialAgent()
    {
        return nullptr;   //dummy
    }

    const AgentInterface *GetLastCarInlane(int laneNumber)
    {
        Q_UNUSED(laneNumber);
        return nullptr;
    } //dummy

    const AgentInterface *GetBicycle() const
    {
        return nullptr;   //dummy
    }

    Position GetPositionByDistanceAndLane(double distance,
                                          int laneNumber) const
    {
        Q_UNUSED(distance);
        Q_UNUSED(laneNumber);
        return Position();
    } //dummy

    bool CreateWorldScenery(const  std::string &sceneryFilename)
    {
        Q_UNUSED(sceneryFilename);
        return false;
    } //dummy

    bool CreateWorldScenario(const  std::string &scenarioFilename)
    {
        Q_UNUSED(scenarioFilename);
        return false;
    } //dummy

protected:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if (callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    // world parameters
    int timeOfDay = 0;
    Weekday weekday = Weekday::Undefined;

    AgentNetwork agentNetwork;

    const CallbackInterface *callbacks = nullptr;
};

#endif // WORLD_IMPLEMENTATION_H
