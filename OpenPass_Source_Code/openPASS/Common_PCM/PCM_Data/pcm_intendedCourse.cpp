#include "pcm_intendedCourse.h"

bool PCM_IntendedCourses::AddPCM_Course(PCM_Course course)
{
    courseVec.push_back(course);
    return true;
}

std::vector<PCM_Course> *PCM_IntendedCourses::GetCourseVec()
{
    return &courseVec;
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
