/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  xmlParser.h
//! @brief This file contains helper functions to parse the configuration.
//-----------------------------------------------------------------------------

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <QFile>
#include <QDomDocument>

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

} // namespace SimulationCommon

#endif // XMLPARSER_H
