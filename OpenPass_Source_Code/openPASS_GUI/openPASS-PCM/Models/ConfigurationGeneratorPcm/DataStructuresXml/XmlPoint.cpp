/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlPoint.h"

XmlPoint::XmlPoint(const PCM_Point *point)
{
    this->point = point;
}

bool XmlPoint::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("point");
    xmlWriter->writeAttribute("id", QString::number(point->GetId(), 'g', 10));
    xmlWriter->writeTextElement("x", QString::number(point->GetX(), 'g', 10));
    xmlWriter->writeTextElement("y", QString::number(point->GetY(), 'g', 10));
    xmlWriter->writeTextElement("z", QString::number(point->GetZ(), 'g', 10));
    xmlWriter->writeEndElement();

    return true;
}
