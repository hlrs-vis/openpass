/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/** \addtogroup SpawnPoint
* @{
* \brief Spawns all agents.
*
* \details This module generates an AgentBlueprint for all agent categories (ego/scenery/common).
* The parameters of ego and scenery are defined in the Scenario.xml. There are several optional parameters, which if not set are randomized.
* The parameters of common cars get are randomized. The base for the randomized parameters is the runConfiguration.
* All randomized parameters and components of each agent are defined by the AgentSampler.
*
* \section SpawnPoint_ExternalParameters External parameters
* \subsection EgoSpawnParameters
* name | meaning
* -----|---------
* egoLaneId | lane on which ego will be spawned if existing.
* egoSpawnDistance | distance at which the ego will be spawned.
* egoSpawnRadius | radius in which common cars will be spawned around the ego.
*
* \subsection SpawnedItemsCounter
* name | meaning
* -----|---------
* maximumNumberOfAgents | maximumNumberOfAgents that will be spawned initially.
*
* \subsection SpawnPointParameters
* name | meaning
* -----|---------
* carsPerSecond | average traffic density per lane.
* platoonRate | probability of a common agent platooning the previous.
* rangeOfView | range of view of all agents.
* assumedDeceleration | Assumed deceleration for each agent per lane.
* timeToBrake | Time to brake for each agent per lane.
*
* \section SpawnPoint_InternalParameters Internal paramters
* \subsection SpawnPointConstants
* name  | value  | meaning
* ------|--------|-------------
* standardCycleTime | 100 | Standard cycle time in ms.
* minimumGapInSeconds  | 1.0  | Minimum gap time between two agents in seconds.
* nonPlatoonGapExtension | 10.0 | Average time which is added to the gap in case the agent is not platooning.
* minimumDistanceToStart | 0.0 | Minimum distance where agents will be spawned on the starting road.
* stdDeviationTimeToBrake | 0.4 | Standard deviation which is applied to the time to brake.
* distanceToEndOfLane | 50.0 | Minimum distance to end of lane, which is required for an agent to spawn on said lane.
* maximumVelocityPerLane | 46.51, 40.99, 35.86, 35.86, 35.86, 35.86, 35.86, 35.86, 35.86, 35.86 | Maximum velocity for each lane.
*
* @} */

#pragma once

#include "Interfaces/spawnPointInterface.h"
#include "Interfaces/worldInterface.h"
#include "Interfaces/samplerInterface.h"
#include "Interfaces/scenarioInterface.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "SpawnPointSceneryParser.h"

/**
* \brief spawns agents for all scenarios
* \details This class implements a SpawnPointInterface which is used by the framework to fill out
* AgentBlueprints. The spawnpoint calls the agentsampler to fill out all probability related
* parameters of the blueprint. The spawnpoint then adds the spawning depended parameters to the
* agentblueprint. The agentblueprint is then returned to the framework.
* \ingroup SpawnPoint
*/
class SpawnPoint : public SpawnPointInterface
{
private:
    static constexpr int NUMBER_OF_TRIALS_STOCHASTIC = 5;

public:
    const std::string COMPONENTNAME = "SpawnPoint";

    SpawnPoint(
            WorldInterface* world,
            const ParameterInterface* parameters,
            const CallbackInterface* callbacks,
            AgentBlueprintProviderInterface* agentBlueprintProvider,
            SamplerInterface* sampler,
            ScenarioInterface* scenario);
    SpawnPoint(const SpawnPoint&) = delete;
    SpawnPoint(SpawnPoint&&) = delete;
    SpawnPoint& operator=(const SpawnPoint&) = delete;
    SpawnPoint& operator=(SpawnPoint&&) = delete;
    virtual ~SpawnPoint() = default;

    /*!
     * \brief Generates fills out an AgentBlueprint
     * \details This function gets an empty AgentBlueprint and fills out all parameters of the AgentBlueprint.  Agentsampler is used to fill out all stochastic based parameters and the components of an agent.
     *
     * @param[in|out] agentBlueprint is used to create an agent.
     */
    bool GenerateAgent(AgentBlueprintInterface *agentBlueprint);

