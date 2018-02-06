/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemXMLSaveModel.h"

#include <QFile>
#include <QIODevice>
#include <QXmlStreamWriter>
#include "openPASS-System/SystemComponentMapInterface.h"
#include "openPASS-System/SystemComponentParameterMapInterface.h"
#include "openPASS-System/SystemComponentScheduleInterface.h"
#include "openPASS-System/SystemConnectionMapInterface.h"
#include "openPASS-System/SystemItemInterface.h"
#include "openPASS-System/SystemMapInterface.h"

bool SystemXMLSaveModel::save(QString const & filepath,
                              SystemMapInterface const * const systems)
{
    // Can we open the file in write only mode?
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Save system to file
        return save(&file, systems);
    }
    // Return failure
    return false;
}

bool SystemXMLSaveModel::save(QIODevice * const device,
                              SystemMapInterface const * const systems)
{
    // Initialize xml stream writer
    QXmlStreamWriter xml(device);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(2);

    // Save system to xml stream
    xml.writeStartDocument();
    xml.writeStartElement(KeySystems);
    for (SystemItemInterface const * const system : *systems)
    {
        saveSystem(xml, system);
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    return true;
}

void SystemXMLSaveModel::saveSystem(QXmlStreamWriter & xml,
                                    SystemItemInterface const * const system)
{
    xml.writeStartElement(KeySystem);
    xml.writeTextElement(KeySystemID, QString::number(system->getID()));
    xml.writeTextElement(KeySystemTitle, system->getTitle());
    xml.writeTextElement(KeySystemPriority, QString::number(system->getPriority()));
    saveSystemComponent(xml, system->getComponents());
    saveSystemConnection(xml, system->getConnections());
    xml.writeEndElement();
}

void SystemXMLSaveModel::saveSystemComponent(QXmlStreamWriter &xml,
                                             SystemComponentMapInterface const * const components)
{
    xml.writeStartElement(KeySystemComponents);
    for (SystemComponentMapInterface::Item const * const component: * components)
    {
        xml.writeStartElement(KeySystemComponent);
        xml.writeTextElement(KeySystemComponentID, QString::number(component->getID()));
        xml.writeTextElement(KeySystemComponentLibrary, component->getLibrary());
        xml.writeTextElement(KeySystemComponentTitle, component->getTitle());
        saveSystemComponentSchedule(xml, component->getSchedule());
        saveSystemComponentParameters(xml, component->getParameters());
        xml.writeStartElement(KeySystemComponentPosition);
        xml.writeTextElement(KeySystemComponentPositionX, QString::number(component->getPosition().x()));
        xml.writeTextElement(KeySystemComponentPositionY, QString::number(component->getPosition().y()));
        xml.writeEndElement();
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

void SystemXMLSaveModel::saveSystemComponentSchedule(QXmlStreamWriter &xml,
                                                     SystemComponentScheduleInterface const * const schedule)
{
    xml.writeStartElement(KeySystemComponentSchedule);
    xml.writeTextElement(KeySystemComponentSchedulePriority, QString::number(schedule->getPriority()));
    xml.writeTextElement(KeySystemComponentScheduleOffset, QString::number(schedule->getOffset()));
    xml.writeTextElement(KeySystemComponentScheduleCycle, QString::number(schedule->getCycle()));
    xml.writeTextElement(KeySystemComponentScheduleResponse, QString::number(schedule->getResponse()));
    xml.writeEndElement();
}

void SystemXMLSaveModel::saveSystemComponentParameters(QXmlStreamWriter &xml,
                                                       SystemComponentParameterMapInterface const * const parameters)
{
    if (!(parameters->count() > 0))
    {
        xml.writeEmptyElement(KeySystemComponentParameters);
        return;
    }
    xml.writeStartElement(KeySystemComponentParameters);
    for (SystemComponentParameterMapInterface::Item const * const parameter : *parameters)
    {
        xml.writeStartElement(KeySystemComponentParameter);
        xml.writeTextElement(KeySystemComponentParameterID, QString::number(parameter->getID()));
        xml.writeTextElement(KeySystemComponentParameterType, parameter->getType());
        xml.writeTextElement(KeySystemComponentParameterUnit, parameter->getUnit());
        xml.writeTextElement(KeySystemComponentParameterValue, parameter->getValue());
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

void SystemXMLSaveModel::saveSystemConnection(QXmlStreamWriter &xml,
                                              SystemConnectionMapInterface const * const connections)
{
    xml.writeStartElement(KeySystemConnections);
    for (SystemConnectionMapInterface::Item const * const connection: *connections)
    {
        xml.writeStartElement(KeySystemConnection);
        xml.writeTextElement(KeySystemConnectionID, QString::number(connection->getID()));
        xml.writeStartElement(KeySystemConnectionSource);
        xml.writeTextElement(KeySystemConnectionSourceComponent, QString::number(
                                 connection->getSource()->getComponent()->getID()));
        xml.writeTextElement(KeySystemConnectionSourceOutput, QString::number(
                                 connection->getSource()->getID()));
        xml.writeEndElement();
        xml.writeStartElement(KeySystemConnectionTarget);
        xml.writeTextElement(KeySystemConnectionTargetComponent, QString::number(
                                 connection->getTarget()->getComponent()->getID()));
        xml.writeTextElement(KeySystemConnectionTargetInput, QString::number(
                                 connection->getTarget()->getID()));
        xml.writeEndElement();
        xml.writeEndElement();
    }
    xml.writeEndElement();
}
