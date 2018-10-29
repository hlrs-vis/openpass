/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlObservation.h"

XmlObservation::XmlObservation(int id, QString library):
    XmlBaseClass(id), library(library)
{}

void XmlObservation::AddParameter(int id, XML_PARAMETER_TYPE type, QString name, QString value)
{
    observationParameters.push_back( XmlParameter( id, type, name, value ) );
}

bool XmlObservation::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Observation");
    xmlWriter->writeAttribute( "id", QString::number(_id) );
    xmlWriter->writeAttribute( "library", library );

    xmlWriter->writeStartElement("ObservationParameters");
    for (XmlParameter parameter : observationParameters)
    {
        if ( !parameter.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // ObservationParameters

    xmlWriter->writeEndElement(); // Observation

    return true;
}