    /*!
    * \brief Extracts SpawnPoint parameters.
    *
    * \details Extracts the Spawnpoint parameters into the corresponding membervariables.
    *
    *
    * @param[in]     parameters    SpawnPoint parameters
    */
    void ExtractSpawnPointParameters(const ParameterInterface* parameters);

    /*!
    * \brief Calculates derived parameters.
    *
    * \details Uses the SpawnPoint parameters to calculate different parameters.
    *
    */
    void CalculateDerivedParameters();

    // for testing
    SpawnPoint(WorldInterface* world,
               const ParameterInterface* parameters,
               AgentBlueprintProviderInterface *agentBlueprintProvider,
               SamplerInterface* sampler,
               ScenarioInterface* scenario,
               SpawnPointState spawnPointState);

    /*!
    * \brief Calculates the SpawnInfo of an agent.
    *
    * \details Calculates the SpawnInfo of an agent. Depending on the SpawnPoint state different methods are used.
    *
    *
    * @param[in]     laneParameters    Parameters of the lane where the next agent will be spawned if possible.
    * @param[in]     agentBlueprint    AgentBlueprint contains all parameters of an agent.
    * @return        true if successful. false if not successful.
    */
    bool GetNextSpawnCarInfo(LaneParameters& laneParameters,
                             AgentBlueprintInterface* agentBlueprint);

    SpawnInfo GetSpawnInfo()
    {
        return spawnInfo;
    }

    /*!
    * \brief Checks if the offset is valid for a specific lane. Only for unit testin !
    *
    * \details  Checks if the vehicle is more than 50% inside the lane and is not placed on an invalid lane
    *
    * @param[in]     roadId   id of the referenced openDrive road
    * @param[in]     laneId   id of the lane
    * @param[in]     distanceFromStart    s-coordinate
    * @param[in]     offset    vehicle offset inside the lane
    * @param[in]     vehicleWidth    width  of the vehicle
    * @return        true if valid. false if not valid.
    */
    bool IsOffsetValidForLane(std::string roadId, int laneId, double distanceFromStart, double offset, double vehicleWidth);

    /*!
    * \brief Calculates the mean velocities per lane (for spawning) based on the specified homogenities.
    *        Reference for the calculation is the right-most lane.
    *
    * @param[in]     roadId      id of the referenced openDrive road
    * @param[in]     distance    s-coordinate at which the lanes are evaluated
    * @return        vector of velocities per lane
    */
    std::vector<double> CalculateMeanVelocities(std::string roadId, double distance);

    /*!
    * \brief Updates the SpawnParameter of an AgentBlueprint.
    *
    * \details Triggers functions to update the SpawnInfo.
    * Then updates the SpawnParameter of an AgentBlueprint.
    *
    * @param[in|out]     agentBlueprint         AgentBlueprint which is being updated.
    * @param[in]         laneParameters         LaneParameters that were previsouly calculated.
    * @return            true if successful. false if not.
    */
    bool CalculateSpawnParameter(AgentBlueprintInterface* agentBlueprint,
                                 LaneParameters laneParameters);

    /*!
    * \brief Sets the predefined SpawnParameters in the AgentBlueprint.
    *
    * @param[in]     agentBlueprint             AgentBlueprint containing all information of an agent which is about to be spawned.
    */
    void SetPredefinedParameter(AgentBlueprintInterface* agentBluePrint);

    /*! For testing
    */
    void SetSpawnInfo(SpawnInfo spawnInfo)
    {
        this->spawnInfo = spawnInfo;
    }

    /*!
    * \brief Updates the LaneParameters.
    *
    * \details Depending on the SpawnPoint state different functions are called to determine the next LaneParameters.
    *
    *
    * @param[out]       laneParameters    LaneParameters of an common agent.
    * @return           true if successful. false if not.
    */
    bool GetSpawningLane(LaneParameters& laneParameters);


private:
    SpawnPointConstants spawnPointConstants;
    SpawnPointParameters spawnPointParameters;
    EgoSpawnParameters egoSpawnParameters;

