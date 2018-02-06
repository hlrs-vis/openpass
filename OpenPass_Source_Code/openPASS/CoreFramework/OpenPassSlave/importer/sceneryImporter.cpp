/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <QApplication>
#include <algorithm>
#include <utility>
#include <limits>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <QDir>
#include <QFile>
#include "log.h"
#include "scenery.h"
#include "sceneryImporter.h"
#include "xmlParser.h"

// local helper macros
#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
{ \
    LOG_INTERN(LogLevel::Warning) << "an error occurred during scenery import"; \
    return false; \
    } \
    } \
    while(0);

namespace SimulationSlave {

bool SceneryImporter::ParseLanes(QDomElement &rootElement,
                                 RoadLaneSectionInterface *laneSection)
{
    QDomElement roadLaneElement;

    QDomNode node = rootElement.firstChildElement("lane");
    if (node.isNull())
    {
        // empty container
        return true;
    }

    roadLaneElement = node.toElement();
    if (roadLaneElement.isNull())
    {
        return false;
    }

    while (!roadLaneElement.isNull())
    {
        int roadLaneId;
        CHECKFALSE(SimulationCommon::ParseAttributeInt(roadLaneElement, "id", roadLaneId));

        std::string roadLaneTypeStr;
        CHECKFALSE(SimulationCommon::ParseAttributeString(roadLaneElement, "type", roadLaneTypeStr));

        RoadLaneTypeType roadLaneType = RoadLaneTypeType::Undefined;
        if (0 == roadLaneTypeStr.compare("none"))
        {
            roadLaneType = RoadLaneTypeType::None;
        }
        else if (0 == roadLaneTypeStr.compare("driving"))
        {
            roadLaneType = RoadLaneTypeType::Driving;
        }
        else if (0 == roadLaneTypeStr.compare("stop"))
        {
            roadLaneType = RoadLaneTypeType::Stop;
        }
        else if (0 == roadLaneTypeStr.compare("shoulder"))
        {
            roadLaneType = RoadLaneTypeType::Shoulder;
        }
        else if (0 == roadLaneTypeStr.compare("biking"))
        {
            roadLaneType = RoadLaneTypeType::Biking;
        }
        else if (0 == roadLaneTypeStr.compare("sidewalk"))
        {
            roadLaneType = RoadLaneTypeType::Sidewalk;
        }
        else if (0 == roadLaneTypeStr.compare("border"))
        {
            roadLaneType = RoadLaneTypeType::Border;
        }
        else if (0 == roadLaneTypeStr.compare("restricted"))
        {
            roadLaneType = RoadLaneTypeType::Restricted;
        }
        else if (0 == roadLaneTypeStr.compare("parking"))
        {
            roadLaneType = RoadLaneTypeType::Parking;
        }
        else if (0 == roadLaneTypeStr.compare("mwyEntry"))
        {
            roadLaneType = RoadLaneTypeType::MwyEntry;
        }
        else if (0 == roadLaneTypeStr.compare("mwyExit"))
        {
            roadLaneType = RoadLaneTypeType::MwyExit;
        }
        else if (0 == roadLaneTypeStr.compare("special1"))
        {
            roadLaneType = RoadLaneTypeType::Special1;
        }
        else if (0 == roadLaneTypeStr.compare("special2"))
        {
            roadLaneType = RoadLaneTypeType::Special2;
        }
        else if (0 == roadLaneTypeStr.compare("special3"))
        {
            roadLaneType = RoadLaneTypeType::Special3;
        }
        else if (0 == roadLaneTypeStr.compare("tram"))
        {
            roadLaneType = RoadLaneTypeType::Tram;
        }
        else
        {
            LOG_INTERN(LogLevel::Error) << "invalid road lane type";
            return false;
        }

        RoadLaneInterface *roadLane = laneSection->AddRoadLane(roadLaneId,
                                                               roadLaneType);
        CHECKFALSE(roadLane);
        LOG_INTERN(LogLevel::DebugCore) << "lane: type " << (int)roadLaneType << ", id=" << roadLaneId;

        if (0 != roadLaneId) // skip center lanes
        {
            QDomElement roadLaneWidthElement;
            // at least one width element necessary
            CHECKFALSE(SimulationCommon::GetFirstChildElement(roadLaneElement, "width", roadLaneWidthElement));
            while (!roadLaneWidthElement.isNull())
            {
                double roadLaneSOffset, roadLaneA, roadLaneB, roadLaneC, roadLaneD;
                CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadLaneWidthElement, "sOffset",
                                                                  roadLaneSOffset));
                CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadLaneWidthElement, "a", roadLaneA));
                CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadLaneWidthElement, "b", roadLaneB));
                CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadLaneWidthElement, "c", roadLaneC));
                CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadLaneWidthElement, "d", roadLaneD));

                roadLane->AddWidth(roadLaneSOffset,
                                   roadLaneA,
                                   roadLaneB,
                                   roadLaneC,
                                   roadLaneD);

                roadLaneWidthElement = roadLaneWidthElement.nextSiblingElement("width");
            }
        }

        QDomElement roadLaneLinkElement;
        if (SimulationCommon::GetFirstChildElement(roadLaneElement, "link", roadLaneLinkElement))
        {
            int roadLaneLinkItemId;
            QDomElement roadLaneLinkItemElement;
            if (SimulationCommon::GetFirstChildElement(roadLaneLinkElement, "successor",
                                                       roadLaneLinkItemElement))
            {
                CHECKFALSE(SimulationCommon::ParseAttributeInt(roadLaneLinkItemElement, "id", roadLaneLinkItemId));
                roadLane->AddSuccessor(roadLaneLinkItemId);
            }

            if (SimulationCommon::GetFirstChildElement(roadLaneLinkElement, "predecessor",
                                                       roadLaneLinkItemElement))
            {
                CHECKFALSE(SimulationCommon::ParseAttributeInt(roadLaneLinkItemElement, "id", roadLaneLinkItemId));
                roadLane->AddPredecessor(roadLaneLinkItemId);
            }
        }

        roadLaneElement = roadLaneElement.nextSiblingElement("lane");
    } // lane loop

    return true;
}

