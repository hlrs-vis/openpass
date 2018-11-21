/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentXMLSaveModel.h"

#include <QFile>
#include <QIODevice>
#include <QXmlStreamWriter>

bool ComponentXMLSaveModel::save(QString const & filepath,
                                 ComponentItemInterface const * const item)
{
    // Can we open the file in write only mode?
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Save component to file
        return save(&file, item);
    }
    // Return failure
    return false;
}

bool ComponentXMLSaveModel::save(QIODevice * const device,
                                 ComponentItemInterface const * const item)
{
    // Initialize xml stream writer
    QXmlStreamWriter xml(device);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(2);

    // Save component to xml stream
    xml.writeStartDocument();
    saveComponent(xml, item);
    xml.writeEndDocument();
    return true;
}

void ComponentXMLSaveModel::saveComponent(QXmlStreamWriter & xml,
                                          ComponentItemInterface const * const item)
{
    xml.writeStartElement(KeyComponent);
    xml.writeTextElement(KeyComponentType, convertComponentTypeToString(item->getType()));
    xml.writeTextElement(KeyComponentTitle, item->getTitle());
    saveComponentSchedule(xml, item->getSchedule());
    saveComponentParameters(xml, item->getParameters());
    saveComponentInputs(xml, item->getInputs());
    saveComponentOutputs(xml, item->getOutputs());
    xml.writeEndElement();
}

void ComponentXMLSaveModel::saveComponentSchedule(QXmlStreamWriter & xml,
                                                  ComponentScheduleInterface const * const schedule)
{
    xml.writeStartElement(KeyComponentSchedule);
    xml.writeTextElement(KeyComponentScheduleOffset, QString::number(schedule->getOffset()));
    xml.writeTextElement(KeyComponentScheduleCycle, QString::number(schedule->getCycle()));
    xml.writeTextElement(KeyComponentScheduleResponse, QString::number(schedule->getResponse()));
    xml.writeEndElement();
}

void ComponentXMLSaveModel::saveComponentParameters(QXmlStreamWriter & xml,
                                                    ComponentParameterMapInterface const * const parameters)
{
    if (!(parameters->count() > 0))
    {
        xml.writeEmptyElement(KeyComponentParameters);
        return;
    }
    xml.writeStartElement(KeyComponentParameters);
    for (ComponentParameterMapInterface::Item const * const parameter : *parameters)
    {
        xml.writeStartElement(KeyComponentParameter);
        xml.writeTextElement(KeyComponentParameterID, QString::number(parameter->getID()));
        xml.writeTextElement(KeyComponentParameterType, parameter->getType());
        xml.writeTextElement(KeyComponentParameterTitle, parameter->getTitle());
        xml.writeTextElement(KeyComponentParameterUnit, parameter->getUnit());
        xml.writeTextElement(KeyComponentParameterValue, parameter->getValue());
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

void ComponentXMLSaveModel::saveComponentInputs(QXmlStreamWriter & xml,
                                                ComponentInputMapInterface const * const inputs)
{
    if (!(inputs->count() > 0))
    {
        xml.writeEmptyElement(KeyComponentInputs);
        return;
    }
    xml.writeStartElement(KeyComponentInputs);
    for (ComponentInputMapInterface::Item const * const input : *inputs)
    {
        xml.writeStartElement(KeyComponentInput);
        xml.writeTextElement(KeyComponentInputID, QString::number(input->getID()));
        xml.writeTextElement(KeyComponentInputType, input->getType());
        xml.writeTextElement(KeyComponentInputTitle, input->getTitle());
        xml.writeTextElement(KeyComponentInputUnit, input->getUnit());
        xml.writeTextElement(KeyComponentInputCardinality, input->getCardinality());
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

void ComponentXMLSaveModel::saveComponentOutputs(QXmlStreamWriter & xml,
                                                 ComponentOutputMapInterface const * const outputs)
{
    if (!(outputs->count() > 0))
    {
        xml.writeEmptyElement(KeyComponentOutputs);
        return;
    }
    xml.writeStartElement(KeyComponentOutputs);
    for (ComponentOutputMapInterface::Item const * const output : *outputs)
    {
        xml.writeStartElement(KeyComponentOutput);
        xml.writeTextElement(KeyComponentOutputID, QString::number(output->getID()));
        xml.writeTextElement(KeyComponentOutputType, output->getType());
        xml.writeTextElement(KeyComponentOutputTitle, output->getTitle());
        xml.writeTextElement(KeyComponentOutputUnit, output->getUnit());
        xml.writeEndElement();
    }
    xml.writeEndElement();
}
