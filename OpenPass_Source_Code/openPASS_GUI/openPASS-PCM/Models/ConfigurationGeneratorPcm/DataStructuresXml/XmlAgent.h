#ifndef XMLAGENT_H
#define XMLAGENT_H

#include "pcm_participantData.h"
#include "XmlBaseClass.h"

class XmlAgent: public XmlBaseClass
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     id               The id of the agent
    //! @param[in]     agentTypeRef     The reference to the agent equipment
    //! @param[in]     participant      The information on the agent read from the PCM
    //-----------------------------------------------------------------------------
    XmlAgent( int id, int agentTypeRef, PCM_ParticipantData participant );

    //-----------------------------------------------------------------------------
    //! Function writing all information of the class onto a stream of an xml
    //! writer
    //!
    //! @param[in]     xmlWriter        The stream the information is to be put on
    //-----------------------------------------------------------------------------
    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    int     agentTypeRef = -1;         //!< reference to the agent equipment
    QString type;                      //!< type of the agent
    QString width;                     //!< width of the agent
    QString length;                    //!< length of the agent
    QString distanceCOGToFrontAxle;    //!< distance center of gravity to front axle of the agent
    QString weight;                    //!< weight of the agent
    QString heightCOG;                 //!< height of the center of gravity of the agent
    QString wheelbase;                 //!< wheelbase of the agent
    QString momentInertiaRoll;         //!< inertia against rolling
    QString momentInertiaPitch;        //!< inertia against pitching
    QString momentInertiaYaw;          //!< inertia against yawing
    QString frictionCoeff;             //!< friction coefficient that holds for the agent
    QString trackWidth;                //!< track width of the agent
    QString distCOGtoLeadingEdge;      //!< distance center of gravity to the leading edge of the agent
};


#endif // XMLAGENT_H