bool SceneryImporter::ParseGeometries(QDomElement &roadElement,
                                      RoadInterface *road)
{
    // road plan view
    QDomElement roadPlanView;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(roadElement, "planView", roadPlanView));

    QDomElement roadGeometryHeaderElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(roadPlanView, "geometry",
                                                      roadGeometryHeaderElement));
    while (!roadGeometryHeaderElement.isNull())
    {
        double roadGeometryS;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "s", roadGeometryS));

        double roadGeometryX;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "x", roadGeometryX));

        double roadGeometryY;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "y", roadGeometryY));

        double roadGeometryHdg;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "hdg",
                                                          roadGeometryHdg));

        double roadGeometryLength;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "length",
                                                          roadGeometryLength));

        QDomElement roadGeometryElement;
        if (SimulationCommon::GetFirstChildElement(roadGeometryHeaderElement, "line", roadGeometryElement))
        {
            CHECKFALSE(road->AddGeometryLine(roadGeometryS,
                                             roadGeometryX,
                                             roadGeometryY,
                                             roadGeometryHdg,
                                             roadGeometryLength));
        }
        else if (SimulationCommon::GetFirstChildElement(roadGeometryHeaderElement, "arc",
                                                        roadGeometryElement))
        {
            double roadGeometryCurvature;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryElement,
                                                              "curvature",
                                                              roadGeometryCurvature));

            CHECKFALSE(road->AddGeometryArc(roadGeometryS,
                                            roadGeometryX,
                                            roadGeometryY,
                                            roadGeometryHdg,
                                            roadGeometryLength,
                                            roadGeometryCurvature));
        }
        else if (SimulationCommon::GetFirstChildElement(roadGeometryHeaderElement, "spiral",
                                                        roadGeometryElement))
        {
            double roadGeometryCurvStart;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryElement,
                                                              "curvStart",
                                                              roadGeometryCurvStart));

            double roadGeometryCurvEnd;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryElement,
                                                              "curvEnd",
                                                              roadGeometryCurvEnd));

            CHECKFALSE(road->AddGeometrySpiral(roadGeometryS,
                                               roadGeometryX,
                                               roadGeometryY,
                                               roadGeometryHdg,
                                               roadGeometryLength,
                                               roadGeometryCurvStart,
                                               roadGeometryCurvEnd));
        }
        else if (SimulationCommon::GetFirstChildElement(roadGeometryHeaderElement, "poly3",
                                                        roadGeometryElement))
        {
            double roadGeometryA;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryElement,
                                                              "a",
                                                              roadGeometryA));

            double roadGeometryB;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryElement,
                                                              "b",
                                                              roadGeometryB));

            double roadGeometryC;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryElement,
                                                              "c",
                                                              roadGeometryC));

            double roadGeometryD;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryElement,
                                                              "d",
                                                              roadGeometryD));

            CHECKFALSE(road->AddGeometryPoly3(roadGeometryS,
                                              roadGeometryX,
                                              roadGeometryY,
                                              roadGeometryHdg,
                                              roadGeometryLength,
                                              roadGeometryA,
                                              roadGeometryB,
                                              roadGeometryC,
                                              roadGeometryD));
        }
        else
        {
            LOG_INTERN(LogLevel::Error) << "invalid geometry";
            return false;
        }

        LOG_INTERN(LogLevel::DebugCore) << "road geometry: s: " << roadGeometryS
                                        << ", x: " << roadGeometryX
                                        << ", y: " << roadGeometryY
                                        << ", hdg: " << roadGeometryHdg
                                        << ", length: " << roadGeometryLength;

        roadGeometryHeaderElement = roadGeometryHeaderElement.nextSiblingElement("geometry");
    } // road geometry loop

    return true;
}

