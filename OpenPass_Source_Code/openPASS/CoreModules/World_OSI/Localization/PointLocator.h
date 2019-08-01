/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <tuple>

#include "LocalizationCommon.h"
#include "Common/vector2d.h"
#include "WorldData.h"
#include "OWL/DataTypes.h"
#include "LaneWalker.h"
#include "GeometryProcessor.h"
#include "PointAggregator.h"
#include "SearchInitializer.h"

namespace World {
namespace Localization {

class PointAggregator;

class PointLocator
{
private:
    PointAggregator& pointAggregator;
    std::list<OWL::SearchablePoint> searchPoints;

    SearchInitializer searchInitializer;

    // search state objects:
    OWL::CSection* section{nullptr};
    double sectionOffset;
    LaneWalkers upstreamLaneWalkers{};
    LaneWalkers downstreamLaneWalkers{};

    size_t indexUpstreamWalker{0};
    size_t indexDownstreamWalker{0};

    int stepsAfterFirstHit{0};
    int maxStepsAfterFirstHit{20};

    bool suppressWalkerChange{false};
    bool firstHit{false};

    /*!
     * \brief Creates a SearchInitializer and stores it if the locator
     * does not already have a SearchInitializer stored
     */
    void SaveSearchInitializerOnFirstHit();

    /*!
     * \brief Creates a SearchInitializer and stores the current locator metadata in it.
     */
    void SaveSearchInitializer();

    /*!
     * \brief Resets internal states (e.g. searchInitializer).
     */
    void InitLocate(const std::list<OWL::SearchablePoint>& searchPoints);

    /*!
     * \brief Rewinds the given LaneWalker to stored s coordinate of first located point.
     * This methode is only called when a new searchInitializer is saved.
     */
    void RewindWalkers(LaneWalkers &laneWalkers);

    /*!
     * \brief Creates new LaneWalkers for the section
     */
    void WrapLanes(bool startAtRoadStart);

    /*!
     * \brief Calculates the maximum number of LaneWalker steps for each direction to search
     * the remaining points after first point is found
     *
     * This is the number of steps required for one locator to move at least maxDistance multipled
     * by the number of LaneWalkers per direction.
     *
     * \param maxDistance  maximum search distance after first point is found
     */
    void CalculateMaxStepsAfterFirstHit(double maxDistance);

    /*!
     * \brief Main locate logic
     *
     * Try to locate all points inside the currentSection until either all points where
     * found or all LaneWalkers have reached the end of the section
     */
    void LocatePoints();

    /*!
     * \brief Increments current LaneWalkers and index
     */
    void Step();

public:
    PointLocator(PointAggregator& pointAggregator,
                 const OWL::Interfaces::Section* section);

    /*!
     * \brief Locates the given points in the road network and stores their road coordinates
     * in the PointAggregator
     *
     * \param searchPoints points to locate
     * \param maxDistance  maximum search distance after first point is found
     */
    void Locate(const std::list<OWL::SearchablePoint>& searchPoints, double maxDistance);

    /*!
     * \brief returns true if the locator was able to locate all points
     */
    bool AllPointsLocated();

    /*!
     * \brief Returns the SearchInitializer to speed up locating the same agent in the next timestamp (Quickstart)
     */
    SearchInitializer GetSearchInitializer() const;

    /*!
     * \brief Sets a SearchInitializer where the locator will start trying
     * to locate the points.
     *
     * A moving agent will in the next timestamp still be in the vicinity of its previous location.
     * To speed up locating the same agent again we use a SearchInitializer which tells the
     * locator where to start (Quickstart)
     *
     * \param searchInitializer SearchInitializer of same agent from previous timestamp
     */
    void SetSearchInitializer(SearchInitializer searchInitializer);
};

} // Localization
} // World

