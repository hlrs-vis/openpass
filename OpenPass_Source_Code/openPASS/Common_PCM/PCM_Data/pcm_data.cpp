/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
                                                   double viewDirection,
                                                   double range) const
{
    MarkType minMarkType = MarkType::NONE;
    double minDistance = INFINITY;
    for (int i = 1; i < static_cast<int>(MarkType::NumberOfMarkTypes); i++)
    {
        PCM_LineSegment lineSegment;
        MarkType currentType = static_cast<MarkType>(i);
        lineSegment = GetNearestLineSegmentOfMarks(currentType, point,
                                                   viewDirection, range);

        double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

        if ((minDistance > distance) && (!std::isinf(distance)))
        {
            minDistance = distance;
            minMarkType = currentType;
        }
    }

    return minMarkType;
}

int PCM_Data::GetIdOfNearestAgent(int egoId, const PCM_Point *point, double viewDirection,
                                  double range) const
{
    int id = -1;
    double minDistance = INFINITY;
    for (int i = 0; i < static_cast<int>(agentVec.size()); i++)
    {
        if (i != egoId)
        {
            PCM_LineSegment lineSegment;
            lineSegment = GetNearestLineSegmentOfAgents(egoId, point,
                                                        viewDirection, range);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

            if ((minDistance > distance) && (!std::isinf(distance)))
            {
                minDistance = distance;
                id = i;
            }
        }
    }

    return id;
}

ObjectType PCM_Data::GetObjectTypeOfNearestLineSegment(const PCM_Point *point, double viewDirection,
                                                       double range) const
{
    ObjectType minObjectType = ObjectType::NONE;
    double minDistance = INFINITY;
    for (int i = 1; i < static_cast<int>(ObjectType::NumberOfObjectTypes); i++)
    {
        PCM_LineSegment lineSegment;
        ObjectType currentType = static_cast<ObjectType>(i);
        lineSegment = GetNearestLineSegmentOfObject(currentType, point,
                                                    viewDirection, range);

        double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

        if ((minDistance > distance) && (!std::isinf(distance)))
        {
            minDistance = distance;
            minObjectType = currentType;
        }
    }

    return minObjectType;
}

PCM_LineSegment PCM_Data::GetNearestLineSegmentOfMarks(MarkType markType, const PCM_Point *point,
                                                       double viewDirection, double range) const
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
            PCM_LineSegment lineSegment;
            lineSegment = GetNearestLineSegmentOfMarks(static_cast<MarkType>(i), point,
                                                       viewDirection, range);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

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
        minLineSegment = marks->GetNearestLineSegment(point, viewDirection, range);
    }

    return minLineSegment;
}

PCM_LineSegment PCM_Data::GetNearestLineSegmentOfAgents(int egoId, const PCM_Point *point,
                                                        double viewDirection, double range) const
{
    PCM_LineSegment minLineSegment;

    if (point == nullptr)
    {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    for (int  i = 0; i < static_cast<int>(agentVec.size()); i++)
    {
        if (i != egoId)
        {
            PCM_LineSegment lineSegment;
            lineSegment = agentVec.at(i)->GetNearestLineSegment(point, viewDirection, range);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

            if ((minDistance > distance) && (!std::isinf(distance)))
            {
                minDistance = distance;
                minLineSegment = lineSegment;
            }
        }
    }

    return minLineSegment;
}

PCM_LineSegment PCM_Data::GetNearestLineSegmentOfObject(ObjectType objectType,
                                                        const PCM_Point *point, double viewDirection, double range) const
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
            PCM_LineSegment lineSegment;
            lineSegment = GetNearestLineSegmentOfObject(static_cast<ObjectType>(i), point,
                                                        viewDirection, range);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

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
            minLineSegment = object->GetNearestLineSegment(point, viewDirection, range);
            break;
        case ObjectType::VIEWOBJECT:
            minLineSegment = viewObject->GetNearestLineSegment(point, viewDirection, range);
            break;
        default:
            break;
        }
    }

    return minLineSegment;
}

PCM_Point PCM_Data::GetNearestPointOfMarks(MarkType markType, const PCM_Point *point,
                                           double viewDirection,
                                           double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegmentOfMarks(markType, point, viewDirection,
                                                                  range);
    return minLineSegment.GetNearestPointFromPoint(point, viewDirection, range);
}

PCM_Point PCM_Data::GetNearestPointOfAgents(int egoId, const PCM_Point *point, double viewDirection,
                                            double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegmentOfAgents(egoId, point, viewDirection,
                                                                   range);
    return minLineSegment.GetNearestPointFromPoint(point, viewDirection, range);
}

PCM_Point PCM_Data::GetNearestPointOfObject(ObjectType objectType, const PCM_Point *point,
                                            double viewDirection, double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegmentOfObject(objectType, point, viewDirection,
                                                                   range);
    return minLineSegment.GetNearestPointFromPoint(point, viewDirection, range);
}

const PCM_Object *PCM_Data::GetObject() const
{
    return object;
}

const PCM_ViewObject *PCM_Data::GetViewObject() const
{
    return viewObject;
}