    ScenarioInterface* scenario;
    SpawnPointHelper::SceneryParser sceneryParser;
    AgentBlueprintProviderInterface* agentBlueprintProvider;
    SamplerInterface* sampler;

    bool isFirstRespawning = true;
    unsigned int egoAndSceneryLanesIndex = 0;
    unsigned int scenarioAgentIterator = 0;


    double sceneryCarCurrentSPosition = -1;
    double sceneryCarCurrentOffset = -9999;

    SpawnInfo spawnInfo;
    SpawnPointState spawnPointState = SpawnPointState::EgoPlacement;
    std::vector<LaneParameters> egoAndSceneryLaneParameters;

    /*!
    * \brief Returns time till spawn for all pre run agents.
    *
    * @return time till spawn
    */
    double GetPreRunTimeTillSpawn();

    /*!
    * \brief Returns the car rate.
    *
    * \details Returns the car rate in seconds. While Common::Spawning, there is a different car rate.
    *
    * @return Car rate
    */
    double GetCarRate();

    /*!
    * \brief Returns the seperation to the next agent.
    *
    * \details Returns the distance at which an agent will be spawned compared to an other agent.
    *
    *
    * @param[in]     gapInSeconds           Gap between agents in seconds.
    * @param[in]     egoVelocity            Velocity of the new agent in m/s.
    * @param[in]     consideredCarLengths   Length of the agent vehicles between both refernce points.
    * @return        Seperation in m.
    */
    double GetSeparation(double gapInSeconds,
                         double egoVelocity,
                         double consideredCarLengths);

    /*!
    * \brief Returns wether the scenery stage is completed.
    *
    * \details Returns wether the scenery stage is completed.
    *
    * @return    true if completed. false if not completed.
    */
    bool IsSceneryCompleted();

    /*!
    * \brief Updates the spawn point state.
    *
    * \details Updates the spawn point to the next state.
    *
    */
    void UpdateSpawnPointState();

    /*!
    * \brief Calculates the SpawnInfo for the ego agent.
    *
    * \details Calculates the SpawnInfo for the ego agent if possible.
    *
    *
    * @param[in]     laneId             Id of the lane where the ego agent is supposed to be.
    * @param[in]     egoVelocity        Velocity of the ego agent.
    * @param[in]     rearCarLength      Length of the ego agent from the reference point to the rear.
    * @param[in]     frontCarLength     Length of the ego agent from the reference point to the front.
    * @return        SpawnInfo of the ego agent.
    */
    SpawnInfo GetEgoSpawnCarInfo(int laneId,
                                 double egoVelocity,
                                 double rearCarLength);

    /*!
    * \brief Calculates the SpawnInfo for a scenery agent.
    *
    * \details Calculates the SpawnInfo for the scenery agent if possible.
    *
    *
    * @param[in]     laneId             Id of the lane where the scenery agent is supposed to be.
    * @param[in]     egoVelocity        Velocity of the scenery agent.
    * @param[in]     rearCarLength      Length of the scenery agent from the reference point to the rear.
    * @return        SpawnInfo of the scenery agent.
    */
    SpawnInfo GetScenerySpawnCarInfo(int laneId,
                                     double egoVelocity,
                                     double rearCarLength);

    /*!
    * \brief Calculates the SpawnInfo for a common agent which gets placed in front of other agents.
    *
    * \details Calculates the SpawnInfo for a common agent which gets placed in front of either an ego or scenery agent if possible.
    *
    *
    * @param[in]     laneParameters     LaneParameters of the lane where the common agent is supposed to be.
    * @param[in]     gapInSeconds       Gap between agents in seconds.
    * @param[in]     egoVelocity        Velocity of the common agent.
    * @param[in]     frontCarRearLength Length of the agent in front from the reference point to the front.
    * @return        SpawnInfo of the common agent.
    */
    SpawnInfo GetNextSpawnCarInfoFront(LaneParameters laneParameters,
                                       double gapInSeconds,
                                       double egoVelocity,
                                       double rearLength);

