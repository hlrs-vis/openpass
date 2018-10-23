#include "XmlSpawnPoint.h"

XmlSpawnPoint::XmlSpawnPoint( int id, QString library ):
    XmlBaseClass(id), library(library)
{}

void XmlSpawnPoint::AddAgentRef(int agentRef)
{
    agentRefs.push_back(agentRef);
}

void XmlSpawnPoint::AddParameter(int id, XML_PARAMETER_TYPE type, QString name, QString value)
{
    spawnPointParameters.push_back( XmlParameter( id, type, name, value ) );
}

bool XmlSpawnPoint::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("SpawnPoint");
    xmlWriter->writeAttribute( "id", QString::number(_id) );
    xmlWriter->writeAttribute( "library", library );

    xmlWriter->writeStartElement("AgentRefs");
    for (int agentRef : agentRefs)
    {
        xmlWriter->writeTextElement("AgentRef", QString::number(agentRef));
    }
    xmlWriter->writeEndElement(); // AgentRefs

    xmlWriter->writeStartElement("SpawnPointParameters");
    for (XmlParameter parameter : spawnPointParameters)
    {
        if ( !parameter.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // SpawnPointParameters

    xmlWriter->writeEndElement(); // SpawnPoint

    return true;
}
