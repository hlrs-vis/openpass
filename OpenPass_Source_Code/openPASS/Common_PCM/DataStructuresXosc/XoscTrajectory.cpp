/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XoscTrajectory.h"

XoscTrajectory::XoscTrajectory(int agentId,
                               PCM_Trajectory *trajectory)
{
    this->agentId = agentId;
    this->trajectory = trajectory;
}

bool XoscTrajectory::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Story");
    xmlWriter->writeAttribute("owner", QString::number(agentId));
    xmlWriter->writeStartElement("Act");
    xmlWriter->writeStartElement("Sequence");
    xmlWriter->writeStartElement("Actors");
    xmlWriter->writeEndElement();
    xmlWriter->writeStartElement("Maneuver");
    xmlWriter->writeStartElement("Event");

    xmlWriter->writeStartElement("Action");
    xmlWriter->writeStartElement("Private");
    xmlWriter->writeStartElement("Routing");
    xmlWriter->writeStartElement("FollowTrajectory");
    xmlWriter->writeStartElement("Trajectory");

    if (trajectory != nullptr)
    {
        xmlWriter->writeAttribute("name", "Trajectory" + QString::number(agentId));
        for (unsigned int i = 0; i < trajectory->GetTimeVec()->size(); i++)
        {
            xmlWriter->writeStartElement("Vertex");
            xmlWriter->writeAttribute("reference", QString::number(trajectory->GetTimeVec()->at(i)));
            xmlWriter->writeStartElement("Position");
            xmlWriter->writeStartElement("World");
            xmlWriter->writeAttribute("x", QString::number(trajectory->GetXPosVec()->at(i), 'g', 10));
            xmlWriter->writeAttribute("y", QString::number(trajectory->GetYPosVec()->at(i), 'g', 10));
            xmlWriter->writeAttribute("h", QString::number(trajectory->GetPsiVec()->at(i), 'g', 10));
            xmlWriter->writeEndElement();
            xmlWriter->writeEndElement();
            xmlWriter->writeStartElement("Shape");
            xmlWriter->writeEmptyElement("Polyline");
            xmlWriter->writeEndElement();
            xmlWriter->writeEndElement();
        }
    }
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();

    return true;
}
