/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLAGENTEQUIPMENT_H
#define XMLAGENTEQUIPMENT_H

#include "XmlComponent.h"

class XmlAgentEquipment: public XmlBaseClass
{
public:
    XmlAgentEquipment(int id, int priority, int nChannels);
    XmlAgentEquipment(int id, int priority, std::vector<int> channelIds);

    ~XmlAgentEquipment();

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

    void AddComponent( XmlComponent *component );

private:
    int priority = -1;                     //!< priority of the agent

    std::vector<int> channelIds;           //!< the list of channel ids of the agent
    std::vector<XmlComponent *> components; //!< vector of all components
};

#endif // XMLAGENTEQUIPMENT_H
