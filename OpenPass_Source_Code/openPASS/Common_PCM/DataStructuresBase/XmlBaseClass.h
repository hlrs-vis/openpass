/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLBASECLASS_H
#define XMLBASECLASS_H

#include <QXmlStreamWriter>

//-----------------------------------------------------------------------------
//! Enumeration class for the parameter types
//-----------------------------------------------------------------------------
enum class XML_PARAMETER_TYPE
{
    undefined = 0,
    bool_,
    int_,
    double_,
    string_,
    boolVector,
    intVector,
    doubleVector
};

//-----------------------------------------------------------------------------
//! Enumeration class for the component types
//-----------------------------------------------------------------------------
enum class XML_COMPONENT_TYPE
{
    undefined = 0,
    init,
    dynamics,
    other
};

//-----------------------------------------------------------------------------
//! Enumeration class for the agent types
//-----------------------------------------------------------------------------
enum class XML_AGENT_TYPE
{
    undefined = 0,
    pedestrian,
    car,
    other
};


class XmlBaseClass
{
public:
    XmlBaseClass() = default;
    XmlBaseClass(int id);

    virtual ~XmlBaseClass() {}

    virtual bool WriteToXml( QXmlStreamWriter *xmlWriter ) = 0;

protected:
    int _id = -1;       //!< the id of the xml-entry
};

#endif // XMLBASECLASS_H
