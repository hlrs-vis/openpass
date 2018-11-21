/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLPARAMETER_H
#define XMLPARAMETER_H

#include "XmlBaseClass.h"

class XmlParameter: public XmlBaseClass
{
public:
    XmlParameter(int id, XML_PARAMETER_TYPE type, QString name, QString value);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    XML_PARAMETER_TYPE type = XML_PARAMETER_TYPE::undefined;   //!< parameter type
    QString name;      //!< name of the parameter
    QString value;     //!< value of the parameter
};

#endif // XMLPARAMETER_H
