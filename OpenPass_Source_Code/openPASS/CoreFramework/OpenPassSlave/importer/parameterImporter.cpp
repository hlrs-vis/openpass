/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "parameterImporter.h"
#include "CoreFramework/CoreShare/log.h"

using namespace SimulationCommon;

bool ParameterImporter::ImportBoolParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement boolParameterElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "Bool", boolParameterElement))
    {
        //Iterates over all int parameters
        while (!boolParameterElement.isNull())
        {
            std::string boolParameterName;
            bool boolParameterValue;

            if (!SimulationCommon::ParseAttributeString(boolParameterElement, "Key", boolParameterName)
                    || !SimulationCommon::ParseAttributeBool(boolParameterElement, "Value", boolParameterValue)
                    || !componentParameterSet.AddParameterBool(boolParameterName, boolParameterValue))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import bool parameter.";
                return false;
            }

            boolParameterElement = boolParameterElement.nextSiblingElement("Bool");
        }
    }

    return true;
}

bool ParameterImporter::ImportIntParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement intParameterElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "Int", intParameterElement))
    {
        //Iterates over all int parameters
        while (!intParameterElement.isNull())
        {
            std::string intParameterName;
            int intParameterValue;

            if (!SimulationCommon::ParseAttributeString(intParameterElement, "Key", intParameterName)
                    || !SimulationCommon::ParseAttributeInt(intParameterElement, "Value", intParameterValue)
                    || !componentParameterSet.AddParameterInt(intParameterName, intParameterValue))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import int parameter.";
                return false;
            }

            intParameterElement = intParameterElement.nextSiblingElement("Int");
        }
    }

    return true;
}

bool ParameterImporter::ImportIntVectorParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement intVectorParameterElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "IntVector", intVectorParameterElement))
    {
        //Iterates over all int vector parameters
        while (!intVectorParameterElement.isNull())
        {
            std::string intVectorParameterName;
            std::vector<int> intVectorParameterValue;

            if (!SimulationCommon::ParseAttributeString(intVectorParameterElement, "Key", intVectorParameterName)
                    || !SimulationCommon::ParseAttributeIntVector(intVectorParameterElement, "Value", &intVectorParameterValue)
                    || !componentParameterSet.AddParameterIntVector(intVectorParameterName, intVectorParameterValue))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import int vector parameter.";
                return false;
            }

            intVectorParameterElement = intVectorParameterElement.nextSiblingElement("IntVector");
        }
    }

    return true;
}

bool ParameterImporter::ImportDoubleParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement doubleParameterElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "Double", doubleParameterElement))
    {
        //Iterates over all double parameters
        while (!doubleParameterElement.isNull())
        {
            std::string doubleParameterName;
            double doubleParameterValue;

            if (!SimulationCommon::ParseAttributeString(doubleParameterElement, "Key", doubleParameterName)
                    || !SimulationCommon::ParseAttributeDouble(doubleParameterElement, "Value", doubleParameterValue)
                    || !componentParameterSet.AddParameterDouble(doubleParameterName, doubleParameterValue))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import double parameter.";
                return false;
            }

            doubleParameterElement = doubleParameterElement.nextSiblingElement("Double");
        }
    }

    return true;
}

bool ParameterImporter::ImportDoubleVectorParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement doubleVectorParameterElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "DoubleVector", doubleVectorParameterElement))
    {
        //Iterates over all double vector parameters
        while (!doubleVectorParameterElement.isNull())
        {
            std::string doubleVectorParameterName;
            std::vector<double> doubleVectorParameterValue;

            if (!SimulationCommon::ParseAttributeString(doubleVectorParameterElement, "Key", doubleVectorParameterName)
                    || !SimulationCommon::ParseAttributeDoubleVector(doubleVectorParameterElement, "Value", &doubleVectorParameterValue)
                    || !componentParameterSet.AddParameterDoubleVector(doubleVectorParameterName, doubleVectorParameterValue))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import double vector parameter.";
                return false;
            }

            doubleVectorParameterElement = doubleVectorParameterElement.nextSiblingElement("DoubleVector");
        }
    }

    return true;
}