bool SceneryImporter::ParseElevationProfile(QDomElement &roadElement,
                                            RoadInterface *road)
{
    QDomElement elevationProfileElement;
    if (SimulationCommon::GetFirstChildElement(roadElement, "elevationProfile",
                                               elevationProfileElement))
    {
        QDomElement elevationElement;
        if (SimulationCommon::GetFirstChildElement(elevationProfileElement, "elevation", elevationElement))
        {
            while (!elevationElement.isNull())
            {
                double elevationS, elevationA, elevationB, elevationC, elevationD;
                CHECKFALSE(SimulationCommon::ParseAttributeDouble(elevationElement,
                                                                  "s",
                                                                  elevationS));

                CHECKFALSE(SimulationCommon::ParseAttributeDouble(elevationElement,
                                                                  "a",
                                                                  elevationA));

                CHECKFALSE(SimulationCommon::ParseAttributeDouble(elevationElement,
                                                                  "b",
                                                                  elevationB));

                CHECKFALSE(SimulationCommon::ParseAttributeDouble(elevationElement,
                                                                  "c",
                                                                  elevationC));

                CHECKFALSE(SimulationCommon::ParseAttributeDouble(elevationElement,
                                                                  "d",
                                                                  elevationD));

                road->AddElevation(elevationS,
                                   elevationA,
                                   elevationB,
                                   elevationC,
                                   elevationD);

                elevationElement = elevationElement.nextSiblingElement("elevation");
            } // elevation loop
        } // if elevations exist
    } // if elevation profiles exist

    return true;
}

