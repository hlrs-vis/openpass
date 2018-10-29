/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_data.h"

PCM_Data::~PCM_Data()
{
    Clear();
}

bool PCM_Data::AddPCM_Marks(const PCM_Marks *marks)
{
    marksVec.push_back(marks);
    return true;
}

bool PCM_Data::SetPCM_Object(const PCM_Object *object)
{
    this->object = object;
    return true;
}

bool PCM_Data::SetPCM_ViewObject(const PCM_ViewObject *viewObject)
{
    this->viewObject = viewObject;
    return true;
}

bool PCM_Data::SetPCM_GlobalData(const PCM_GlobalData *globalData)
{
    this->globalData = globalData;
    return true;
}

bool PCM_Data::SetCallbacks(const CallbackInterface *callbacks)
{
    this->callbacks = callbacks;
    return true;
}

bool PCM_Data::AddPCM_Course(const PCM_Course *course)
{
    intendedCourses.push_back(course);
    return true;
}

std::vector<const PCM_Marks *> *PCM_Data::GetMarksVec()
{
    return &marksVec;
}

const PCM_Marks *PCM_Data::GetMarksOfType(MarkType markType) const
{
    for (const PCM_Marks *marks : marksVec)
    {
        if (marks != nullptr)
        {
            if (marks->GetMarkType() == markType)
            {
                return marks;
            }
        }
    }
    return nullptr;
}

bool PCM_Data::AddPCM_Agent(int id, double xPos, double yPos, double yawAngle, double width,
                            double height)
{
//    LOGINFO(QString().sprintf("To dd PCM_Agent %d with width %.2f and height %.2f", id, width, height).toStdString());

    PCM_Agent *agent = new PCM_Agent(id);
    PCM_Line *line = new PCM_Line(0);

    std::vector<Common::Vector2d> pointVector;
    pointVector.push_back(Common::Vector2d(width / 2, height / 2));
    pointVector.push_back(Common::Vector2d(-width / 2, height / 2));
    pointVector.push_back(Common::Vector2d(-width / 2, -height / 2));
    pointVector.push_back(Common::Vector2d(width / 2, -height / 2));

    int count = 0;
    for (Common::Vector2d point : pointVector)
    {
        point.Rotate(yawAngle);
        point.Translate(xPos, yPos);
        line->AddPCM_Point(new PCM_Point(count++, point.x, point.y));
    }

    agent->AddPCM_Line(line);
    agentVec.push_back(agent);

    return true;
}

void PCM_Data::ClearAgentData()
{
    for (const PCM_Agent *agent : agentVec)
    {
        delete agent;
    }
    agentVec.clear();
}

void PCM_Data::Clear()
{
    for (const PCM_Marks *marks : marksVec)
    {
        if (marks != nullptr)
        {
            delete marks;
        }
    }
    marksVec.clear();

    if (object != nullptr)
    {
        delete object;
        object = nullptr;
    }

    if (viewObject != nullptr)
    {
        delete viewObject;
        viewObject = nullptr;
    }

    for (const PCM_Course *course : intendedCourses)
    {
        if (course != nullptr)
        {
            delete course;
        }
    }
    intendedCourses.clear();

    if (globalData != nullptr)
    {
        delete globalData;
        globalData = nullptr;
    }

    ClearAgentData();
}

MarkType PCM_Data::GetMarkTypeOfNearestLineSegment(const PCM_Point *point,
                                                   double viewAngle,
                                                   double range) const
{
    MarkType minMarkType = MarkType::NONE;
    double minDistance = INFINITY;
    for (int i = 1; i < static_cast<int>(MarkType::NumberOfMarkTypes); i++)
    {
        MarkType currentType = static_cast<MarkType>(i);
        PCM_LineSegment lineSegment = GetNearestLineSegmentOfMarks(currentType, point,
                                                   viewAngle, range);

        double distance = lineSegment.CalcDistanceFromPoint(point, viewAngle, range);

        if ((minDistance > distance) && (!std::isinf(distance)))
        {
            minDistance = distance;
            minMarkType = currentType;
        }
    }

    return minMarkType;
}

