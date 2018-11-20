/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
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
#include "roadInterface.h"
#include "scenery.h"
#include "sceneryImporter.h"
#include "utilities.h"
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
        else if (0 == roadLaneTypeStr.compare("bidirectional"))
        {
            roadLaneType = RoadLaneTypeType::Bidirectional;
        }
        else if (0 == roadLaneTypeStr.compare("median"))
        {
            roadLaneType = RoadLaneTypeType::Median;
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
        else if (0 == roadLaneTypeStr.compare("roadworks"))
        {
            roadLaneType = RoadLaneTypeType::Roadworks;
        }
        else if (0 == roadLaneTypeStr.compare("tram"))
        {
            roadLaneType = RoadLaneTypeType::Tram;
        }
        else if (0 == roadLaneTypeStr.compare("rail"))
        {
            roadLaneType = RoadLaneTypeType::Rail;
        }
        else if (0 == roadLaneTypeStr.compare("entry"))
        {
            roadLaneType = RoadLaneTypeType::Entry;
        }
        else if (0 == roadLaneTypeStr.compare("exit"))
        {
            roadLaneType = RoadLaneTypeType::Exit;
        }
        else if (0 == roadLaneTypeStr.compare("offRamp"))
        {
            roadLaneType = RoadLaneTypeType::OffRamp;
        }
        else if (0 == roadLaneTypeStr.compare("onRamp"))
        {
            roadLaneType = RoadLaneTypeType::OnRamp;
        }
        else
        {
            LOG_INTERN(LogLevel::Error) << "invalid road lane type";
            return false;
        }

        RoadLaneInterface *roadLane = laneSection->AddRoadLane(roadLaneId,
                                                               roadLaneType);
        CHECKFALSE(roadLane);
        LOG_INTERN(LogLevel::DebugCore) << "lane: type " << static_cast<int>(roadLaneType) << ", id=" << roadLaneId;

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

        ParseLaneRoadMark(rootElement.nodeName().toStdString(), roadLaneElement, roadLane);

        roadLaneElement = roadLaneElement.nextSiblingElement("lane");
    } // lane loop

    return true;
}

