/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlParameter.h"

XmlParameter::XmlParameter( int id, XML_PARAMETER_TYPE type, QString name, QString value):
    XmlBaseClass(id), type(type), name(name), value(value)
{}

bool XmlParameter::WriteToXml( QXmlStreamWriter *xmlWriter )
{
    switch (type)
    {
    case XML_PARAMETER_TYPE::bool_:
        xmlWriter->writeStartElement("Bool");
        break;
    case XML_PARAMETER_TYPE::int_:
        xmlWriter->writeStartElement("Int");
        break;
    case XML_PARAMETER_TYPE::double_:
        xmlWriter->writeStartElement("Double");
        break;
    case XML_PARAMETER_TYPE::string_:
        xmlWriter->writeStartElement("String");
        break;
    case XML_PARAMETER_TYPE::boolVector:
        xmlWriter->writeStartElement("BoolVector");
        break;
    case XML_PARAMETER_TYPE::intVector:
        xmlWriter->writeStartElement("IntVector");
        break;
    case XML_PARAMETER_TYPE::doubleVector:
        xmlWriter->writeStartElement("DoubleVector");
        break;
    default:
        return false;
    }
    xmlWriter->writeAttribute("id", QString::number(_id));
    xmlWriter->writeAttribute("name", name);
    xmlWriter->writeAttribute("value", value);

    xmlWriter->writeEndElement();

    return true;
}
