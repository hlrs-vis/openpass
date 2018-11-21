/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlComponentObservation.h"

XmlComponentObservation::XmlComponentObservation( int id, int observationRef ):
    XmlBaseClass(id), observationRef(observationRef)
{}

bool XmlComponentObservation::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("ComponentObservation");

    xmlWriter->writeAttribute("id", QString::number(_id));
    xmlWriter->writeAttribute("observationRef", QString::number(observationRef));

    xmlWriter->writeEndElement();

    return true;
}
