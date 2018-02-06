#include "XmlIntendedCourses.h"

XmlIntendedCourses::XmlIntendedCourses(PCM_IntendedCourses *intendedCourses)
{
    this->intendedCourses = intendedCourses;
}

bool XmlIntendedCourses::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement(QString::fromStdString(PCM_Helper::GetIntendedCourseDBString()));

    if (intendedCourses != nullptr)
    {
        std::vector<PCM_Course> *courseVec = intendedCourses->GetCourseVec();
        for (PCM_Course course : *courseVec)
        {
            xmlWriter->writeStartElement("course");
            xmlWriter->writeAttribute("betNr", QString::number(course.GetBetNr()));
            const std::map<int, const PCM_Point *> *pointMap = course.GetPointMap();
            for (const std::pair<const int, const PCM_Point *> pointPair : *pointMap)
            {
                const PCM_Point *point = pointPair.second;
                XmlPoint xmlPoint(point);
                xmlPoint.WriteToXml(xmlWriter);
            }

            xmlWriter->writeEndElement();
        }
    }

    xmlWriter->writeEndElement();

    return true;
}