bool ParameterImporter::ImportStringParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement stringParameterElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "String", stringParameterElement))
    {
        //Iterates over all string parameters
        while (!stringParameterElement.isNull())
        {
            std::string stringParameterName;
            std::string stringParameterValue;

            if (!SimulationCommon::ParseAttributeString(stringParameterElement, "Key", stringParameterName)
                    || !SimulationCommon::ParseAttributeString(stringParameterElement, "Value", stringParameterValue)
                    || !componentParameterSet.AddParameterString(stringParameterName, stringParameterValue))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import string parameter.";
                return false;
            }

            stringParameterElement = stringParameterElement.nextSiblingElement("String");
        }
    }

    return true;
}

bool ParameterImporter::ImportNormalDistributionParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement normalDistributionParameterElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "NormalDistribution",
            normalDistributionParameterElement))
    {
        //Iterates over all string parameters
        while (!normalDistributionParameterElement.isNull())
        {
            std::string normalDistributionParameterName;
            StochasticDefintions::NormalDistributionParameter normalDistribution;

            if (!SimulationCommon::ParseAttributeString(
                        normalDistributionParameterElement, "Key", normalDistributionParameterName)
                    || !SimulationCommon::ParseAttributeDouble(
                        normalDistributionParameterElement, "Mean", normalDistribution.mean)
                    || !SimulationCommon::ParseAttributeDouble(
                        normalDistributionParameterElement, "SD", normalDistribution.standardDeviation)
                    || !SimulationCommon::ParseAttributeDouble(
                        normalDistributionParameterElement, "Min", normalDistribution.min)
                    || !SimulationCommon::ParseAttributeDouble(
                        normalDistributionParameterElement, "Max", normalDistribution.max)
                    || !componentParameterSet.AddParameterNormalDistribution(
                        normalDistributionParameterName, normalDistribution))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import normal distribution parameter.";
                return false;
            }

            normalDistributionParameterElement = normalDistributionParameterElement.nextSiblingElement("NormalDistribution");
        }
    }

    return true;
}

bool ParameterImporter::ImportParameterLists(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    QDomElement parameterListsElement;
    if (SimulationCommon::GetFirstChildElement(componentParameterSetElement, "List", parameterListsElement))
    {
        //Iterates over all parameter lists
        while (!parameterListsElement.isNull())
        {
            std::string parameterListName;

            if (!SimulationCommon::ParseAttributeString(parameterListsElement, "Name", parameterListName))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import string parameter.";
                return false;
            }

            // Parse all list items
            QDomElement parameterListElement;
            if (SimulationCommon::GetFirstChildElement(parameterListsElement, "ListItem", parameterListElement))
            {
                while (!parameterListElement.isNull())
                {
                    auto& listItem = componentParameterSet.InitializeListItem(parameterListName);
                    ImportParameters(parameterListElement, listItem);
                    parameterListElement = parameterListElement.nextSiblingElement("ListItem");
                }
            }

            parameterListsElement = parameterListsElement.nextSiblingElement("List");
        }
    }

    return true;
}

bool ParameterImporter::ImportParameters(
    QDomElement componentParameterSetElement,
    ParameterInterface& componentParameterSet)
{
    //Parse Bool Parameters
    if (!ImportBoolParameters(componentParameterSetElement, componentParameterSet)
            || !ImportIntParameters(componentParameterSetElement, componentParameterSet)
            || !ImportIntVectorParameters(componentParameterSetElement, componentParameterSet)
            || !ImportDoubleParameters(componentParameterSetElement, componentParameterSet)
            || !ImportDoubleVectorParameters(componentParameterSetElement, componentParameterSet)
            || !ImportNormalDistributionParameters(componentParameterSetElement, componentParameterSet)
            || !ImportStringParameters(componentParameterSetElement, componentParameterSet))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import parameter set.";
        return false;
    }

    if (!ImportParameterLists(componentParameterSetElement, componentParameterSet))
    {
        LOG_INTERN(LogLevel::Error) << "Could not import parameter lists.";
        return false;
    }

    return true;
}