bool SceneryImporter::ParseLaneRoadMark(std::string leftCenterRight, QDomElement &roadLaneElement, RoadLaneInterface* roadLane)
{

    RoadLaneRoadDescriptionType descType = RoadLaneRoadDescriptionType::Center;


    if(0 == leftCenterRight.compare("left"))
    {
        descType = RoadLaneRoadDescriptionType::Left;
    }
    else if(0 == leftCenterRight.compare("right"))
    {
        descType = RoadLaneRoadDescriptionType::Right;
    }



    QDomElement roadLaneRoadMarkElement;
    CHECKFALSE(SimulationCommon::GetFirstChildElement(roadLaneElement, "roadMark", roadLaneRoadMarkElement));
    double roadLaneSOffset;
    CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadLaneRoadMarkElement, "sOffset", roadLaneSOffset));
    std::string roadLaneRoadMarkStr;
    CHECKFALSE(SimulationCommon::ParseAttributeString(roadLaneRoadMarkElement, "type", roadLaneRoadMarkStr));

    while(!roadLaneRoadMarkElement.isNull())
    {
    RoadLaneRoadMarkType roadLaneRoadMark = RoadLaneRoadMarkType::Undefined;

    if(0 == roadLaneRoadMarkStr.compare("none"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::None;
    }
    else if(0 == roadLaneRoadMarkStr.compare("solid"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Solid;
    }
    else if(0 == roadLaneRoadMarkStr.compare("broken"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Broken;
    }
    else if(0 == roadLaneRoadMarkStr.compare("solid solid"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Solid_Solid;
    }
    else if(0 == roadLaneRoadMarkStr.compare("solid broken"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Solid_Broken;
    }
    else if(0 == roadLaneRoadMarkStr.compare("broken solid"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Broken_Solid;
    }
    else if(0 == roadLaneRoadMarkStr.compare("broken broken"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Broken_Broken;
    }
    else if(0 == roadLaneRoadMarkStr.compare("botts dots"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Botts_Dots;
    }
    else if(0 == roadLaneRoadMarkStr.compare("grass"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Grass;
    }
    else if(0 == roadLaneRoadMarkStr.compare("curb"))
    {
        roadLaneRoadMark = RoadLaneRoadMarkType::Curb;
    }

    RoadLaneRoadMarkColor color = RoadLaneRoadMarkColor::Standard;
    RoadLaneRoadMarkLaneChange roadChange = RoadLaneRoadMarkLaneChange::Undefined;

    roadLane->AddRoadMark(roadLaneSOffset, descType, roadLaneRoadMark, color, roadChange);

    roadLaneRoadMarkElement = roadLaneRoadMarkElement.nextSiblingElement("roadMark");
    }

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
        roadGeometryS = Common::roundDoubleWithDecimals(roadGeometryS, 3);

        double roadGeometryX;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "x", roadGeometryX));
        roadGeometryX = Common::roundDoubleWithDecimals(roadGeometryX, 3);

        double roadGeometryY;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "y", roadGeometryY));
        roadGeometryY = Common::roundDoubleWithDecimals(roadGeometryY, 3);

        double roadGeometryHdg;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "hdg",
                                                          roadGeometryHdg));
        roadGeometryHdg = Common::roundDoubleWithDecimals(roadGeometryHdg, 6);

        double roadGeometryLength;
        CHECKFALSE(SimulationCommon::ParseAttributeDouble(roadGeometryHeaderElement, "length",
                                                          roadGeometryLength));
        roadGeometryLength = Common::roundDoubleWithDecimals(roadGeometryLength, 3);

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

            if (roadGeometryCurvStart < 1e-7 && roadGeometryCurvStart > -1e-7)
            {
                roadGeometryCurvStart = 0;
            }

            if (roadGeometryCurvEnd < 1e-7 && roadGeometryCurvEnd > -1e-7)
            {
                roadGeometryCurvEnd = 0;
            }

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

bool SceneryImporter::checkRoadSignalBoundaries(RoadSignalSpecification signal)
{
    return
    signal.s >= 0 &&
   (signal.dynamic == "yes" || signal.dynamic == "no") &&
   (signal.orientation == "+" || signal.orientation == "-" || signal.orientation == "none") &&
   !signal.country.empty() &&
    signal.height >= 0 &&
    signal.width >= 0;
}

bool SceneryImporter::ParseSignals(QDomElement &roadElement,
                                   RoadInterface *road)
{
    using namespace SimulationCommon;

    QDomElement signalsElement;
    if(GetFirstChildElement(roadElement, "signals", signalsElement))
    {
        QDomElement signalElement;
        if(GetFirstChildElement(signalsElement, "signal", signalElement))
        {
            while(!signalElement.isNull())
            {
                RoadSignalSpecification signal;

                CHECKFALSE(ParseAttributeDouble(signalElement, "s",           signal.s));
                CHECKFALSE(ParseAttributeDouble(signalElement, "t",           signal.t));
                CHECKFALSE(ParseAttributeDouble(signalElement, "zOffset",     signal.zOffset));
                CHECKFALSE(ParseAttributeDouble(signalElement, "value",       signal.value));
                CHECKFALSE(ParseAttributeDouble(signalElement, "hOffset",     signal.hOffset));
                CHECKFALSE(ParseAttributeDouble(signalElement, "pitch",       signal.pitch));
                CHECKFALSE(ParseAttributeString(signalElement, "id",          signal.id));
                CHECKFALSE(ParseAttributeString(signalElement, "name",        signal.name));
                CHECKFALSE(ParseAttributeString(signalElement, "dynamic",     signal.dynamic));
                CHECKFALSE(ParseAttributeString(signalElement, "orientation", signal.orientation));
                CHECKFALSE(ParseAttributeString(signalElement, "country",     signal.country));

                std::string signalType;
                CHECKFALSE(ParseAttributeString(signalElement, "type",        signalType));
                CHECKFALSE(ParseSignalType(signalType, signal.type));

                CHECKFALSE(ParseAttributeString(signalElement, "subtype",     signal.subtype));

                // optional
                std::string signalUnit;
                ParseAttributeString(signalElement, "unit", signalUnit);
                CHECKFALSE(ParseSignalUnit(signalUnit, signal.unit));

                // not generated by ODDlot
                // potentially optional - no clue from the standard
                ParseAttributeDouble(signalElement, "height", signal.height);
                ParseAttributeDouble(signalElement, "width",  signal.width);
                ParseAttributeString(signalElement, "text",   signal.text);

                // check validity subtag
                CHECKFALSE(ParseElementValidity(signalElement, signal.validity));

                // check other parameters
                CHECKFALSE(checkRoadSignalBoundaries(signal));

                road->AddRoadSignal(signal);

                signalElement = signalElement.nextSiblingElement("signal");
            }
        }
    }
    return true;
}

bool SceneryImporter::ParseObjects(QDomElement &roadElement, RoadInterface *road)
{
    using namespace SimulationCommon;

    QDomElement objectsElement;
    if(GetFirstChildElement(roadElement, "objects", objectsElement))
    {
        QDomElement objectElement;
        if(GetFirstChildElement(objectsElement, "object", objectElement))
        {
            while(!objectElement.isNull())
            {
                CHECKFALSE(ParseObject(objectElement, road));

                objectElement = objectElement.nextSiblingElement("object");
            }
        }
    }
    return true;
}

bool SceneryImporter::ParseObject(QDomElement &objectElement, RoadInterface *road)
{
    using namespace SimulationCommon;

    RoadObjectSpecification object;

    CHECKFALSE(ParseAttributeType(objectElement, "type", object.type));
    CHECKFALSE(ParseAttributeString(objectElement, "name", object.name));
    CHECKFALSE(ParseAttributeString(objectElement, "id", object.id));
    CHECKFALSE(ParseAttributeDouble(objectElement, "s", object.s));
    CHECKFALSE(ParseAttributeDouble(objectElement, "t", object.t));
    CHECKFALSE(ParseAttributeDouble(objectElement, "zOffset", object.zOffset));
    CHECKFALSE(ParseAttributeDouble(objectElement, "validLength", object.validLength));
    CHECKFALSE(ParseAttributeType(objectElement, "orientation", object.orientation));
    CHECKFALSE(ParseAttributeDouble(objectElement, "width", object.width));
    CHECKFALSE(ParseAttributeDouble(objectElement, "length", object.length));
    CHECKFALSE(ParseAttributeDouble(objectElement, "height", object.height));
    CHECKFALSE(ParseAttributeDouble(objectElement, "hdg", object.hdg));
    CHECKFALSE(ParseAttributeDouble(objectElement, "pitch", object.pitch));
    CHECKFALSE(ParseAttributeDouble(objectElement, "roll", object.roll));

    if (ParseAttributeDouble(objectElement, "radius", object.radius) && object.radius != 0) {
        LOG_INTERN(LogLevel::Warning) << "object radius will be ignored";
    }
    object.radius = 0;

    CHECKFALSE(object.checkLimits());
    CHECKFALSE(ParseElementValidity(objectElement, object.validity));

    std::list<RoadObjectSpecification> parsedObjectRepetitions = ParseObjectRepeat(objectElement, object);
    AddParsedObjectsToRoad(parsedObjectRepetitions, road);

    return true;
 }

void SceneryImporter::AddParsedObjectsToRoad(std::list<RoadObjectSpecification> parsedObjects, RoadInterface *road)
{
    for(auto object : parsedObjects)
    {
      road->AddRoadObject(object);
    }
}

std::list<RoadObjectSpecification> SceneryImporter::ParseObjectRepeat(QDomElement &objectElement, const RoadObjectSpecification &object)
{
    using namespace SimulationCommon;

    std::list<RoadObjectSpecification> objectRepetitions;

    QDomElement repeatElement;
    if(GetFirstChildElement(objectElement, "repeat", repeatElement))
    {
        while(!repeatElement.isNull())
        {
            ParseRepeat(repeatElement, object, objectRepetitions);
            repeatElement = repeatElement.nextSiblingElement("repeat");
        }
    }
    else
    {
        objectRepetitions.push_back(object);
    }
    return objectRepetitions;
}

void SceneryImporter::ParseOptionalInterval(QDomElement &repeatElement, std::string startAttribute, std::string endAttribute, OptionalInterval &interval)
{
    bool checkStartIsSet = SimulationCommon::ParseAttributeDouble(repeatElement, startAttribute, interval.start);
    bool checkEndIsSet = SimulationCommon::ParseAttributeDouble(repeatElement, endAttribute, interval.end);

    if(checkStartIsSet && checkEndIsSet)
    {
        interval.isSet = true;
        return;
    }
    if(!checkStartIsSet && !checkEndIsSet)
    {
        return;
    }
    else
    {
        LOG_INTERN(LogLevel::Error) << "an error occured during scenery import";
        throw std::runtime_error("Missing intervall parameter in scenery import");
    }
}

bool SceneryImporter::ParseRepeat(QDomElement &repeatElement, RoadObjectSpecification object, std::list<RoadObjectSpecification> &objectRepitions)
{
    using namespace SimulationCommon;
    ObjectRepeat repeat;

    CHECKFALSE(ParseAttributeDouble(repeatElement, "s",            repeat.s));
    CHECKFALSE(ParseAttributeDouble(repeatElement, "length",       repeat.length));
    CHECKFALSE(ParseAttributeDouble(repeatElement, "distance",     repeat.distance));

    ParseOptionalInterval(repeatElement, "tStart", "tEnd", repeat.t);
    ParseOptionalInterval(repeatElement, "widthStart", "widthEnd", repeat.width);
    ParseOptionalInterval(repeatElement, "heightStart", "heightEnd", repeat.height);
    ParseOptionalInterval(repeatElement, "zOffsetStart", "zOffsetEnd", repeat.zOffset);

    CHECKFALSE(repeat.checkLimits());

    return ApplyRepeat(repeat, object, objectRepitions);
}

bool SceneryImporter::ApplyRepeat(ObjectRepeat repeat, RoadObjectSpecification object, std::list<RoadObjectSpecification> &objectRepitions)
{
    if(repeat.distance == 0)
    {
        repeat.distance = object.length;
    }

    int objectCount = int(repeat.length / repeat.distance);

    std::vector<double> interpolatedT, interpolatedHeight, interpolatedWidth, interpolatedZOffset;

    if(repeat.t.isSet)      interpolatedT = Common::LinearInterpolation::InterpolateLinear(repeat.t.start,  repeat.t.end, objectCount);
    if(repeat.height.isSet) interpolatedHeight = Common::LinearInterpolation::InterpolateLinear(repeat.height.start, repeat.height.end, objectCount);
    if(repeat.width.isSet)  interpolatedWidth = Common::LinearInterpolation::InterpolateLinear(repeat.width.start, repeat.width.end, objectCount);
    if(repeat.zOffset.isSet) interpolatedZOffset = Common::LinearInterpolation::InterpolateLinear(repeat.zOffset.start, repeat.zOffset.end, objectCount);

    for(int i = 0; i < objectCount; i++)
    {
        RoadObjectSpecification repeatingObject = object;
        repeatingObject.s = repeat.s + (i * repeat.distance);

        if(repeat.t.isSet) repeatingObject.t = interpolatedT.at(i);
        if(repeat.height.isSet)repeatingObject.height = interpolatedHeight.at(i);
        if(repeat.width.isSet)repeatingObject.width = interpolatedWidth.at(i);
        if(repeat.zOffset.isSet)repeatingObject.zOffset = interpolatedZOffset.at(i);

        CHECKFALSE(repeatingObject.checkLimits());
        objectRepitions.push_back(repeatingObject);
    }

    return true;
}

bool SceneryImporter::ParseElementValidity(const QDomElement &rootElement, RoadElementValidity &validity)
{
    using namespace SimulationCommon;

    QDomElement validityElement;
    if(GetFirstChildElement(rootElement, "validity", validityElement))
    {
        int fromLane;
        int toLane;

        CHECKFALSE(ParseAttributeInt(validityElement, "fromLane", fromLane));
        CHECKFALSE(ParseAttributeInt(validityElement, "toLane",   toLane));

        if(fromLane > toLane){
            std::swap(fromLane, toLane);
        }

        for(int laneId = fromLane; laneId <= toLane; laneId++)
        {
            validity.lanes.push_back(laneId);
        }
    }
    else
    {
        validity.all = true;
    }

    return true;
}

bool SceneryImporter::ParseRoadTypes(QDomElement &roadElement, RoadInterface *road)
{
    using namespace SimulationCommon;


        QDomElement typeElement;
        if(GetFirstChildElement(roadElement, "type", typeElement))
        {
            while(!typeElement.isNull())
            {
                RoadTypeSpecification roadType;
                CHECKFALSE(ParseAttributeDouble(typeElement, "s",           roadType.s));
                std::string roadTypeStr;
                CHECKFALSE(ParseAttributeString(typeElement, "type",           roadTypeStr));
                RoadTypeInformation roadTypeInfo = RoadTypeInformation::Undefined;
                if(0 == roadTypeStr.compare("unknown"))
                {
                    roadTypeInfo = RoadTypeInformation::Unknown;
                }
                else if(0 == roadTypeStr.compare("rural"))
                {
                    roadTypeInfo = RoadTypeInformation::Rural;
                }
                else if(0 == roadTypeStr.compare("town"))
                {
                    roadTypeInfo = RoadTypeInformation::Town;
                }
                else if(0 == roadTypeStr.compare("motorway"))
                {
                    roadTypeInfo = RoadTypeInformation::Motorway;
                }
                else if(0 == roadTypeStr.compare("lowSpeed"))
                {
                    roadTypeInfo = RoadTypeInformation::LowSpeed;
                }
                else if(0 == roadTypeStr.compare("pedestrian"))
                {
                    roadTypeInfo = RoadTypeInformation::Pedestrian;
                }
                else if(0 == roadTypeStr.compare("bicycle"))
                {
                    roadTypeInfo = RoadTypeInformation::Bicycle;
                }

                roadType.roadType=roadTypeInfo;


                road->AddRoadType(roadType);

                typeElement = typeElement.nextSiblingElement("type");
            }
        }

    return true;
}

bool SceneryImporter::ParseSignalType(std::string element, RoadSignalType& signalType)
{
    if(element == "-1" || element == "none") {
        signalType = RoadSignalType::Undefined;
        return true;
    }

    try {
        signalType = static_cast<RoadSignalType>(std::stoi(element));
    }
    catch(...) {
        return false;
    }
    return true;
}

bool SceneryImporter::ParseSignalUnit(std::string element, RoadSignalUnit& signalUnit)
{
    // empty is a valid state!
    if(element.empty()) {
        signalUnit = RoadSignalUnit::Undefined;
        return true;
    }

    if(element == "m") {
        signalUnit = RoadSignalUnit::Meter;
        return true;
    }
    if(element == "km") {
        signalUnit = RoadSignalUnit::Kilogram;
        return true;
    }
    if(element == "ft") {
        signalUnit = RoadSignalUnit::Feet;
        return true;
    }
    if(element == "mile") {
        signalUnit = RoadSignalUnit::LandMile;
        return true;
    }
    if(element == "m/s") {
        signalUnit = RoadSignalUnit::MetersPerSecond;
        return true;
    }
    if(element == "km/h") {
        signalUnit = RoadSignalUnit::KilometersPerHour;
        return true;
    }
    if(element == "mph") {
        signalUnit = RoadSignalUnit::MilesPerHour;
        return true;
    }
    if(element == "kg") {
        signalUnit = RoadSignalUnit::Kilogram;
        return true;
    }
    if(element == "t") {
        signalUnit = RoadSignalUnit::MetricTons;
        return true;
    }
    if(element == "%") {
        signalUnit = RoadSignalUnit::Percent;
        return true;
    }

    return false;
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

            if (!ParseObjects(roadElement, road))
            {
                return false;
            }

            if (!ParseSignals(roadElement, road))
            {
                return false;
            }

            if (!ParseRoadTypes(roadElement, road))
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