bool SceneryImporter::ParseRoadLinks(QDomElement &roadElement,
                                     RoadInterface *road)
{
    QDomElement roadLinkElement;
    if (SimulationCommon::GetFirstChildElement(roadElement, "link", roadLinkElement))
    {
        QDomElement roadLinkItemElement;
        if (SimulationCommon::GetFirstChild(roadLinkElement, roadLinkItemElement))
        {
            while (!roadLinkItemElement.isNull())
            {
                RoadLinkType roadLinkType = RoadLinkType::Undefined;
                RoadLinkElementType roadLinkElementType = RoadLinkElementType::Undefined;
                std::string roadLinkElementId;
                ContactPointType roadLinkContactPoint = ContactPointType::Undefined;
                RoadLinkDirectionType roadLinkDirection = RoadLinkDirectionType::Undefined;
                RoadLinkSideType roadLinkSide = RoadLinkSideType::Undefined;

                if (0 == roadLinkItemElement.tagName().compare("predecessor")
                        || 0 == roadLinkItemElement.tagName().compare("successor"))
                {
                    if (0 == roadLinkItemElement.tagName().compare("predecessor"))
                    {
                        roadLinkType = RoadLinkType::Predecessor;
                    }
                    else
                    {
                        roadLinkType = RoadLinkType::Successor;
                    }

                    std::string roadLinkItem_ElementTypeAttribute;
                    CHECKFALSE(SimulationCommon::ParseAttributeString(roadLinkItemElement, "elementType",
                                                                      roadLinkItem_ElementTypeAttribute));

                    if (0 == roadLinkItem_ElementTypeAttribute.compare("road"))
                    {
                        roadLinkElementType = RoadLinkElementType::Road;
                    }
                    else
                    {
                        LOG_INTERN(LogLevel::Error) << "invalid road link attribute";
                        return false;
                    }

                    CHECKFALSE(SimulationCommon::ParseAttributeString(roadLinkItemElement, "elementId",
                                                                      roadLinkElementId));

                    if (RoadLinkElementType::Road == roadLinkElementType)
                    {
                        std::string roadLinkItem_ContactPointAttribute;
                        CHECKFALSE(SimulationCommon::ParseAttributeString(roadLinkItemElement, "contactPoint",
                                                                          roadLinkItem_ContactPointAttribute));

                        if (0 == roadLinkItem_ContactPointAttribute.compare("start"))
                        {
                            roadLinkContactPoint = ContactPointType::Start;
                        }
                        else if (0 == roadLinkItem_ContactPointAttribute.compare("end"))
                        {
                            roadLinkContactPoint = ContactPointType::End;
                        }
                        else
                        {
                            LOG_INTERN(LogLevel::Error) << "invalid road link attribute";
                            return false;
                        }
                    }
                }
                else if (0 == roadLinkItemElement.tagName().compare("neighbor"))
                {
                    roadLinkType = RoadLinkType::Neighbor;

                    std::string roadLinkItem_SideAttribute;
                    CHECKFALSE(SimulationCommon::ParseAttributeString(roadLinkItemElement, "side",
                                                                      roadLinkItem_SideAttribute));

                    if (0 == roadLinkItem_SideAttribute.compare("left"))
                    {
                        roadLinkSide = RoadLinkSideType::Left;
                    }
                    else if (0 == roadLinkItem_SideAttribute.compare("right"))
                    {
                        roadLinkSide = RoadLinkSideType::Right;
                    }
                    else
                    {
                        LOG_INTERN(LogLevel::Error) << "invalid road link attribute";
                        return false;
                    }

                    CHECKFALSE(SimulationCommon::ParseAttributeString(roadLinkItemElement, "elementId",
                                                                      roadLinkElementId));

                    std::string roadLinkItem_DirectionAttribute;
                    CHECKFALSE(SimulationCommon::ParseAttributeString(roadLinkItemElement, "direction",
                                                                      roadLinkItem_DirectionAttribute));

                    if (0 == roadLinkItem_DirectionAttribute.compare("same"))
                    {
                        roadLinkDirection = RoadLinkDirectionType::Same;
                    }
                    else if (0 == roadLinkItem_DirectionAttribute.compare("opposite"))
                    {
                        roadLinkDirection = RoadLinkDirectionType::Opposite;
                    }
                    else
                    {
                        LOG_INTERN(LogLevel::Error) << "invalid road link attribute";
                        return false;
                    }
                }
                else
                {
                    LOG_INTERN(LogLevel::Warning) << "invalid road link type";
                }

                LOG_INTERN(LogLevel::DebugCore) << "roadlink: " << roadLinkItemElement.tagName().toStdString() <<
                                                " (" << (int)roadLinkElementType << ", " << roadLinkElementId << ", " <<
                                                (int)roadLinkContactPoint << ", " << (int)roadLinkDirection << ", " << (int)roadLinkSide << ")";

                CHECKFALSE(road->AddLink(roadLinkType,
                                         roadLinkElementType,
                                         roadLinkElementId,
                                         roadLinkContactPoint,
                                         roadLinkDirection,
                                         roadLinkSide));

                roadLinkItemElement = roadLinkItemElement.nextSiblingElement(); // any type of road link element
            } // road link item loop
        } // if road link item exists
    } // if road links exist

    return true;
}

