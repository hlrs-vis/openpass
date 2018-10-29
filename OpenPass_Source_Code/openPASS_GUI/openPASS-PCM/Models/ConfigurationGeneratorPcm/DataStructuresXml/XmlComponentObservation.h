/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLCOMPONENTOBSERVATION_H
#define XMLCOMPONENTOBSERVATION_H

#include "XmlBaseClass.h"

class XmlComponentObservation: public XmlBaseClass
{
public:
    XmlComponentObservation( int id, int oberservationRef );

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    int observationRef;     //!< id of the referenced observation module
};

#endif // XMLCOMPONENTOBSERVATION_H
