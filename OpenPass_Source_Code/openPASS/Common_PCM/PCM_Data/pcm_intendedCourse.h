/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PCM_INTENDEDCOURSE_H
#define PCM_INTENDEDCOURSE_H

#include <vector>

#include "pcm_course.h"

class PCM_IntendedCourses
{
public:

    PCM_IntendedCourses() = default;

    virtual ~PCM_IntendedCourses() = default;

    bool AddPCM_Course(PCM_Course course);

    std::vector<PCM_Course> GetCourseVec();

    bool IsCoursePresent(int betNr);

    PCM_Course *GetCourseByBetNr(int betNr);

private:
    std::vector<PCM_Course> courseVec;      //!< vector of courses
};


#endif // PCM_INTENDEDCOURSE_H