bool SceneryImporter::ParseRoadLanes(QDomElement &roadElement,
                                     RoadInterface *road)
{
    QDomElement roadLanesElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(roadElement, "lanes", roadLanesElement));

    // parse lane offsets
    QDomElement laneOffsetElement;
    if (SimulationCommon::GetFirstChildElement(roadLanesElement, "laneOffset", laneOffsetElement))
    {
        while (!laneOffsetElement.isNull())
        {
            double laneOffsetS, laneOffsetA, laneOffsetB, laneOffsetC, laneOffsetD;
            CHECKFALSE(SimulationCommon::ParseAttributeDouble(laneOffsetElement,
                                                              "s",
                                                              laneOffsetS));

            CHECKFALSE(SimulationCommon::ParseAttributeDouble(laneOffsetElement,
                                                              "a",
                                                              laneOffsetA));

            CHECKFALSE(SimulationCommon::ParseAttributeDouble(laneOffsetElement,
                                                              "b",
                                                              laneOffsetB));

            CHECKFALSE(SimulationCommon::ParseAttributeDouble(laneOffsetElement,
                                                              "c",
                                                              laneOffsetC));

            CHECKFALSE(SimulationCommon::ParseAttributeDouble(laneOffsetElement,
                                                              "d",
                                                              laneOffsetD));

            road->AddLaneOffset(laneOffsetS,
                                laneOffsetA,
                                laneOffsetB,
                                laneOffsetC,
                                laneOffsetD);

            laneOffsetElement = laneOffsetElement.nextSiblingElement("laneOffset");
        } // laneOffset loop
    } // if laneOffsets exist

    // parse lane sections
    QDomElement roadLaneSectionElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(roadLanesElement, "laneSection",
                                                      roadLaneSectionElement));
    while (!roadLaneSectionElement.isNull())
    {
        double roadLaneSectionStart;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadLaneSectionElement, "s",
                                                          roadLaneSectionStart));

        // add OpenDrive lane section in ascending order
        RoadLaneSectionInterface *laneSection = road->AddRoadLaneSection(roadLaneSectionStart);
        CHECKFALSE(laneSection);

        LOG_INTERN(LogLevel::DebugCore) << "lane section (s=" << roadLaneSectionStart << ")";

        // left lanes
        QDomElement roadLaneSectionSideElement;

        if (SimulationCommon::GetFirstChildElement(roadLaneSectionElement, "left",
                                                   roadLaneSectionSideElement))
        {
            if (!ParseLanes(roadLaneSectionSideElement, laneSection))
            {
                return false;
            }
        }

        // skip center lanes

        // right lanes
        if (SimulationCommon::GetFirstChildElement(roadLaneSectionElement, "right",
                                                   roadLaneSectionSideElement))
        {
            if (!ParseLanes(roadLaneSectionSideElement, laneSection))
            {
                return false;
            }
        }

        roadLaneSectionElement = roadLaneSectionElement.nextSiblingElement("laneSection");
    } // road lane section loop

    return true;
}

bool SceneryImporter::ParseRoads(QDomElement &documentRoot,
                                 Scenery &scenery)
{
    QDomElement roadElement;
    if (SimulationCommon::GetFirstChildElement(documentRoot, "road", roadElement))
    {
        while (!roadElement.isNull())
        {
            // road id
            std::string id;
            CHECKFALSE(SimulationCommon::ParseAttributeString(roadElement, "id", id));

            RoadInterface *road = scenery.AddRoad(id);
            CHECKFALSE(road);

            LOG_INTERN(LogLevel::DebugCore) << "road: id: " << id;

            if (!ParseGeometries(roadElement, road))
            {
                return false;
            }

            if (!ParseElevationProfile(roadElement, road))
            {
                return false;
            }

            if (!ParseRoadLinks(roadElement, road))
            {
                return false;
            }

            if (!ParseRoadLanes(roadElement, road)) // parsing laneOffset is included here
            {
                return false;
            }

            roadElement = roadElement.nextSiblingElement("road");
        } // road loop
    } // if roads exist

    return true;
}

//-----------------------------------------------------------------------------
//! Imports a scenery from a given file
//!
//!
//! @param[in]  filename            DOM element containing e.g. OPENDrive road
//! @param[out] globalObjects       Target container for the scenery data
//!
//! @return                         False if an error occurred, true otherwise
//-----------------------------------------------------------------------------
bool SceneryImporter::Import(const std::string &filename,
                             WorldInterface *world)
{
    std::locale::global(std::locale("C"));

    Scenery scenery;

    QString absoluteFilePath = "";

    if (QFileInfo(QString::fromStdString(filename)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        absoluteFilePath = baseDir.absoluteFilePath(QString::fromStdString(filename));
        absoluteFilePath = baseDir.cleanPath(absoluteFilePath);
    }
    else
    {
        absoluteFilePath = QString::fromStdString(filename);
    }

    if (!QFileInfo(absoluteFilePath).exists())
    {
        LOG_INTERN(LogLevel::Error) << "Scenery Config does not exist.";
        return false;
    }

    QFile xmlFile(absoluteFilePath); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Warning) << "an error occurred during scenery import";
        return false;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    QString errorMsg;
    int errorLine;
    if (!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml file format of file " << filename;
        LOG_INTERN(LogLevel::Warning) << "in line " << errorLine << " : " << errorMsg.toStdString();
        return false;
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        return false;
    }

    if (documentRoot.tagName().toLower() == "opendrive")
    {
        // parse roads
        ParseRoads(documentRoot, scenery);

        // let the world create its scenery
        return world->CreateScenery(scenery);
    }

    return world->CreateWorldScenery(absoluteFilePath.toStdString());
}

} // namespace SimulationSlave
