/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlAgentEquipment.h"

XmlAgentEquipment::XmlAgentEquipment(int id, int priority, int nChannels):
    XmlBaseClass(id), priority(priority)
{
    for (int iChannel = 0; iChannel < nChannels; iChannel++)
    {
        channelIds.push_back(iChannel);
    }
}

XmlAgentEquipment::XmlAgentEquipment(int id, int priority, std::vector<int> channelIds):
    XmlBaseClass(id), priority(priority), channelIds(channelIds)
{}

XmlAgentEquipment::~XmlAgentEquipment()
{
    for (XmlComponent *component : components)
    {
        delete component;
    }
}

void XmlAgentEquipment::AddComponent(XmlComponent *component)
{
    components.push_back(component);
}

bool XmlAgentEquipment::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Agent");

    xmlWriter->writeAttribute( "id", QString::number(_id) );
    xmlWriter->writeAttribute( "priority", QString::number(priority) );

    xmlWriter->writeStartElement("Channels");
    for ( int channelId : channelIds)
    {
        xmlWriter->writeStartElement("Channel");
        xmlWriter->writeAttribute( "id", QString::number(channelId) );
        xmlWriter->writeEndElement(); // Channel
    }
    xmlWriter->writeEndElement(); // Channels

    xmlWriter->writeStartElement("Components");
    for (XmlComponent *component : components)
    {
        if ( !component->WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // Components

    xmlWriter->writeEndElement(); // Agent

    return true;
}

