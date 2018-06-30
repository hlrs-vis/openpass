/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemXMLLoadModel.h"

#include "Models/SystemComponentItemModel.h"
#include <QIODevice>
#include <QFile>
#include <QXmlStreamReader>

bool SystemXMLLoadModel::load(QString const & filepath,
                              SystemItemInterface * const system,
                              SystemComponentManagerInterface const * const manager)
{
    // Does the file exist and can we open it?
    QFile file(filepath);
    if ((file.exists()) && (file.open(QIODevice::ReadOnly | QIODevice::Text)))
    {
        // Load component from file
        return load(&file, system, manager);
    }
    // Return failure
    return false;
}

bool SystemXMLLoadModel::load(QIODevice * const device,
                              SystemItemInterface * const system,
                              SystemComponentManagerInterface const * const manager)
{
    // Initialize xml stream reader
    QXmlStreamReader xml(device);

    // Verify xml header
    if ((xml.readNext() == QXmlStreamReader::TokenType::StartDocument) &&
            (xml.readNext() == QXmlStreamReader::TokenType::StartElement) &&
            (xml.name() == KeySystems))
    {
        // Verify first system
        xml.readNext();
        if ((xml.readNext() == QXmlStreamReader::TokenType::StartElement) &&
                (xml.name() == KeySystem))
        {
            // Load first system from xml stream
            return loadSystemItem(xml, system, manager);
        }
        return false;
    }
    return false;
}

bool SystemXMLLoadModel::loadSystemItem(QXmlStreamReader & xml,
                                        SystemItemInterface * const system,
                                        SystemComponentManagerInterface const * const manager)
{
    bool success = true;
    QList<QString> keys = KeyListSystem;
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemTitle)
            system->setTitle(xml.readElementText());
        else if (xml.name() == KeySystemPriority)
            system->setPriority(xml.readElementText().toUInt());
        else if (xml.name() == KeySystemComponents)
        {
            // Enable component modify mode
            Q_EMIT system->getComponents()->mode(true);
            // Load components from xml stream
            success = loadSystemComponentMap(
                        xml, system->getComponents(), manager) && success;
            // Disable component modify mode
            Q_EMIT system->getComponents()->mode(false);
        }
        else if (xml.name() == KeySystemConnections)
        {
            // Enable connection modify mode
            Q_EMIT system->getComponents()->mode(true);
            // Load connections from xml stream
            success = loadSystemConnectionMap(
                        xml, system->getComponents(), system->getConnections()) && success;
            // Disable connection modify mode
            Q_EMIT system->getComponents()->mode(false);
        }
        else
            xml.skipCurrentElement();
    }
    return success && keys.isEmpty();
}

bool SystemXMLLoadModel::loadSystemComponentMap(QXmlStreamReader & xml,
                                                SystemComponentMapInterface * const components,
                                                SystemComponentManagerInterface const * const manager)
{
    bool success = true;
    while (xml.readNextStartElement())
    {
        if (xml.name() == KeySystemComponent)
            success = loadSystemComponentItem(xml, components, manager) && success;
        else
            xml.skipCurrentElement();
    }
    return success;
}

