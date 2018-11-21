/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XOSCTRAJECTORY_H
#define XOSCTRAJECTORY_H

#include "PCM_Data/pcm_trajectory.h"
#include "DataStructuresBase/XmlBaseClass.h"

class XoscTrajectory: public XmlBaseClass
{
public:

    XoscTrajectory(int agentId,
                   PCM_Trajectory *trajectory);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    int agentId;
    PCM_Trajectory *trajectory;              //!< PCM marks
};

#endif // XOSCTRAJECTORY_H
