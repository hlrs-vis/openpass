/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentXMLLoadModel.h"

#include "Models/ComponentInputItemModel.h"
#include "Models/ComponentOutputItemModel.h"
#include "Models/ComponentParameterItemModel.h"
#include <QIODevice>
#include <QFile>
#include <QXmlStreamReader>

bool ComponentXMLLoadModel::load(QString const & filepath,
                                 ComponentItemInterface * const item)
{
    // Does the file exist and can we open it?
    QFile file(filepath);
    if ((file.exists()) && (file.open(QIODevice::ReadOnly | QIODevice::Text)))
    {
        // Load component from file
        return load(&file, item);
    }
    // Return failure
    return false;
}

bool ComponentXMLLoadModel::load(QIODevice * const device,
                                 ComponentItemInterface * const item)
{
    // Initialize xml stream reader
    QXmlStreamReader xml(device);

    // Verify xml header
    if ((xml.readNext() == QXmlStreamReader::TokenType::StartDocument) &&
            (xml.readNext() == QXmlStreamReader::TokenType::StartElement) &&
            (xml.name() == KeyComponent))
    {
        // Load component from xml stream
        return loadComponent(xml, item);
    }
    return false;
}

bool ComponentXMLLoadModel::loadComponent(QXmlStreamReader & xml,
                                          ComponentItemInterface * const item)
{
    bool success = true;
    QList<QString> keys = KeyListComponent;
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeyComponentType)
            item->setType(convertStringToComponentType(xml.readElementText()));
        else if (xml.name() == KeyComponentTitle)
            item->setTitle(xml.readElementText());
        else if (xml.name() == KeyComponentSchedule)
            success = loadComponentSchedule(xml, item->getSchedule()) && success;
        else if (xml.name() == KeyComponentParameters)
            success = loadComponentParameterMap(xml, item->getParameters()) && success;
        else if (xml.name() == KeyComponentInputs)
            success = loadComponentInputMap(xml, item->getInputs()) && success;
        else if (xml.name() == KeyComponentOutputs)
            success = loadComponentOutputMap(xml, item->getOutputs()) && success;
        else
            xml.skipCurrentElement();
    }
    return success && keys.isEmpty();
}

bool ComponentXMLLoadModel::loadComponentSchedule(QXmlStreamReader & xml,
                                                  ComponentScheduleInterface * const schedule)
{
    QList<QString> keys = KeyListComponentSchedule;
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeyComponentScheduleOffset)
            schedule->setOffset(xml.readElementText().toUInt());
        else if (xml.name() == KeyComponentScheduleCycle)
            schedule->setCycle(xml.readElementText().toUInt());
        else if (xml.name() == KeyComponentScheduleResponse)
            schedule->setResponse(xml.readElementText().toUInt());
        else
            xml.skipCurrentElement();
    }
    return keys.isEmpty();
}

bool ComponentXMLLoadModel::loadComponentParameterMap(QXmlStreamReader & xml,
                                                      ComponentParameterMapInterface * const parameters)
{
    bool success = true;
    while (xml.readNextStartElement())
    {
        if (xml.name() == KeyComponentParameter)
            success = loadComponentParameterItem(xml, parameters) && success;
        else
            xml.skipCurrentElement();
    }
    return success;
}

bool ComponentXMLLoadModel::loadComponentParameterItem(QXmlStreamReader & xml,
                                                       ComponentParameterMapInterface * const parameters)
{
    QList<QString> keys = KeyListComponentParameter;
    ComponentParameterMapInterface::ID id;
    ComponentParameterMapInterface::Item * const item = new ComponentParameterItemModel();
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeyComponentParameterID)
            id = xml.readElementText().toUInt();
        else if (xml.name() == KeyComponentParameterType)
            item->setType(xml.readElementText());
        else if (xml.name() == KeyComponentParameterTitle)
            item->setTitle(xml.readElementText());
        else if (xml.name() == KeyComponentParameterUnit)
            item->setUnit(xml.readElementText());
        else if (xml.name() == KeyComponentParameterValue)
            item->setValue(xml.readElementText());
        else
            xml.skipCurrentElement();
    }
    if (keys.isEmpty() && parameters->add(id, item))
        return true;
    delete item;
    return false;
}

bool ComponentXMLLoadModel::loadComponentInputMap(QXmlStreamReader & xml,
                                                  ComponentInputMapInterface * const inputs)
{
    bool success = true;
    while (xml.readNextStartElement())
    {
        if (xml.name() == KeyComponentInput)
            success = loadComponentInputItem(xml, inputs) && success;
        else
            xml.skipCurrentElement();
    }
    return success;
}

bool ComponentXMLLoadModel::loadComponentInputItem(QXmlStreamReader & xml,
                                                   ComponentInputMapInterface * const inputs)
{
    QList<QString> keys = KeyListComponentInput;
    ComponentInputMapInterface::ID id;
    ComponentInputMapInterface::Item * const item = new ComponentInputItemModel();
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeyComponentInputID)
            id = xml.readElementText().toUInt();
        else if (xml.name() == KeyComponentInputType)
            item->setType(xml.readElementText());
        else if (xml.name() == KeyComponentInputTitle)
            item->setTitle(xml.readElementText());
        else if (xml.name() == KeyComponentInputUnit)
            item->setUnit(xml.readElementText());
        else if (xml.name() == KeyComponentInputCardinality)
            item->setCardinality(xml.readElementText());
        else
            xml.skipCurrentElement();
    }
    if (keys.isEmpty() && inputs->add(id, item))
        return true;
    delete item;
    return false;
}

bool ComponentXMLLoadModel::loadComponentOutputMap(QXmlStreamReader & xml,
                                                   ComponentOutputMapInterface * const outputs)
{
    bool success = true;
    while (xml.readNextStartElement())
    {
        if (xml.name() == KeyComponentOutput)
            success = loadComponentOutputItem(xml, outputs) && success;
        else
            xml.skipCurrentElement();
    }
    return success;
}

bool ComponentXMLLoadModel::loadComponentOutputItem(QXmlStreamReader & xml,
                                                    ComponentOutputMapInterface * const outputs)
{
    QList<QString> keys = KeyListComponentOutput;
    ComponentOutputMapInterface::ID id;
    ComponentOutputMapInterface::Item * const item = new ComponentOutputItemModel();
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeyComponentOutputID)
            id = xml.readElementText().toUInt();
        else if (xml.name() == KeyComponentOutputType)
            item->setType(xml.readElementText());
        else if (xml.name() == KeyComponentOutputTitle)
            item->setTitle(xml.readElementText());
        else if (xml.name() == KeyComponentOutputUnit)
            item->setUnit(xml.readElementText());
        else
            xml.skipCurrentElement();
    }
    if (keys.isEmpty() && outputs->add(id, item))
        return true;
    delete item;
    return false;
}