bool SystemXMLLoadModel::loadSystemComponentItem(QXmlStreamReader & xml,
                                                 SystemComponentMapInterface * const components,
                                                 SystemComponentManagerInterface const * const manager)
{
    bool success = true;
    QList<QString> keys = KeyListSystemComponent;
    // Load ID from xml stream
    xml.readNext();
    if (!((xml.readNext() == QXmlStreamReader::TokenType::StartElement) &&
          (xml.name() == KeySystemComponentID)))
        return false;
    SystemComponentMapInterface::ID const id = xml.readElementText().toUInt();
    keys.removeAll(xml.name().toString());
    // Load library from xml stream, lookup component and create system component
    xml.readNext();
    if (!((xml.readNext() == QXmlStreamReader::TokenType::StartElement) &&
          (xml.name() == KeySystemComponentLibrary)))
        return false;
    ComponentItemInterface const * const base = manager->lookupItemByName(xml.readElementText());
    if (!base)
        return false;
    SystemComponentMapInterface::Item * const component = new SystemComponentItemModel(base);
    keys.removeAll(xml.name().toString());
    // Load system component from xml stream
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemComponentTitle)
            component->setTitle(xml.readElementText());
        else if (xml.name() == KeySystemComponentPriority)
            component->getSchedule()->setPriority(xml.readElementText().toUInt());
        else if (xml.name() == KeySystemComponentSchedule)
            success = loadSystemComponentSchedule(xml, component->getSchedule()) && success;
        else if (xml.name() == KeySystemComponentParameters)
            success = loadSystemComponentParameterMap(xml, component->getParameters()) && success;
        else if (xml.name() == KeySystemComponentPosition)
            success = loadSystemComponentPosition(xml, component) && success;
        else
            xml.skipCurrentElement();
    }
    // Add system component to system
    if (success && keys.isEmpty() && components->add(id, component))
        return true;
    delete component;
    return false;
}

bool SystemXMLLoadModel::loadSystemComponentSchedule(QXmlStreamReader & xml,
                                                     SystemComponentScheduleInterface * const schedule)
{
    QList<QString> keys = KeyListSystemComponentSchedule;
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemComponentSchedulePriority)
            schedule->setPriority(xml.readElementText().toUInt());
        else if (xml.name() == KeySystemComponentScheduleOffset)
            schedule->setOffset(xml.readElementText().toUInt());
        else if (xml.name() == KeySystemComponentScheduleCycle)
            schedule->setCycle(xml.readElementText().toUInt());
        else if (xml.name() == KeySystemComponentScheduleResponse)
            schedule->setResponse(xml.readElementText().toUInt());
        else
            xml.skipCurrentElement();
    }
    return keys.isEmpty();
}

bool SystemXMLLoadModel::loadSystemComponentParameterMap(QXmlStreamReader & xml,
                                                         SystemComponentParameterMapInterface * const parameters)
{
    bool success = true;
    while (xml.readNextStartElement())
    {
        if (xml.name() == KeySystemComponentParameter)
            success = loadSystemComponentParameterItem(xml, parameters) && success;
        else
            xml.skipCurrentElement();
    }
    return success;
}

bool SystemXMLLoadModel::loadSystemComponentParameterItem(QXmlStreamReader & xml,
                                                          SystemComponentParameterMapInterface * const parameters)
{
    QList<QString> keys = KeyListSystemComponentParameter;
    SystemComponentParameterMapInterface::ID id = 0;
    SystemComponentParameterItemInterface::Type type = SystemComponentParameterItemInterface::Type();
    SystemComponentParameterItemInterface::Unit unit = SystemComponentParameterItemInterface::Unit();
    SystemComponentParameterItemInterface::Value value = SystemComponentParameterItemInterface::Value();
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemComponentParameterID)
            id = xml.readElementText().toUInt();
        else if (xml.name() == KeySystemComponentParameterType)
            type = xml.readElementText();
        else if (xml.name() == KeySystemComponentParameterUnit)
            unit = xml.readElementText();
        else if (xml.name() == KeySystemComponentParameterValue)
            value = xml.readElementText();
        else
            xml.skipCurrentElement();
    }
    if (keys.isEmpty())
    {
        SystemComponentParameterItemInterface * const parameter = parameters->getItem(id);
        if ((parameter) && (parameter->getType() == type) && (parameter->getUnit() == unit))
        {
            parameter->setValue(value);
        }
        return true;
    }
    return false;
}

