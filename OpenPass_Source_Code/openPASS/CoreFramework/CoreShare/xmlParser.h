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

//-----------------------------------------------------------------------------
//! @file  XmlParser.h
//! @brief This file contains helper functions to parse the configuration.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <QFile>
#include <QDomDocument>
#include <unordered_map>
#include "log.h"

#include "roadInterface/roadElementTypes.h"

namespace SimulationCommon
{

extern bool GetFirstChildElement(QDomElement rootElement, const std::string &tag, QDomElement &result);

extern bool GetFirstChild(QDomElement rootElement, QDomElement &result);

extern bool GetLastChildElement(QDomElement rootElement, const std::string &tag, QDomElement &result);

extern bool ParseCurrentInt(QDomElement currentElement, int &result);

/// Tempalted wrapper for Parse* functions
template <typename T>
bool Parse(QDomElement rootElement, const std::string &tag, T & result);

extern bool ParseString(QDomElement rootElement, const std::string &tag, std::string &result);

extern bool ParseDouble(QDomElement rootElement, const std::string &tag, double &result);

extern bool ParseDoubleVector(QDomElement rootElement, const std::string &tag, std::vector<double> &result);

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

//template <typename T>
//bool ParseAttributeType(QDomElement element, const std::string &attributeName, T &result);

bool ParseType(const std::string& element, RoadElementOrientation& orientation);
bool ParseType(const std::string& element, RoadObjectType& objectType);

//template <typename T>
//bool assignIfMatching(std::string element, T& enumeration, std::string match, T value);

/*!
 *  \brief Parses the value of an XML attribute into the provided container.
 *
 *  Depending on the datatype of the result parameter, different parsing techniques are applied.
 *
 *  \param[in]  element         XML element holding the attribute
 *  \param[in]  attributeName   Name of the atttribute to parse the value from
 *  \param[out] result          Container holding the result
 *
 *  \return     True on successful parsing, false otherwise.
 */
template <typename T>
bool ParseAttribute(QDomElement element, const std::string &attributeName, T& result);

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
bool assignIfMatching(const std::string& element, T& enumeration, const std::string& match, const T& value)
{
    if(element == match){
        enumeration = value;
        return true;
    }
    return false;
}

/*!
* \brief Imports a probability map
*
* \details This method imports a probability map for some value of type T. T can either be int, double or string
*
*
* @param[in]     parentElement          Element containing the information
* @param[in]     key                    Name how the value is specified in the xml file
* @param[in]     tag                    Name of the tags that should be imported
* @param[out]    probabilities          Map where the probabilities get saved
* @param[in]     mustAddUpToOne         flag which specifies wether the sum of all probalities must be 1
* @return	     true, if successful
*/

template <typename T>
bool ImportProbabilityMap(QDomElement parentElement,
                          const std::string key,
                          const QString tag,
                          std::unordered_map<T, double> &probabilities,
                          bool mustAddUpToOne = true)
{
    double probabilitySum = 0.0;

    QDomElement childElement;
    if(!GetFirstChildElement(parentElement, tag.toStdString(), childElement))
    {
        LOG_INTERN(LogLevel::Error) << "At least one element is required.";
        return false;
    }

    while(!childElement.isNull())
    {
        T keyValue;
        double probability;

        if(!ParseAttribute<T>(childElement, key, keyValue))
        {
            LOG_INTERN(LogLevel::Error) << "Key is invalid.";
            return false;
        }

        if(!ParseAttributeDouble(childElement, "Probability", probability))
        {
            LOG_INTERN(LogLevel::Error) << "Probability is invalid.";
            return false;
        }

        auto insertReturn = probabilities.insert({keyValue, probability});
        if(!insertReturn.second)
        {
            LOG_INTERN(LogLevel::Error) << "Key must be unique.";
            return false;
        }

        probabilitySum += probability;

        childElement = childElement.nextSiblingElement(tag);
    }

    //Checks probabilities
    if(mustAddUpToOne && std::abs(probabilitySum - 1.0) > 1e-6)
    {
        LOG_INTERN(LogLevel::Error) << "Probabilities do not add up to 1.0.";
        return false;
    }

    if(probabilitySum > 1.0 + 1e-6)
    {
        LOG_INTERN(LogLevel::Error) << "Probabilities add up to more than 1.0.";
        return false;
    }

    return true;
}



} // namespace SimulationCommon


