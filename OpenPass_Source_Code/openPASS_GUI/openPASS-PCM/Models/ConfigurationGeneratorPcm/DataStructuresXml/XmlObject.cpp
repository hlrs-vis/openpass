/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlObject.h"

XmlObject::XmlObject(PCM_Object *object)
{
    this->object = object;
}

bool XmlObject::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    if (object != nullptr)
    {
        xmlWriter->writeStartElement(QString::fromStdString(PCM_Helper::ConvertObjectTypeToDBString(
                                                                ObjectType::OBJECT)));

        const std::map<int, PCM_Line *> *lineMap = object->GetLineMap();
        for (std::pair<int, PCM_Line *> linePair : *lineMap)
        {
            PCM_Line *line = linePair.second;
            XmlLine xmlLine(line);
            xmlLine.WriteToXml(xmlWriter);
        }
        xmlWriter->writeTextElement("ObjectType", QString::number(object->GetObjectType()));

        xmlWriter->writeEndElement();
    }

    return true;
}
