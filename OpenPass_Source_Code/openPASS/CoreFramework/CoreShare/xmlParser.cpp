/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

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

bool GetLastChildElement(QDomElement rootElement, const std::string &tag, QDomElement &result)
{
    QDomNode node = rootElement.lastChildElement(QString::fromStdString(tag));
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

bool ParseDoubleVector(QDomElement rootElement, const std::string &tag, std::vector<double> &result)
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
        std::stringstream valueStream(element.text().toStdString());

        double item;
        while(valueStream >> item)
        {
            result.push_back(item);

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

template <typename T>
bool Parse(QDomElement, const std::string&, T&)
{
    throw std::runtime_error("not implemented yet");
}

template <>
bool Parse(QDomElement rootElement, const std::string &tag, std::string & result)
{
    return ParseString(rootElement, tag, result);
}

template <>
bool Parse(QDomElement rootElement, const std::string &tag, double & result)
{
    return ParseDouble(rootElement, tag, result);
}

template <>
bool Parse(QDomElement rootElement, const std::string &tag, std::vector<double> & result)
{
    return ParseDoubleVector(rootElement, tag, result  );
}

template <>
bool Parse(QDomElement rootElement, const std::string &tag, int & result)
{
    return ParseInt(rootElement, tag, result   );
}

template <>
bool Parse(QDomElement rootElement, const std::string &tag, unsigned long & result)
{
    return ParseULong(rootElement, tag, result );
}

template <>
bool Parse(QDomElement rootElement, const std::string &tag, bool & result)
{
    return ParseBool(rootElement, tag, result  );
}


template<>
bool ParseAttribute<int>(QDomElement element, const std::string &attributeName, int& result)
{
   return ParseAttributeInt(element, attributeName, result);
}

template<>
bool ParseAttribute<double>(QDomElement element, const std::string &attributeName, double& result)
{
   return ParseAttributeDouble(element, attributeName, result);
}

template<>
bool ParseAttribute<std::string>(QDomElement element, const std::string &attributeName, std::string& result)
{
   return ParseAttributeString(element, attributeName, result);
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

bool ParseType(const std::string &element, RoadElementOrientation &orientation)
{
    return
            assignIfMatching(element, orientation, "none", RoadElementOrientation::both) ||
            assignIfMatching(element, orientation, "+",    RoadElementOrientation::positive) ||
            assignIfMatching(element, orientation, "-",    RoadElementOrientation::negative);
}

bool ParseType(const std::string &element, RoadObjectType &objectType)
{
    return
            assignIfMatching(element, objectType, "barrier", RoadObjectType::barrier) ||
            assignIfMatching(element, objectType, "obstacle", RoadObjectType::obstacle) ||
            assignIfMatching(element, objectType, "car", RoadObjectType::car) ||
            assignIfMatching(element, objectType, "truck", RoadObjectType::truck) ||
            assignIfMatching(element, objectType, "van", RoadObjectType::van) ||
            assignIfMatching(element, objectType, "bus", RoadObjectType::bus) ||
            assignIfMatching(element, objectType, "trailer", RoadObjectType::trailer) ||
            assignIfMatching(element, objectType, "bike", RoadObjectType::bike) ||
            assignIfMatching(element, objectType, "motorbike", RoadObjectType::motorbike) ||
            assignIfMatching(element, objectType, "tram", RoadObjectType::tram) ||
            assignIfMatching(element, objectType, "train", RoadObjectType::train) ||
            assignIfMatching(element, objectType, "pedestrian", RoadObjectType::pedestrian)||
            assignIfMatching(element, objectType, "pole", RoadObjectType::pole) ||
            assignIfMatching(element, objectType, "tree", RoadObjectType::tree) ||
            assignIfMatching(element, objectType, "vegetation", RoadObjectType::vegetation) ||
            assignIfMatching(element, objectType, "building", RoadObjectType::building) ||
            assignIfMatching(element, objectType, "parkingSpace", RoadObjectType::parkingSpace) ||
            assignIfMatching(element, objectType, "wind", RoadObjectType::wind)  ||
            assignIfMatching(element, objectType, "patch", RoadObjectType::patch) ||
            assignIfMatching(element, objectType, "GuardRail", RoadObjectType::guardRail) ||
            assignIfMatching(element, objectType, "RoadSideMarkerPost", RoadObjectType::roadSideMarkerPost);
}

} // namespace SimulationCommon