bool SystemXMLLoadModel::loadSystemComponentPosition(QXmlStreamReader & xml,
                                                     SystemComponentItemInterface * const component)
{
    QList<QString> keys = KeyListSystemComponentPosition;
    SystemComponentItemInterface::Position position = SystemComponentItemInterface::Position(0, 0);
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemComponentPositionX)
            position.setX(xml.readElementText().toInt());
        else if (xml.name() == KeySystemComponentPositionY)
            position.setY(xml.readElementText().toInt());
        else
            xml.skipCurrentElement();
    }
    return component->setPosition(position) && keys.isEmpty();
}

bool SystemXMLLoadModel::loadSystemConnectionMap(QXmlStreamReader & xml,
                                                 SystemComponentMapInterface const * const components,
                                                 SystemConnectionMapInterface * const connections)
{
    bool success = true;
    while (xml.readNextStartElement())
    {
        if (xml.name() == KeySystemConnection)
            success = loadSystemConnectionItem(xml, components, connections) && success;
        else
            xml.skipCurrentElement();
    }
    return success;
}

bool SystemXMLLoadModel::loadSystemConnectionItem(QXmlStreamReader & xml,
                                                  SystemComponentMapInterface const * const components,
                                                  SystemConnectionMapInterface * const connections)
{
    bool success = true;
    QList<QString> keys = KeyListSystemConnection;
    SystemConnectionMapInterface::ID id = 0;
    SystemComponentMapInterface::ID sourceComponentID = 0;
    SystemComponentOutputMapInterface::ID sourceOutputID = 0;
    SystemComponentMapInterface::ID targetComponentID = 0;
    SystemComponentInputMapInterface::ID targetInputID = 0;
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemConnectionID)
            id = xml.readElementText().toUInt();
        else if (xml.name() == KeySystemConnectionSource)
            success = loadSystemConnectionItemSource(
                        xml, sourceComponentID, sourceOutputID) && success;
        else if (xml.name() == KeySystemConnectionTarget)
            success = loadSystemConnectionItemTarget(
                        xml, targetComponentID, targetInputID) && success;
        else
            xml.skipCurrentElement();
    }
    if (success && keys.isEmpty())
    {
        SystemComponentMapInterface::Item const * const sourceComponent =
                components->getItem(sourceComponentID);
        SystemComponentMapInterface::Item const * const targetComponent =
                components->getItem(targetComponentID);
        if (sourceComponent && targetComponent)
        {
            SystemConnectionMapInterface::Source const * const source =
                    sourceComponent->getOutputs()->getItem(sourceOutputID);
            SystemConnectionMapInterface::Target const * const target =
                    targetComponent->getInputs()->getItem(targetInputID);
            if (source && target)
            {
                connections->add(id, source, target);
            }
        }
        return true;
    }
    return false;
}

bool SystemXMLLoadModel::loadSystemConnectionItemSource(QXmlStreamReader & xml,
                                                        SystemComponentMapInterface::ID & sourceComponentID,
                                                        SystemComponentOutputMapInterface::ID & sourceOutputID)
{
    QList<QString> keys = KeyListSystemConnectionSource;
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemConnectionSourceComponent)
            sourceComponentID = xml.readElementText().toUInt();
        else if (xml.name() == KeySystemConnectionSourceOutput)
            sourceOutputID = xml.readElementText().toUInt();
        else
            xml.skipCurrentElement();
    }
    return keys.isEmpty();
}

bool SystemXMLLoadModel::loadSystemConnectionItemTarget(QXmlStreamReader & xml,
                                                        SystemComponentMapInterface::ID & targetComponentID,
                                                        SystemComponentInputMapInterface::ID & targetInputID)
{
    QList<QString> keys = KeyListSystemConnectionTarget;
    while (xml.readNextStartElement())
    {
        keys.removeAll(xml.name().toString());
        if (xml.name() == KeySystemConnectionTargetComponent)
            targetComponentID = xml.readElementText().toUInt();
        else if (xml.name() == KeySystemConnectionTargetInput)
            targetInputID = xml.readElementText().toUInt();
        else
            xml.skipCurrentElement();
    }
    return keys.isEmpty();
}