AgentDetection PCM_Data::GetNearestAgent(int egoId, const PCM_Point *point,
                                         double viewAngle, double range) const
{
    AgentDetection detection;
    if((egoId<0) || (point == nullptr))
        return detection;

    detection.egoId = egoId;
    detection.egoPoint = point;
    detection.viewAngle = viewAngle;
    detection.range = range;

    for (int i = 0; i < static_cast<int>(agentVec.size()); i++)
    {
        if (i != egoId)
        {
            // find nearest line segment from point to the agent i (agent i must not be ego)
            PCM_LineSegment fullLineSegment = GetNearestLineSegmentOfAgents(i, egoId, point, viewAngle, range, false);
            PCM_LineSegment subLineSegment = fullLineSegment.CalcSubLineSegmentInViewRange(point, viewAngle, range);

//            LOGINFO(QString().sprintf("PCM_Data: GetNearestAgent: ego%d (%.2f, %.2f), (%.2f, %.2f) viewAngle %.2f, range %.2f", egoId,
//                          fullLineSegment.GetFirstPoint().GetX(), fullLineSegment.GetFirstPoint().GetY(),
//                          fullLineSegment.GetSecondPoint().GetX(), fullLineSegment.GetSecondPoint().GetY(), viewAngle, range).toStdString());
//            LOGINFO(QString().sprintf("PCM_Data: GetNearestAgent: ego%d, line length %f", egoId, subLineSegment.CalculateLength()).toStdString());

            double distance = subLineSegment.CalcDistanceFromPoint(point, viewAngle, range);

            if ((!std::isinf(distance)) && (distance < detection.distance))
            {
                detection.oppId = i;
                detection.fullOppLineSegment = fullLineSegment;
                detection.subOppLineSegment = subLineSegment;
                detection.oppPoint = subLineSegment.GetNearestPointFromPoint(point, viewAngle, range);
                detection.distance = distance;

//                if(fullLineSegment.CalculateLength() > subLineSegment.CalculateLength())
//                    LOGINFO(QString().sprintf("GetNearestAgent: subOppLineSegment (%f m) is shorter than fullOppLineSegment (%f m)",
//                                              subLineSegment.CalculateLength(), fullLineSegment.CalculateLength()).toStdString());
            }
        }
    }
    return detection;
}

ObjectType PCM_Data::GetObjectTypeOfNearestLineSegment(const PCM_Point *point, double viewAngle,
                                                       double range) const
{
    ObjectType minObjectType = ObjectType::NONE;
    double minDistance = INFINITY;
    for (int i = 1; i < static_cast<int>(ObjectType::NumberOfObjectTypes); i++)
    {
        ObjectType currentType = static_cast<ObjectType>(i);
        PCM_LineSegment lineSegment = GetNearestLineSegmentOfObject(currentType, point,
                                                    viewAngle, range);

        double distance = lineSegment.CalcDistanceFromPoint(point, viewAngle, range);

        if ((minDistance > distance) && (!std::isinf(distance)))
        {
            minDistance = distance;
            minObjectType = currentType;
        }
    }

    return minObjectType;
}

PCM_LineSegment PCM_Data::GetNearestLineSegmentOfMarks(MarkType markType, const PCM_Point *point,
                                                       double viewAngle, double range) const
{
    PCM_LineSegment minLineSegment;

    if (point == nullptr)
    {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    // if no mark type, than find minimal distance for all existing mark types
    if (markType == MarkType::NONE)
    {
        for (int i = 1; i < static_cast<int>(MarkType::NumberOfMarkTypes); i++)
        {
            PCM_LineSegment lineSegment = GetNearestLineSegmentOfMarks(static_cast<MarkType>(i), point,
                                                       viewAngle, range);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewAngle, range);

            if ((minDistance > distance) && (!std::isinf(distance)))
            {
                minDistance = distance;
                minLineSegment = lineSegment;
            }
        }
    }
    else
    {
        const PCM_Marks *marks = GetMarksOfType(markType);
        minLineSegment = marks->GetNearestLineSegment(point, viewAngle, range);
    }

    return minLineSegment;
}

