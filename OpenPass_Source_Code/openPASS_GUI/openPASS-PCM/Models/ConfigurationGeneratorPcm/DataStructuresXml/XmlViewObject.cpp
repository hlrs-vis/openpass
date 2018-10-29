/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlViewObject.h"

XmlViewObject::XmlViewObject(PCM_ViewObject *viewObject)
{
    this->viewObject = viewObject;
}

bool XmlViewObject::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    if (viewObject != nullptr)
    {
        xmlWriter->writeStartElement(QString::fromStdString(PCM_Helper::ConvertObjectTypeToDBString(
                                                                ObjectType::VIEWOBJECT)));
        const std::map<int, PCM_Line *> *lineMap = viewObject->GetLineMap();
        for (std::pair< int, PCM_Line *> linePair : *lineMap)
        {
            PCM_Line *line = linePair.second;
            XmlLine xmlLine(line);
            xmlLine.WriteToXml(xmlWriter);
        }
        xmlWriter->writeEndElement();
    }

    return true;
}
