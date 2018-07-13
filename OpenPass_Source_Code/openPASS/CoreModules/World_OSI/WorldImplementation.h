/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

#include <algorithm>
#include <qglobal.h>
#include "worldInterface.h"
#include "AgentNetwork.h"
#include "SceneryConverter.h"
#include "CommonSceneryHelper.h"
#include "LocalizationCache.h"
#include "parameterInterface.h"

#include "WorldData.h"

#include "osi/osi_groundtruth.pb.h"

/** \addtogroup World
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
* @} */

/*!
 * \brief Implementation of a WorldInterface
 *
 * This class implements all function of the WorldInterface. It is responsible for all
 * dynamic and static objects in a given scenery.
 *
 * \ingroup World
 */
class WorldImplementation : public WorldInterface
{
public:
    const std::string MODULENAME = "WORLD";

    WorldImplementation(const CallbackInterface *callbacks);
    WorldImplementation(const WorldImplementation&) = delete;
    WorldImplementation(WorldImplementation&&) = delete;
    WorldImplementation& operator=(const WorldImplementation&) = delete;
    WorldImplementation& operator=(WorldImplementation&&) = delete;

    virtual ~WorldImplementation() override;

    bool AddAgent(int id, AgentInterface *agent) override;
    const AgentInterface *GetAgent(int id) const override;
    const std::map<int, const AgentInterface*> &GetAgents() const override;
    const std::list<const AgentInterface*> &GetRemovedAgents() const;

    // framework internal methods to access members without restrictions
    void Clear() override;
    bool CreateGlobalDrivingView() override;

    // model callbacks
    int GetTimeOfDay() const override;

    void* GetGlobalDrivingView() override;
    void* GetGlobalObjects() override;
    void* GetWorldData() override;
    void* GetOsiGroundTruth();

    void QueueAgentUpdate(std::function<void(double)> func, double val) override;
    void QueueAgentRemove(const AgentInterface *agent) override;
    void SyncGlobalData() override;

    bool CreateScenery(SceneryInterface &scenery) override;

    AgentInterface *CreateAgentAdapterForAgent() override;

    const AgentInterface *GetSpecialAgent() override;

    std::list<AgentInterface*> GetAgentsByGroupType(AgentCategory &agentCategory);

    Position GetPositionByDistanceAndLane(double distance, int laneNumber) const override;
    Position GetPositionByDistanceAndLaneWithOffset(double distanceOnLane, double offset, int laneId) const;

    bool GetNextValidSOnLane(double distance, int laneNumber, double &nextValidSOnLane);
    bool GetLastValidSOnLane(double distance, int laneNumber, double &lastValidSOnLane);
    bool IsSValidOnLane(double distance, int laneNumber);
    double GetDistanceToEndOfLane(double currentPosition, int laneId);
    bool IntersectsWithAgent(double x, double y, double rotation, double length, double width, double center);

    polygon_t GetBoundingBoxAroundAgent(AgentInterface* agent, double width, double length);

    const AgentInterface* GetBicycle() const override;
    const AgentInterface* GetLastCarInlane(int laneNumber) override;

    Position RoadCoord2WorldCoord(RoadPosition roadCoord, std::string roadID) const;

    bool CreateWorldScenario(const std::string& scenarioFilename) override;
    bool CreateWorldScenery(const std::string& sceneryFilename) override;
    Weekday GetWeekday() const override;
    void SetTimeOfDay(int timeOfDay) override;
    void SetWeekday(Weekday weekday) override;
    void SetParameter(WorldParameter *worldParameter) override;

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
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    void InitTrafficObjects();
    void InitSectionViews();

    OWL::WorldData worldData;

    WorldParameter* worldParameter;

    AgentNetwork agentNetwork;

    const CallbackInterface *callbacks;

    mutable std::vector<const AgentInterface*> worldObjects;
    std::map<AgentInterface*, AgentAdapter*> agentList;

    const int stepSizeLookingForValidS = 100;
    SceneryInterface* scenery;

    World::Localization::Cache localizationCache;
};