    /*!
    * \brief Calculates the SpawnInfo for a common agent which gets placed behind of other agents.
    *
    * \details Calculates the SpawnInfo for a common agent which gets placed behind of either an ego or scenery agent if possible.
    *
    *
    * @param[in]     laneParameters     LaneParameters of the lane where the common agent is supposed to be.
    * @param[in]     gapInSeconds       Gap between agents in seconds.
    * @param[in]     egoVelocity        Velocity of the common agent.
    * @param[in]     frontCarLength     Length of the common agent from the reference point to the front.
    * @return        SpawnInfo of the common agent.
    */
    SpawnInfo GetNextSpawnCarInfoRear(LaneParameters laneParameters,
                                      double gapInSeconds,
                                      double egoVelocity,
                                      double frontCarLength);

    /*!
    * \brief Calculates the SpawnInfo for a common agent which gets placed in a lane without other agents.
    *
    * \details Calculates the SpawnInfo for a common agent which gets placed in a lane without other agents if possible.
    *
    *
    * @param[in]     LaneParameters                         LaneParameters of the lane where the common agent is supposed to be.
    * @param[in]     gapInSeconds                           Gap between agents in seconds.
    * @param[in]     egoVelocity                            Velocity of the common agent.
    * @param[in]     distanceReferencePointToLeadingEdge    Length of the common agent from the reference point to the front.
    * @return        SpawnInfo of the common agent.
    */
    SpawnInfo GetNextSpawnCarInfoPreRunRegular(LaneParameters laneParameters,
                                               double gapInSeconds,
                                               double egoVelocity,
                                               double distanceReferencePointToLeadingEdge);

    /*!
    * \brief Calculates the SpawnInfo for a common agent during runtime.
    *
    * \details Calculates the SpawnInfo for a common agent during runtime.
    *
    * @param[in]     roadId                         Id of the referenced openDrive road
    * @param[in]     laneId                         Id of the lane where the common agent is supposed to be.
    * @param[in]     gapInSeconds                   Gap between agents in seconds.
    * @param[in]     egoVelocity                    Velocity of the common agent.
    * @param[in]     rearCarLength                  Length of the common agent from the reference point to the rear.
    * @param[in]     distanceCogToLeadingEdge       Length of the common agent from the reference point to the front.
    * @return        SpawnInfo of the common agent.
    */
    SpawnInfo GetNextSpawnCarInfoRegular(std::string roadId,
                                         int laneId,
                                         double gapInSeconds,
                                         double egoVelocity,
                                         double rearCarLength,
                                         double distanceReferencePointToLeadingEdge);

    /*!
    * \brief Respawns an agent.
    *
    * \details Respawns an agent. This is only activated if the manipulator for this event exists.
    *
    *
    * @param[in]     LaneParameters                         LaneParameters of the lane where the common agent is supposed to be.
    * @param[in]     gapInSeconds                   Gap between agents in seconds.
    * @param[in]     egoVelocity                    Velocity of the common agent.
    * @param[in]     frontCarRearLength Length of the agent in front from the reference point to the front.
    * @param[in]     distanceReferencePointToLeadingEdge    Length of the common agent from the reference point to the front.
    * @return        SpawnInfo of the common agent.
    */
    SpawnInfo GetNextSpawnCarInfoRespawn(LaneParameters laneParameters,
                                         double gapInSeconds,
                                         double egoVelocity,
                                         double frontCarRearLength,
                                         double distanceReferencePointToLeadingEdge);

    /*!
    * \brief Helper of the SetPredefinedParameter method.
    *
    * \details Sets the predefined SpawnParameters in the AgentBlueprint.
    *
    *
    * @param[in]     agentBlueprint             AgentBlueprint containing all information of an agent which is about to be spawned.
    * @param[in]     agentTypeGroupName         Name of the agent type group the agent belongs to.
    * @param[in]     scenarioEntity             The entity information from the scenario file
    */
    void SetPredefinedParameterHelper(AgentBlueprintInterface* agentBlueprint,
                                      AgentCategory agentCategory,
                                      const ScenarioEntity& scenarioEntity);

