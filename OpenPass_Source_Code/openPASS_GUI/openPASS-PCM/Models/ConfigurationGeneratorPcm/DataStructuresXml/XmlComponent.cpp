/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlComponent.h"

XmlComponent::XmlComponent(int id, XML_COMPONENT_TYPE type, int priority, int offsetTime,
                           int responseTime, int cycleTime, QString library):
    XmlBaseClass(id), type(type), priority(priority), offsetTime(offsetTime),
    responseTime(responseTime), cycleTime(cycleTime), library(library)
{}

void XmlComponent::AddInput(int id, QString name, int channelRef)
{
    componentInputs.push_back( XmlComponentSignal( id, true, name, channelRef ) );
}

void XmlComponent::AddOutput(int id, QString name, int channelRef)
{
    componentOutputs.push_back( XmlComponentSignal( id, false, name, channelRef ) );
}

void XmlComponent::AddParameter(int id, XML_PARAMETER_TYPE type, QString name, QString value)
{
    componentParameters.push_back( XmlParameter( id, type, name, value ) );
}

void XmlComponent::AddObservation(int id, int observationRef)
{
    componentObservations.push_back( XmlComponentObservation( id, observationRef ) );
}

bool XmlComponent::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Component");

    xmlWriter->writeAttribute( "id",       QString::number(_id) );
    xmlWriter->writeAttribute( "init",     (type == XML_COMPONENT_TYPE::init) ? "true" : "false" );
    xmlWriter->writeAttribute( "priority", QString::number(priority) );
    if (type != XML_COMPONENT_TYPE::init)
    {
        xmlWriter->writeAttribute( "offsetTime",   QString::number(offsetTime) );
        xmlWriter->writeAttribute( "responseTime", QString::number(responseTime) );
        xmlWriter->writeAttribute( "cycleTime",    QString::number(cycleTime) );
    }
    xmlWriter->writeAttribute( "library", library );

    xmlWriter->writeStartElement("ComponentInputs");
    for (XmlComponentSignal input : componentInputs)
    {
        if ( !input.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // ComponentInputs

    xmlWriter->writeStartElement("ComponentOutputs");
    for (XmlComponentSignal output : componentOutputs)
    {
        if ( !output.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // ComponentOutputs

    xmlWriter->writeStartElement("ComponentParameters");
    for (XmlParameter parameter : componentParameters)
    {
        if ( !parameter.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); //ComponentParameters

    xmlWriter->writeStartElement("ComponentObservations");
    for (XmlComponentObservation observation : componentObservations)
    {
        if ( !observation.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); //ComponentObservations

    xmlWriter->writeEndElement(); // Component
    return true;
}
