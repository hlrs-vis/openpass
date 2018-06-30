#ifndef XMLTRAJECTORY_H
#define XMLTRAJECTORY_H

#include "pcm_trajectory.h"
#include "XmlBaseClass.h"

class XmlTrajectory: public XmlBaseClass
{
public:

    XmlTrajectory(int agentId, PCM_Trajectory *trajectory);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    int agentId;
    PCM_Trajectory *trajectory;              //!< PCM Trajectory
};


#endif // XMLTRAJECTORY_H
