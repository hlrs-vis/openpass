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
    foreach (const PCM_Marks *marks, marksVec) {
        if (marks != nullptr) {
            if (marks->GetMarkType() == markType) {
                return marks;
            }
        }
    }
    return nullptr;
}

void PCM_Data::Clear()
{
    foreach (const PCM_Marks *marks, marksVec) {
        if (marks != nullptr) {
            delete marks;
        }
    }
    marksVec.clear();

    if (object != nullptr) {
        delete object;
        object = nullptr;
    }

    if (viewObject != nullptr) {
        delete viewObject;
        viewObject = nullptr;
    }

    foreach (const PCM_Course *course, intendedCourses) {
        if (course != nullptr) {
            delete course;
        }
    }
    intendedCourses.clear();

    if (globalData != nullptr) {
        delete globalData;
        globalData = nullptr;
    }
}

MarkType PCM_Data::GetTypeOfNearestLineSegment(const PCM_Point *point,
                                               double viewDirection,
                                               double range) const
{
    MarkType minMarkType = MarkType::NONE;
    double minDistance = INFINITY;
    for (int i = 1; i < static_cast<int>(MarkType::NumberOfMarkTypes); i++) {
        PCM_LineSegment lineSegment;
        MarkType currentType = static_cast<MarkType>(i);
        lineSegment = GetNearestLineSegment(currentType, point,
                                            viewDirection, range);

        double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

        if ((minDistance > distance) && (!std::isinf(distance))) {
            minDistance = distance;
            minMarkType = currentType;
        }
    }

    return minMarkType;
}

PCM_LineSegment PCM_Data::GetNearestLineSegment(MarkType markType, const PCM_Point *point,
                                                double viewDirection, double range) const
{
    PCM_LineSegment minLineSegment;

    if (point == nullptr) {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    // if no mark type, than find minimal distance for all existing mark types
    if (markType == MarkType::NONE) {
        for (int i = 1; i < static_cast<int>(MarkType::NumberOfMarkTypes); i++) {
            PCM_LineSegment lineSegment;
            lineSegment = GetNearestLineSegment(static_cast<MarkType>(i), point,
                                                viewDirection, range);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

            if ((minDistance > distance) && (!std::isinf(distance))) {
                minDistance = distance;
                minLineSegment = lineSegment;
            }
        }
    } else {
        const PCM_Marks *marks = GetMarksOfType(markType);
        minLineSegment = marks->GetNearestLineSegment(point, viewDirection, range);
    }

    return minLineSegment;
}

PCM_Point PCM_Data::GetNearestPoint(MarkType markType, const PCM_Point *point, double viewDirection,
                                    double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegment(markType, point, viewDirection, range);
    return minLineSegment.GetNearestPointFromPoint(point, viewDirection, range);
}
