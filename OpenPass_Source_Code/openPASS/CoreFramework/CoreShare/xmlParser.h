/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  xmlParser.h
//! @brief This file contains helper functions to parse the configuration.
//-----------------------------------------------------------------------------

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <QFile>
#include <QDomDocument>

#include "roadInterface/roadElementTypes.h"

namespace SimulationCommon
{

extern bool GetFirstChildElement(QDomElement rootElement, const std::string &tag, QDomElement &result);

extern bool GetFirstChild(QDomElement rootElement, QDomElement &result);

extern bool ParseCurrentInt(QDomElement currentElement, int &result);

extern bool ParseString(QDomElement rootElement, const std::string &tag, std::string &result);

extern bool ParseDouble(QDomElement rootElement, const std::string &tag, double &result);

extern bool ParseInt(QDomElement rootElement, const std::string &tag, int &result);

extern bool ParseULong(QDomElement rootElement, const std::string &tag, unsigned long &result);

extern bool ParseBool(QDomElement rootElement, const std::string &tag, bool &result);

extern bool ParseAttributeString(QDomElement element, const std::string &attributeName, std::string &result);

extern bool ParseAttributeDouble(QDomElement element, const std::string &attributeName, double &result);

extern bool ParseAttributeInt(QDomElement element, const std::string &attributeName, int &result);

extern bool ParseAttributeBool(QDomElement element, const std::string &attributeName, bool &result);

extern bool ParseAttributeDoubleVector(QDomElement element, const std::string &attributeName, std::vector<double>* result);

extern bool ParseAttributeIntVector(QDomElement element, const std::string &attributeName, std::vector<int>* result);

extern bool ParseAttributeBoolVector(QDomElement element, const std::string &attributeName, std::vector<bool>* result);

bool ParseType(const std::string& element, RoadElementOrientation& orientation);
bool ParseType(const std::string& element, RoadObjectType& objectType);

template <typename T>
bool ParseAttributeType(QDomElement element, const std::string &attributeName, T &result)
{
    if(!element.hasAttribute(QString::fromStdString(attributeName)))
    {
        return false;
    }

    QDomAttr attribute = element.attributeNode(QString::fromStdString(attributeName));
    if(attribute.isNull())
    {
        return false;
    }

    return ParseType(attribute.value().toStdString(), result);
}

template <typename T>
bool assignIfMatching(std::string element, T& enumeration, std::string match, T value)
{
    if(element == match){
        enumeration = value;
        return true;
    }
    return false;
}

} // namespace SimulationCommon

#endif // XMLPARSER_H
