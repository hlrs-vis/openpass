/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlTrajectory.h"

XmlTrajectory::XmlTrajectory(int agentId, PCM_Trajectory *trajectory)
{
    this->agentId = agentId;
    this->trajectory = trajectory;
}

bool XmlTrajectory::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Trajectory");

    if (trajectory != nullptr)
    {
        xmlWriter->writeAttribute("id", QString::number(agentId));
        xmlWriter->writeTextElement("Time", trajectory->GetTimeVecString());
        xmlWriter->writeTextElement("XPos", trajectory->GetXPosVecString());
        xmlWriter->writeTextElement("YPos", trajectory->GetYPosVecString());
        xmlWriter->writeTextElement("VelocityX", trajectory->GetUVelVecString());
        xmlWriter->writeTextElement("VelocityY", trajectory->GetVVelVecString());
        xmlWriter->writeTextElement("Angle", trajectory->GetPsiVecString());
    }

    xmlWriter->writeEndElement();

    return true;
}
