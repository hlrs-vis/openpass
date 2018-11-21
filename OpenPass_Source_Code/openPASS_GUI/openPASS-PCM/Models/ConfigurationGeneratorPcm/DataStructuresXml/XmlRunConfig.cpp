/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlRunConfig.h"

XmlRunConfig::XmlRunConfig(int startTime, QString endTime, int nInvocations, QString weekday,
                           int timeOfDay, int randomSeed):
    startTime(startTime), endTime(endTime), numberInvocations(nInvocations), weekday(weekday),
    timeOfDay(timeOfDay), randomSeed(randomSeed)
{}

XmlRunConfig::~XmlRunConfig()
{
    for ( XmlSpawnPoint *sp : spawnPoints)
    {
        delete sp;
    }

    for ( XmlObservation *observation : observations)
    {
        delete observation;
    }
}

void XmlRunConfig::AddSpawnPoint(XmlSpawnPoint *sp)
{
    spawnPoints.push_back(sp);
}

void XmlRunConfig::AddAgent(int id, int agentTypeRef, PCM_ParticipantData participant)
{
    agents.push_back( XmlAgent(id, agentTypeRef, participant) );
}

void XmlRunConfig::AddObservation(XmlObservation *observation)
{
    observations.push_back(observation);
}

bool XmlRunConfig::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeTextElement( "StartTime", QString::number(startTime) );
    xmlWriter->writeTextElement( "EndTime", endTime );
    xmlWriter->writeTextElement( "NumberInvocations", QString::number(numberInvocations) );
    xmlWriter->writeTextElement( "Weekday", weekday );
    xmlWriter->writeTextElement( "TimeOfDay", QString::number(timeOfDay) );
    xmlWriter->writeTextElement( "WorldLibrary", worldLibrary);
    xmlWriter->writeTextElement( "Stochastics", stochastics);
    xmlWriter->writeTextElement( "CollisionDetection", collisionDetection);
    xmlWriter->writeTextElement( "RandomSeed", QString::number(randomSeed) );

    xmlWriter->writeStartElement("SpawnPoints");
    for (XmlSpawnPoint *sp : spawnPoints)
    {
        if ( !sp->WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // SpawnPoints

    xmlWriter->writeStartElement("Agents");
    for (XmlAgent agent : agents)
    {
        if ( !agent.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // Agents

    xmlWriter->writeStartElement("Observations");
    for (XmlObservation *observation : observations)
    {
        if ( !observation->WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // Observations

    return true;
}
