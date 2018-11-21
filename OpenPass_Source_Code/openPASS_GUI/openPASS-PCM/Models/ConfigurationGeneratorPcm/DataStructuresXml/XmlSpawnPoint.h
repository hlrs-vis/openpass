/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
