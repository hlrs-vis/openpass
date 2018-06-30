#include "XmlAgent.h"

XmlAgent::XmlAgent(int id, int agentTypeRef, PCM_ParticipantData participant):
    XmlBaseClass(id),
    agentTypeRef(agentTypeRef),
    type(participant.GetType()),
    width(participant.GetWidth()),
    length(participant.GetLength()),
    distanceCOGToFrontAxle(participant.GetDistcgfa()),
    weight(participant.GetWeight()),
    heightCOG(participant.GetHeightcg()),
    wheelbase(participant.GetWheelbase()),
    momentInertiaRoll(participant.GetIxx()),
    momentInertiaPitch(participant.GetIyy()),
    momentInertiaYaw(participant.GetIzz()),
    frictionCoeff(participant.GetMue()),
    trackWidth(participant.GetTrackwidth()),
    distCOGtoLeadingEdge(participant.GetCgfront())
{}

bool XmlAgent::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Agent");
    xmlWriter->writeAttribute( "id", QString::number(_id) );

    xmlWriter->writeTextElement("AgentTypeRef", QString::number(agentTypeRef));
    xmlWriter->writeTextElement("Type", type);
    xmlWriter->writeTextElement("Width", width);
    xmlWriter->writeTextElement("Length", length);
    xmlWriter->writeTextElement("DistanceCOGtoFrontAxle", distanceCOGToFrontAxle);
    xmlWriter->writeTextElement("Weight", weight);
    xmlWriter->writeTextElement("HeightCOG", heightCOG);
    xmlWriter->writeTextElement("Wheelbase", wheelbase);
    xmlWriter->writeTextElement("MomentInertiaRoll", momentInertiaRoll);
    xmlWriter->writeTextElement("MomentInertiaPitch", momentInertiaPitch );
    xmlWriter->writeTextElement("MomentInertiaYaw", momentInertiaYaw);
    xmlWriter->writeTextElement("FrictionCoeff", frictionCoeff);
    xmlWriter->writeTextElement("TrackWidth", trackWidth);
    xmlWriter->writeTextElement("DistanceCOGtoLeadingEdge", distCOGtoLeadingEdge);

    xmlWriter->writeEndElement(); // Agent

    return true;
}
