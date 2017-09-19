/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  sceneryImporter.h
//! @brief This file contains the importer of the scenery configuration.
//-----------------------------------------------------------------------------

#ifndef SCENERYIMPORTER_H
#define SCENERYIMPORTER_H

#include <map>
#include <list>
#include <QDomDocument>
#include "worldInterface.h"
#include "scenery.h"

namespace SimulationSlave
{

class SceneryConverter;

class SceneryImporter
{
public:    
    SceneryImporter() = default;
    SceneryImporter(const SceneryImporter&) = delete;
    SceneryImporter(SceneryImporter&&) = delete;
    SceneryImporter& operator=(const SceneryImporter&) = delete;
    SceneryImporter& operator=(SceneryImporter&&) = delete;
    virtual ~SceneryImporter() = default;

    //-----------------------------------------------------------------------------
    //! Imports data structures from the scenery configuration file e.g. OpenDrive file
    //!
    //! @param[in]  filename       path to OpenDrive file
    //! @param[in]  world          the destination world, where the scenery will be created
    //! @return                    true on success
    //-----------------------------------------------------------------------------
    static bool Import(const std::string &filename,
                       WorldInterface *world);

private:

    //-----------------------------------------------------------------------------
    //! @brief Parses lane elements in a lane section and stores their information
    //!         in a lane section object.
    //!
    //! OPENDrive attributes whose information is stored in the lane section:
    //! - lane tag:                     id and type
    //! - width subtag:                 sOffset, a, b, c, and d
    //! - link subtag:                  predecessor (and its id) and successor
    //!                                 (and its id)
    //!
    //! @param[in]  rootElement         DOM element containing the OPENDrive lanes
    //! @param[out] laneSection         Lane section with the contents of the DOM
    //!                                 elements
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool ParseLanes(QDomElement &rootElement,
                           RoadLaneSectionInterface *laneSection);

    //-----------------------------------------------------------------------------
    //! @brief Parses the road geometry into a road object.
    //!
    //! OPENDrive attributes whose information is stored in the road:
    //! - geometry tag:                 s, x, y, hdg and length
    //! - line subtag
    //! - arc subtag:                   curvature
    //! - spiral subtag:                curvStart and curvEnd
    //! - poly3 subtag:                 a, b, c, and d
    //!
    //! @param[in]  roadElement         DOM element containing the OPENDrive road
    //! @param[out] road                Road with the contents of the DOM element
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool ParseGeometries(QDomElement &roadElement,
                                RoadInterface *road);

    //-----------------------------------------------------------------------------
    //! @brief Parses the elevation profile into a road object.
    //!
    //! OPENDrive attributes whose information is stored in the road:
    //! - elevationProfile tag
    //! - elevation subtag:             s, a, b, c, and d
    //!
    //! @param[in]  roadElement         DOM element containing the OPENDrive road
    //! @param[out] road                Road with the contents of the DOM element
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool ParseElevationProfile(QDomElement &roadElement,
                                      RoadInterface *road);

    //-----------------------------------------------------------------------------
    //! @brief Parses the road links into a road object.
    //!
    //! OPENDrive attributes whose information is stored in the road:
    //! - link tag
    //! - predecessor subtag:           elementType (only "road" allowed), elementId,
    //!                                 contactPoint ("start" or "end")
    //! - successor subtag:             elementType (only "road" allowed), elementId,
    //!                                 contactPoint ("start" or "end")
    //! - neighbor subtag:              side ("left" or "right"), elementId, and
    //!                                 direction ("same" or "opposite")
    //!
    //! @param[in]  roadElement         DOM element containing the OPENDrive road
    //! @param[out] road                Road with the contents of the DOM element
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool ParseRoadLinks(QDomElement &roadElement, RoadInterface *road);

    //-----------------------------------------------------------------------------
    //! @brief Parses the lanes specification into a road object.
    //!
    //! OPENDrive attributes whose information is stored in the road:
    //! - lanes tag
    //! - laneOffset subtag:            s, a, b, c, and d
    //! - laneSection subtag:           s
    //! - left subtag:                  ParseLanes called
    //! - right subtag:                 ParseLanes called
    //!
    //!
    //! @param[in]  roadElement         DOM element containing the OPENDrive road
    //! @param[out] road                Road with the contents of the DOM element
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool ParseRoadLanes(QDomElement &roadElement, RoadInterface *road);

    //-----------------------------------------------------------------------------
    //! @brief Parses roads into a scenery object.
    //!
    //! OPENDrive attributes whose information is stored in the road:
    //! - road tag:                     id
    //! Then parses geometries, elevationProfile, road links, and road lanes.
    //!
    //! @param[in]  documentRoot        DOM element containing the OPENDrive road
    //! @param[out] scenery             Scenery with the contents of the DOM element
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    static bool ParseRoads(QDomElement &documentRoot,
                           Scenery &scenery);

    constexpr static const double SAMPLING_RATE = 1.0; // 1m sampling rate of reference line
};

} // namespace SimulationSlave

#endif // SCENERYIMPORTER_H
