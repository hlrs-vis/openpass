/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#ifndef WORLD_PCM_IMPLEMENTATION_H
#define WORLD_PCM_IMPLEMENTATION_H

#include <QtGlobal>
#include "worldInterface.h"
#include "agentNetwork.h"
#include "sceneryImporterPCM.h"
#include "scenarioImporterPCM.h"
#include "pcm_trajectory.h"

/**
* \addtogroup CoreModules_PCM openPASS CoreModules pcm
* @{
* \addtogroup World_PCM
*
* \brief World module based on PCM data.
*
* The World module implements a WorldInterface which is used
* by the framework and the agents.
* It is used to set up a world based on PCM-data simulated with static objects like
* roads, intersection and curves and dynamic objects like cars, bicycles and
* pedastrians.
*
* The World handles all simulated objects.
*
* @see WorldInterface
* @}
*/

/*!
* \copydoc World_PCM
* \ingroup World_PCM
*/
class World_PCM_Implementation : public WorldInterface
{
public:
    World_PCM_Implementation(const CallbackInterface *callbacks);
    World_PCM_Implementation(const World_PCM_Implementation &) = delete;
    World_PCM_Implementation(World_PCM_Implementation &&) = delete;
    World_PCM_Implementation &operator=(const World_PCM_Implementation &) = delete;
    World_PCM_Implementation &operator=(World_PCM_Implementation &&) = delete;
    virtual ~World_PCM_Implementation();

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
    virtual void* GetWorldData()
    {
        return nullptr; // dummy
    }
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

    bool CreateWorldScenery(const  std::string &sceneryFilename);

    bool CreateWorldScenario(const  std::string &scenarioFilename);

    //----------------END WorldInterface functions-------------------------------------

    const PCM_Data *GetPCM_Data() const;
    const PCM_Trajectory *GetTrajectory(int agentId) const;

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
    void UpdatePcmAgentData();

    // world parameters
    int timeOfDay = 0;
    Weekday weekday = Weekday::Undefined;

    AgentNetwork agentNetwork;
    SceneryImporterPCM sceneryImporterPCM;
    ScenarioImporterPCM scenarioImporterPCM;

    const CallbackInterface *callbacks = nullptr;

    /** \addtogroup World_PCM
    *  @{
    *    \name PCM_Data
    *    @{
    */
    PCM_Data pcmData; //!< store all pcm data
    std::map<int, PCM_Trajectory> trajectories;   //!<map of all trajectories
    /**
    *    @}
    *  @}
    */
};

#endif // WORLD_PCM_IMPLEMENTATION_H
