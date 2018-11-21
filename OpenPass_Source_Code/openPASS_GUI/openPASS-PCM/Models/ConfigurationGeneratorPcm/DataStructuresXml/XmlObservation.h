/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLOBSERVATION_H
#define XMLOBSERVATION_H

#include "XmlParameter.h"

class XmlObservation: public XmlBaseClass
{
public:
    XmlObservation( int id, QString library );

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

    void AddParameter( int id, XML_PARAMETER_TYPE type, QString name, QString value );

private:
    QString library;               //!< library of the observation module
    std::vector<XmlParameter>
    observationParameters;    //!< vector of all parameters of the observation module
};

#endif // XMLOBSERVATION_H
