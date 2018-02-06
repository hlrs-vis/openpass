#ifndef XMLSPAWNPOINT_H
#define XMLSPAWNPOINT_H

#include "XmlParameter.h"

class XmlSpawnPoint: public XmlBaseClass
{
public:
    XmlSpawnPoint( int id, QString library );

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

    void AddAgentRef( int agentRef );
    void AddParameter( int id, XML_PARAMETER_TYPE type, QString name, QString value );

private:
    QString library;               //!< library of the spawn point
    std::vector<int> agentRefs;    //!< agent reference
    std::vector<XmlParameter> spawnPointParameters;    //!< vector of all parameters of the spawn point
};

#endif // XMLSPAWNPOINT_H
