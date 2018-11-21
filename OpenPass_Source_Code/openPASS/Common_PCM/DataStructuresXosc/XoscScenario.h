/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLSCENARIO_H
#define XMLSCENARIO_H

#include "XoscTrajectory.h"

class XoscScenario : public XmlBaseClass
{
public:
    XoscScenario() = default;
    virtual ~XoscScenario() = default;

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

    void AddTrajectory(int agentId,
                       PCM_Trajectory *trajectory);

private:
    std::vector<XoscTrajectory> trajectories;     //!< vector of all trajectories of all agents

};

#endif // XMLSCENARIO_H
