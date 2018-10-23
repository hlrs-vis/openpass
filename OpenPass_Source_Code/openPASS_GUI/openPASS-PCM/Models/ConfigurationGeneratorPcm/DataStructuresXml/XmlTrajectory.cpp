#include "XmlTrajectory.h"

XmlTrajectory::XmlTrajectory(int agentId, PCM_Trajectory *trajectory)
{
    this->agentId = agentId;
    this->trajectory = trajectory;
}

bool XmlTrajectory::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Trajectory");

    if (trajectory != nullptr)
    {
        xmlWriter->writeAttribute("id", QString::number(agentId));
        xmlWriter->writeTextElement("Time", trajectory->GetTimeVecString());
        xmlWriter->writeTextElement("XPos", trajectory->GetXPosVecString());
        xmlWriter->writeTextElement("YPos", trajectory->GetYPosVecString());
        xmlWriter->writeTextElement("VelocityX", trajectory->GetUVelVecString());
        xmlWriter->writeTextElement("VelocityY", trajectory->GetVVelVecString());
        xmlWriter->writeTextElement("Angle", trajectory->GetPsiVecString());
    }

    xmlWriter->writeEndElement();

    return true;
}
