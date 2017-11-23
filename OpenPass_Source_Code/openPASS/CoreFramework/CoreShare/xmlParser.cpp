/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <sstream>
#include "xmlParser.h"

namespace SimulationCommon
{

bool GetFirstChildElement(QDomElement rootElement, const std::string &tag, QDomElement &result)
{
    QDomNode node = rootElement.firstChildElement(QString::fromStdString(tag));
    if(node.isNull())
    {
        return false;
    }

    result = node.toElement();
    if(result.isNull())
    {
        return false;
    }

    return true;
}

bool GetFirstChild(QDomElement rootElement, QDomElement &result)
{
    QDomNode node = rootElement.firstChild();
    if(node.isNull())
    {
        return false;
    }

    result = node.toElement();
    if(result.isNull())
    {
        return false;
    }

    return true;
}

bool ParseString(QDomElement rootElement, const std::string &tag, std::string &result)
{
    QDomNode node = rootElement.firstChildElement(QString::fromStdString(tag));
    if(node.isNull())
    {
        return false;
    }

    QDomElement element = node.toElement();
    if(element.isNull())
    {
        return false;
    }

    result = element.text().toStdString();

    return true;
}

bool ParseCurrentInt(QDomElement currentElement, int &result)
{
    try
    {
        result = std::stoi(currentElement.text().toStdString());
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseDouble(QDomElement rootElement, const std::string &tag, double &result)
{
    QDomNode node = rootElement.firstChildElement(QString::fromStdString(tag));
    if(node.isNull())
    {
        return false;
    }

    QDomElement element = node.toElement();
    if(element.isNull())
    {
        return false;
    }

    try
    {
        result = std::stod(element.text().toStdString());
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseInt(QDomElement rootElement, const std::string &tag, int &result)
{
    QDomNode node = rootElement.firstChildElement(QString::fromStdString(tag));
    if(node.isNull())
    {
        return false;
    }

    QDomElement element = node.toElement();
    if(element.isNull())
    {
        return false;
    }

    try
    {
        result = std::stoi(element.text().toStdString());
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseULong(QDomElement rootElement, const std::string &tag, unsigned long &result)
{
    QDomNode node = rootElement.firstChildElement(QString::fromStdString(tag));
    if(node.isNull())
    {
        return false;
    }

    QDomElement element = node.toElement();
    if(element.isNull())
    {
        return false;
    }

    try
    {
        result = std::stoul(element.text().toStdString());
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseBool(QDomElement rootElement, const std::string &tag, bool &result)
{
    QDomNode node = rootElement.firstChildElement(QString::fromStdString(tag));
    if(node.isNull())
    {
        return false;
    }

    QDomElement element = node.toElement();
    if(element.isNull())
    {
        return false;
    }

    try
    {
        std::string value = element.text().toStdString();
        std::transform(value.begin(),
                       value.end(),
                       value.begin(),
                       ::tolower);
        std::istringstream is(value);
        is >> std::boolalpha >> result;
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseAttributeString(QDomElement element, const std::string &attributeName, std::string &result)
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

    result = attribute.value().toStdString();

    return true;
}

bool ParseAttributeDouble(QDomElement element, const std::string &attributeName, double &result)
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

    try
    {
        result = std::stod(attribute.value().toStdString());
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseAttributeInt(QDomElement element, const std::string &attributeName, int &result)
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

    try
    {
        result = std::stoi(attribute.value().toStdString());
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseAttributeBool(QDomElement element, const std::string &attributeName, bool &result)
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

    try
    {
        std::string value = attribute.value().toStdString();
        std::transform(value.begin(),
                       value.end(),
                       value.begin(),
                       ::tolower);
        std::istringstream is(value);
        is >> std::boolalpha >> result;
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseAttributeDoubleVector(QDomElement element, const std::string &attributeName, std::vector<double> *result)
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

    try
    {
        std::stringstream valueStream(attribute.value().toStdString());

        double item;
        while(valueStream >> item)
        {
            result->push_back(item);

            if(valueStream.peek() == ',')
            {
                valueStream.ignore();
            }
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseAttributeIntVector(QDomElement element, const std::string &attributeName, std::vector<int> *result)
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

    try
    {
        std::stringstream valueStream(attribute.value().toStdString());

        int item;
        while(valueStream >> item)
        {
            result->push_back(item);

            if(valueStream.peek() == ',')
            {
                valueStream.ignore();
            }
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ParseAttributeBoolVector(QDomElement element, const std::string &attributeName, std::vector<bool> *result)
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

    try
    {
        std::stringstream valueStream(attribute.value().toStdString());
        std::string value;

        while(std::getline(valueStream, value, ','))
        {
            bool item;
            std::transform(value.begin(),
                           value.end(),
                           value.begin(),
                           ::tolower);
            std::istringstream is(value);
            is >> std::boolalpha >> item;

            result->push_back(item);
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

} // namespace SimulationCommon