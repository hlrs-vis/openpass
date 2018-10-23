/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <memory>
#include <QDomDocument>
#include "parameterParser.h"
#include "xmlParser.h"
#include "log.h"

// local helper macros
#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
{ \
    LOG_INTERN(LogLevel::Warning) << "an error occurred during import"; \
    return false; \
    } \
    } \
    while(0);

namespace SimulationCommon
{

bool ImportParameters(QDomElement &parametersElement, Parameters &parameters)
{
    // parse double parameters
    QDomElement parameterDouble;
    if(GetFirstChildElement(parametersElement, "Double", parameterDouble))
    {
        while(!parameterDouble.isNull())
        {
            // parse id
            int id;
            CHECKFALSE(ParseAttributeInt(parameterDouble, "id", id));

            // parse value
            double value;
            CHECKFALSE(ParseAttributeDouble(parameterDouble, "value", value));

            std::stringstream stream;
            stream << "- double: " << id << " = " << value;
            LOG_INTERN(LogLevel::DebugCore) << stream.str();

            CHECKFALSE(parameters.AddParameterDouble(id, value));

            parameterDouble = parameterDouble.nextSiblingElement("Double");
        } // double parameter loop
    } // if double parameters exist

    // parse int parameters
    QDomElement parameterInt;
    if(GetFirstChildElement(parametersElement, "Int", parameterInt))
    {
        while(!parameterInt.isNull())
        {
            // parse id
            int id;
            CHECKFALSE(ParseAttributeInt(parameterInt, "id", id));

            // parse value
            int value;
            CHECKFALSE(ParseAttributeInt(parameterInt, "value", value));

            std::stringstream stream;
            stream << "- int: " << id << " = " << value;
            LOG_INTERN(LogLevel::DebugCore) << stream.str();

            CHECKFALSE(parameters.AddParameterInt(id, value));

            parameterInt = parameterInt.nextSiblingElement("Int");
        } // int parameter loop
    } // if int parameters exist

    // parse bool parameters
    QDomElement parameterBool;
    if(GetFirstChildElement(parametersElement, "Bool", parameterBool))
    {
        while(!parameterBool.isNull())
        {
            // parse id
            int id;
            CHECKFALSE(ParseAttributeInt(parameterBool, "id", id));

            // parse value
            bool value;
            CHECKFALSE(ParseAttributeBool(parameterBool, "value", value));

            std::stringstream stream;
            stream << "- int: " << id << " = " << (value ? "true" : "false");
            LOG_INTERN(LogLevel::DebugCore) << stream.str();

            CHECKFALSE(parameters.AddParameterBool(id, value));

            parameterBool = parameterBool.nextSiblingElement("Bool");
        } // bool parameter loop
    } // if bool parameters exist

    // parse string parameters
    QDomElement parameterString;
    if(GetFirstChildElement(parametersElement, "String", parameterString))
    {
        while(!parameterString.isNull())
        {
            // parse id
            int id;
            CHECKFALSE(ParseAttributeInt(parameterString, "id", id));

            // parse value
            std::string value;
            CHECKFALSE(ParseAttributeString(parameterString, "value", value));

            std::stringstream stream;
            stream << "- string: " << id << " = " << value;
            LOG_INTERN(LogLevel::DebugCore) << stream.str();

            CHECKFALSE(parameters.AddParameterString(id, value));

            parameterString = parameterString.nextSiblingElement("String");
        } // string parameter loop
    } // if string parameters exist

    // parse double vector parameters
    QDomElement parameterDoubleVector;
    if(GetFirstChildElement(parametersElement, "DoubleVector", parameterDoubleVector))
    {
        while(!parameterDoubleVector.isNull())
        {
            // parse id
            int id;
            CHECKFALSE(ParseAttributeInt(parameterDoubleVector, "id", id));

            std::stringstream logStream;
            logStream << "- double vector: " << id << " = ";

            // parse value

            std::vector<double> *vector = new (std::nothrow) std::vector<double>();
            CHECKFALSE(vector);

            CHECKFALSE(ParseAttributeDoubleVector(parameterDoubleVector, "value", vector));
            if(!parameters.AddParameterDoubleVector(id, vector))
            {
                LOG_INTERN(LogLevel::Warning) << "an error occurred during import of parameters";
                delete vector;
                return false;
            }

            for(const double &item : *vector)
            {
                logStream << item << ", ";
            }

            LOG_INTERN(LogLevel::DebugCore) << logStream.str();

            parameterDoubleVector = parameterDoubleVector.nextSiblingElement("DoubleVector");
        } // double vector parameter loop
    } // if double vector parameters exist

    // parse int vector parameters
    QDomElement parameterIntVector;
    if(GetFirstChildElement(parametersElement, "IntVector", parameterIntVector))
    {
        while(!parameterIntVector.isNull())
        {
            // parse id
            int id;
            CHECKFALSE(ParseAttributeInt(parameterIntVector, "id", id));

            std::stringstream logStream;
            logStream << "- int vector: " << id << " = ";

            // parse value
            std::vector<int> *vector = new (std::nothrow) std::vector<int>();
            CHECKFALSE(vector);

            CHECKFALSE(ParseAttributeIntVector(parameterIntVector, "value", vector));
            if(!parameters.AddParameterIntVector(id, vector))
            {
                LOG_INTERN(LogLevel::Warning) << "an error occurred during import of parameters";
                delete vector;
                return false;
            }

            for(int item : *vector)
            {
                logStream << item << ", ";
            }

            LOG_INTERN(LogLevel::DebugCore) << logStream.str();

            parameterIntVector = parameterIntVector.nextSiblingElement("IntVector");
        } // int vector parameter loop
    } // if int vector parameters exist

    // parse bool vector parameters
    QDomElement parameterBoolVector;
    if(GetFirstChildElement(parametersElement, "BoolVector", parameterBoolVector))
    {
        while(!parameterBoolVector.isNull())
        {
            // parse id
            int id;
            CHECKFALSE(ParseAttributeInt(parameterBoolVector, "id", id));

            std::stringstream logStream;
            logStream << "- bool vector: " << id << " = ";

            // parse value
            std::vector<bool> *vector = new (std::nothrow) std::vector<bool>();
            CHECKFALSE(vector);

            CHECKFALSE(ParseAttributeBoolVector(parameterBoolVector, "value", vector));
            if(!parameters.AddParameterBoolVector(id, vector))
            {
                LOG_INTERN(LogLevel::Warning) << "an error occurred during import of parameters";
                delete vector;
                return false;
            }

            for(const bool &item : *vector)
            {
                logStream << item << ", ";
            }

            LOG_INTERN(LogLevel::DebugCore) << logStream.str();

            parameterBoolVector = parameterBoolVector.nextSiblingElement("BoolVector");
        } // bool vector parameter loop
    } // if bool vector parameters exist

    return true;
}

bool ImportSystemParameters(QDomElement &parametersElement, Parameters &parameters)
{
    QDomElement parameterElement = parametersElement.firstChildElement("parameter");
    while(!parameterElement.isNull()){
        int id = parameterElement.firstChildElement("id").text().toInt();

        QString type = parameterElement.firstChildElement("type").text();
        QString value = parameterElement.firstChildElement("value").text();

        if(type == "int"){
            CHECKFALSE(parameters.AddParameterInt(id, value.toInt()));
        }else if(type == "double"){
            CHECKFALSE(parameters.AddParameterDouble(id, value.toDouble()));
        }else if(type == "bool"){
            value.toLower();
            CHECKFALSE(parameters.AddParameterBool(id, value == "true" ? true : false));
        }else if(type == "string"){
            CHECKFALSE(parameters.AddParameterString(id, value.toStdString()));
        }else if(type == "intVector"){
            std::vector<int> *vector = new (std::nothrow) std::vector<int>();
            CHECKFALSE(vector);
            try
            {
                std::stringstream valueStream(value.toStdString());

                int item;
                while(valueStream >> item)
                {
                    vector->push_back(item);

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
            if(!parameters.AddParameterIntVector(id, vector))
            {
                LOG_INTERN(LogLevel::Warning) << "an error occurred during import of parameters";
                delete vector;
                return false;
            }
        }else if(type == "doubleVector"){
            std::vector<double> *vector = new (std::nothrow) std::vector<double>();
            CHECKFALSE(vector);
            try
            {
                std::stringstream valueStream(value.toStdString());

                double item;
                while(valueStream >> item)
                {
                    vector->push_back(item);

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
            if(!parameters.AddParameterDoubleVector(id, vector))
            {
                LOG_INTERN(LogLevel::Warning) << "an error occurred during import of parameters";
                delete vector;
                return false;
            }
        }else if(type == "boolVector"){
            std::vector<bool> *vector = new (std::nothrow) std::vector<bool>();
            CHECKFALSE(vector);
            try
            {
                std::stringstream valueStream(value.toStdString());

                bool item;
                while(valueStream >> item)
                {
                    vector->push_back(item);

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
            if(!parameters.AddParameterBoolVector(id, vector))
            {
                LOG_INTERN(LogLevel::Warning) << "an error occurred during import of parameters";
                delete vector;
                return false;
            }
        }

        parameterElement = parameterElement.nextSiblingElement("parameter");
    }
    return true;
}

} // namespace SimulationCommon