PCM_LineSegment PCM_Data::GetNearestLineSegmentOfAgents(int agentId, int agentIdExclude, const PCM_Point *point,
                                                        double viewAngle, double range, bool calculateSubLine) const
{
    PCM_LineSegment minLineSegment;

    if (point == nullptr)
    {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    for (int  i = 0; i < static_cast<int>(agentVec.size()); i++)
    {
        if (((agentId >= 0) && (i == agentId))
                || ((agentIdExclude >= 0) && (i != agentIdExclude)) )
        {
            PCM_LineSegment lineSegment = agentVec.at(i)->GetNearestLineSegment(point, viewAngle, range, calculateSubLine);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewAngle, range);

            if ((minDistance > distance) && (!std::isinf(distance)))
            {
                minDistance = distance;
                minLineSegment = lineSegment;
            }

            //debug
//            QString logStr = QString().sprintf("debug: lines of agent %d", i);
//            const std::map<int, PCM_Line *> *lineMap = agentVec.at(i)->GetLineMap();
//            for (std::pair<int, PCM_Line *> pcmLinePair : *lineMap)
//            {
//                PCM_Line *line = pcmLinePair.second;
//                const std::map<int, const PCM_Point *> *pointMap = line->GetPointMap();
//                for (std::pair<int, const PCM_Point *> pcmPointPair : *pointMap)
//                {
//                    const PCM_Point *point = pcmPointPair.second;
//                    logStr += QString().sprintf(" point (%.2f, %.2f)", point->GetX(), point->GetY());
//                }
//            }
//            LOGINFO(logStr.toStdString());
        }
    }

    return minLineSegment;
}

PCM_LineSegment PCM_Data::GetNearestLineSegmentOfObject(ObjectType objectType,
                                                        const PCM_Point *point, double viewAngle, double range) const
{
    PCM_LineSegment minLineSegment;

    if (point == nullptr)
    {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    // if no mark type, than find minimal distance for all existing mark types
    if (objectType == ObjectType::NONE)
    {
        for (int i = 1; i < static_cast<int>(ObjectType::NumberOfObjectTypes); i++)
        {
            PCM_LineSegment lineSegment = GetNearestLineSegmentOfObject(static_cast<ObjectType>(i), point,
                                                        viewAngle, range);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewAngle, range);

            if ((minDistance > distance) && (!std::isinf(distance)))
            {
                minDistance = distance;
                minLineSegment = lineSegment;
            }
        }
    }
    else
    {
        switch (objectType)
        {
        case ObjectType::OBJECT:
            minLineSegment = object->GetNearestLineSegment(point, viewAngle, range);
            break;
        case ObjectType::VIEWOBJECT:
            minLineSegment = viewObject->GetNearestLineSegment(point, viewAngle, range);
            break;
        default:
            break;
        }
    }

    return minLineSegment;
}

PCM_Point PCM_Data::GetNearestPointOfMarks(MarkType markType, const PCM_Point *point,
                                           double viewAngle,
                                           double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegmentOfMarks(markType, point, viewAngle,
                                                                  range);
    return minLineSegment.GetNearestPointFromPoint(point, viewAngle, range);
}

PCM_Point PCM_Data::GetNearestPointOfAgents(int agentId, int agentIdExclude, const PCM_Point *point, double viewAngle,
                                            double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegmentOfAgents(agentId, agentIdExclude, point, viewAngle, range);
    return minLineSegment.GetNearestPointFromPoint(point, viewAngle, range);
}

PCM_Point PCM_Data::GetNearestPointOfObject(ObjectType objectType, const PCM_Point *point,
                                            double viewAngle, double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegmentOfObject(objectType, point, viewAngle,
                                                                   range);
    return minLineSegment.GetNearestPointFromPoint(point, viewAngle, range);
}

const PCM_Object *PCM_Data::GetObject() const
{
    return object;
}

const PCM_ViewObject *PCM_Data::GetViewObject() const
{
    return viewObject;
}
