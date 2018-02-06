#ifndef PCM_INTENDEDCOURSE_H
#define PCM_INTENDEDCOURSE_H

#include "PCM_Course.h"

class PCM_IntendedCourses
{
public:

    PCM_IntendedCourses() = default;

    virtual ~PCM_IntendedCourses() = default;

    bool AddPCM_Course(PCM_Course course);

    std::vector<PCM_Course> *GetCourseVec();

    bool IsCoursePresent(int betNr);

    PCM_Course *GetCourseByBetNr(int betNr);

private:
    std::vector<PCM_Course> courseVec;      //!< vector of courses
};


#endif // PCM_INTENDEDCOURSE_H