    /*!
    * \brief Calculates the timely gap between to agents
    *
    * \details Calculates the gap between to agents in seconds.
    * It uses the AgentSampler for all stochastic related tasks.
    *
    * @return    Gap in seconds.
    */
    double CalculateGap();

    /*!
    * \brief Revises the velocity to match a lower velocity limit
    *
    * \details Revises the velocity to match a minimum velocity limit.
    * The lower limit only has a mean value and differs for each agent.
    *
    * @param[in]     velocity       Previously calculated velocity in m/s.
    * @param[in]     mean           Average minimum velocity in m/s.
    * @param[in]     stdDeviation   Standard deviation for the minimum velocity.
    * @return        Velocity in m/s.
    */
    double ReviseLowerVelocityLimit(double velocity,
                                    double mean,
                                    double stdDeviation);

    /*!
    * \brief Revises the velocity to match a upper velocity limit
    *
    * \details Revises the velocity to match a maximum velocity limit.
    * The upper limit only is a previous set value for each lane.
    *
    * @param[in]     velocity       Previously calculated velocity in m/s.
    * @param[in]     lane           Lane index.
    * @return        Velocity in m/s.
    */
    double ReviseUpperVelocityLimit(double velocity,
                                    int laneId);

    /*!
    * \brief Calculates the velocity of the agent which is being spawned.
    *
    * \details Calculates the velocity of the agent which is being spawned.
    * This function also considers possible predefined velocities.
    * The velocity is revised to match maximum and minimum velocities.
    *
    *
    * @param[in]     laneId    Lane index.
    * @param[in]     offset    S offset.
    * @return        Velocity in m\s.
    */
    double CalculateEgoVelocity(int laneId, double distance);

    /*!
    * \brief Checks wether a distance is valid for spawning on a lane.
    *
    * \details Checks wether a distance is valid for spawning on a lane.
    * This method considers wether the lane exists at this distance.
    * Then it checks wether another agent is already at this position.
    * Then it tests wether the distance is to close to the end of the road.
    *
    * @param[in]     roadId                 Id of the referenced openDrive road
    * @param[in]     laneId                 Lane id.
    * @param[in]     distanceCOG            Distance to the reference point of the agent.
    * @param[in]     distanceRearEdge       Distance to the rear point of the agent.
    * @return        true if valid. false if not valid.
    */
    bool AreSpawningCoordinatesValid(std::string roadId,
                                     int laneId,
                                     double distanceCOG,
                                     double offset);

    /*!
    * \brief Checks wether a distance is within the area around the ego.
    *
    * \details Checks wether a distance is within the area around the ego.
    * This method considers wether the lane exists at this distance.
    * Then it checks wether another agent is already at this position.
    * Then it tests wether the distance is to close to the end of the road.
    *
    * @param[in]     distanceReferncePoint      Distance of the refernce point of an agent.
    * @return        true if within the radius. false if not.
    */
    bool IsWithinEgoRadius(double distanceReferncePoint);

    /*!
    * \brief Updates the LaneParameters to contain the ego Laneparameters.
    *
    * \details Looks up the ego LaneParameters specified in the scenario file.
    *
    *
    * @param[out]       laneParameters    LaneParameters of an ego agent.
    * @return           true if successful. false if not.
    */
    bool GetNextEgoLane(LaneParameters& laneParameters);

    /*!
    * \brief Updates the LaneParameters to contain the next scenery Laneparameters.
    *
    * \details Looks up the next scenery LaneParameters specified in the scenario file.
    *
    *
    * @param[out]       laneParameters    LaneParameters of an scenery agent.
    * @return           true if successful. false if not.
    */
    bool GetNextSceneryLane(LaneParameters& laneParameters);

