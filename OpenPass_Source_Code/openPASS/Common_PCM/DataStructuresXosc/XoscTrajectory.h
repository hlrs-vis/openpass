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
