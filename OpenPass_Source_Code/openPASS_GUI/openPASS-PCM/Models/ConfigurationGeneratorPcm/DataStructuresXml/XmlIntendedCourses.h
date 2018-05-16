#ifndef XMLINTENDEDCOURSES_H
#define XMLINTENDEDCOURSES_H

#include "pcm_intendedCourse.h"
#include "XmlPoint.h"

class XmlIntendedCourses: public XmlBaseClass
{
public:
    XmlIntendedCourses() = default;
    XmlIntendedCourses(PCM_IntendedCourses *intendedCourses);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_IntendedCourses *intendedCourses = nullptr;              //!< PCM intendedCourses
};


#endif // XMLINTENDEDCOURSES_H
