/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlLine.h"

XmlLine::XmlLine(const PCM_Line *line)
{
    this->line = line;
}

bool XmlLine::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("line");
    xmlWriter->writeAttribute("id", QString::number(line->GetId()));
    const std::map<int, const PCM_Point *> *pointMap = line->GetPointMap();
    for (const std::pair<const int, const PCM_Point *> pointPair : *pointMap)
    {
        const PCM_Point *point = pointPair.second;
        XmlPoint xmlPoint(point);
        xmlPoint.WriteToXml(xmlWriter);
    }

    xmlWriter->writeEndElement();

    return true;
}
