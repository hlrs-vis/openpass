/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlIntendedCourses.h"

XmlIntendedCourses::XmlIntendedCourses(PCM_IntendedCourses &intendedCourses)
{
    this->intendedCourses = &intendedCourses;
}

bool XmlIntendedCourses::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement(QString::fromStdString(PCM_Helper::GetIntendedCourseDBString()));

    if (intendedCourses != nullptr)
    {
        std::vector<PCM_Course> courseVec = intendedCourses->GetCourseVec();
        for (PCM_Course course : courseVec)
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
