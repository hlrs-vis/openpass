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
