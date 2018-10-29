/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlComponentSignal.h"

XmlComponentSignal::XmlComponentSignal(int id, bool isInput, QString name, int channelRef):
    XmlBaseClass(id), isInput(isInput), name(name), channelRef(channelRef)
{}

bool XmlComponentSignal::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    if (isInput)
    {
        xmlWriter->writeStartElement("ComponentInput");
    }
    else
    {
        xmlWriter->writeStartElement("ComponentOutput");
    }
    xmlWriter->writeAttribute("id", QString::number(_id));
    xmlWriter->writeAttribute("name", name);
    xmlWriter->writeAttribute("channelRef", QString::number(channelRef));

    xmlWriter->writeEndElement();

    return true;
}
