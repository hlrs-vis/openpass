/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XoscScenario.h"

bool XoscScenario::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("OpenSCENARIO");

    xmlWriter->writeStartElement("FileHeader");
    xmlWriter->writeAttribute("description", "Example for OpenScenario");
    xmlWriter->writeAttribute("author", "ITK Engineering");
    xmlWriter->writeEndElement();

    xmlWriter->writeEmptyElement("ParameterDeclaration");

    xmlWriter->writeEmptyElement("Catalogs");

    xmlWriter->writeEmptyElement("RoadNetwork");

    xmlWriter->writeStartElement("Entities");
    xmlWriter->writeEndElement();

    xmlWriter->writeStartElement("Storyboard");

    xmlWriter->writeEmptyElement("Init");

    for (XoscTrajectory oscTrajectory : trajectories)
    {
        oscTrajectory.WriteToXml(xmlWriter);
    }
    xmlWriter->writeEndElement();

    xmlWriter->writeEndElement();

    return true;
}

void XoscScenario::AddTrajectory(int agentId,
                                 PCM_Trajectory *trajectory)
{
    trajectories.push_back(XoscTrajectory(agentId, trajectory));
}
