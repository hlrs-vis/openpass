/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_intendedCourse.h"

bool PCM_IntendedCourses::AddPCM_Course(PCM_Course course)
{
    courseVec.push_back(course);
    return true;
}

std::vector<PCM_Course> PCM_IntendedCourses::GetCourseVec()
{
    return courseVec;
}

bool PCM_IntendedCourses::IsCoursePresent(int betNr)
{
    PCM_Course *course = GetCourseByBetNr(betNr);
    if (course != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

PCM_Course *PCM_IntendedCourses::GetCourseByBetNr(int betNr)
{
    auto it = std::find_if(courseVec.begin(), courseVec.end(),
                           [&betNr](PCM_Course & course)
    {
        return course.GetBetNr() == betNr;
    });
    if (it != courseVec.end())
    {
        return &*it;
    }

    return nullptr;
}