    /*!
    * \brief Updates the LaneParameters to contain the next common Laneparameters.
    *
    * \details Looks up the next common LaneParameters for lanes where ego or scenery agents were already spawned.
    *
    *
    * @param[out]       laneParameters    LaneParameters of an common agent.
    * @return           true if successful. false if not.
    */
    bool GetNextCommonLane(LaneParameters& laneParameters);

    /*!
    * \brief Updates the LaneParameters to contain the next common Laneparameters.
    *
    * \details Looks up the next common LaneParameters for lanes where neither ego nor scenery agents were already spawned.
    *
    *
    * @param[out]       laneParameters    LaneParameters of an common agent.
    * @return           true if successful. false if not.
    */
    bool GetNextPlacementLane(LaneParameters& laneParameters);

    /*!
    * \brief Updates the LaneParameters to contain the next common Laneparameters.
    *
    * \details Looks up the next common LaneParameters for lanes when the SpawnPoint state is on respawning.
    *
    *
    * @param[out]       laneParameters    LaneParameters of an common agent.
    * @return           true if successful. false if not.
    */
    bool GetNextRespawnLane(LaneParameters& laneParameters);

    bool RemoveLastPlacementLane();

    /*!
    * \brief Updates the LaneParameters to contain the next common Laneparameters.
    *
    * \details Looks up the next common LaneParameters when the SpawnPoint state is on regular spawning.
    *
    *
    * @param[out]       laneParameters    LaneParameters of an common agent.
    * @return           true if successful. false if not.
    */
    bool GetNextSpawningLane(LaneParameters& laneParameters);

    /*!
    * \brief Logs a message for debug mode.
    *
    * \details Logs a message for debug mode.
    *
    * @param[in]     message    Logged message.
    */
    void LogCurrentDebugStatus(std::string message);

    /*!
    * \brief Logs a message for warning mode.
    *
    * \details Logs a message for warning mode.
    *
    * @param[in]     message    Logged message.
    */
    void LogWarning(std::string message);

    /*!
    * \brief Logs a error message.
    *
    * \details Logs a error message.
    *
    * @param[in]     message    Logged message.
    */
    void LogError(std::string message);

    /*!
    * \brief Converts per hour to per second.
    *
    * \details Divides the input by 3600.
    *
    * @param[in]     perHourValue    Value per hours.
    * @return        Value per seconds.
    */
    inline double ConvertPerHourToPerSecond(double perHourValue) const;

    /*!
    * \brief Checks if the given lane is an ego or scenery lane.
    *
    * \details Checks whether ego or scenery agents are spawned on this lane.
    *
    * @param[in]        laneId         lane id.
    * @return           true if ego or scenery lane. false if not.
    */
    bool IsEgoOrSceneryLane(int laneId);

    /*!
    * \brief Checks wether a LaneParameters are already listed as ego or scenery LaneParameters.
    *
    * \details Checks wether a LaneParameters are already listed as ego or scenery LaneParameters.
    *
    * @param[in]        newLaneParameters         LaneParameters.
    * @return           true if already listed. false if not.
    */
    bool IsLaneParameterAlreadyListed(LaneParameters laneParameters);

    /*!
    * \brief Checks wether a new agent intersects with an existing agent
    *

    * @return           true if if new agent intersects with existing agent
    */
    bool NewAgentIntersectsWithExistingAgent(int laneId, double offset);

    /*!
     * \brief TryGetSpawnDistance
     *
     * @param roadId        Id of the referenced openDrive road
     * \param laneId        Lane ID of interest (e.g. -1)
     * \param sAttribute    potentially stochastic attribute for the spawn distance
     * \return              true if (rolled) spawn distance is on lane of interest
     */
    std::tuple<bool, double> TryGetSpawnDistance(std::string roadId, int laneId, SpawnAttribute sAttribute);

    void CalculateAttributeValue(SpawnAttribute& attribute);


    AgentBlueprintInterface* currentBluePrint;
    double currentDistanceToLaneStart;

    double GetValueForLane(int laneIdx, const std::vector<double>& values);
};


