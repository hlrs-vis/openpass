/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlGlobalData.h"

XmlGlobalData::XmlGlobalData(PCM_GlobalData *globalData)
{
    this->globalData = globalData;
}

bool XmlGlobalData::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement(QString::fromStdString(PCM_Helper::GetGlobalDataDBString()));

    if (globalData != nullptr)
    {
        xmlWriter->writeTextElement("OffsetX", QString::number(globalData->GetOffsetX(), 'g', 10));
        xmlWriter->writeTextElement("OffsetY", QString::number(globalData->GetOffsetY(), 'g', 10));
        xmlWriter->writeTextElement("Participants", QString::number(globalData->GetParticipants()));
        xmlWriter->writeTextElement("SimulationVersion",
                                    QString::number(globalData->GetSimulationVersion()));
    }

    xmlWriter->writeEndElement();

    return true;
}
