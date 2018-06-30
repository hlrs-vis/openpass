#include "XmlScenery.h"

bool XmlScenery::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("PCM");

    globalData.WriteToXml(xmlWriter);

    xmlWriter->writeStartElement("Marks");
    for (XmlMarks marks : marksVec)
    {
        marks.WriteToXml(xmlWriter);
    }
    xmlWriter->writeEndElement();

    xmlWriter->writeStartElement("Objects");
    object.WriteToXml(xmlWriter);
    xmlWriter->writeEndElement();

    xmlWriter->writeStartElement("ViewObjects");
    viewObject.WriteToXml(xmlWriter);
    xmlWriter->writeEndElement();

    intendedCourse.WriteToXml(xmlWriter);

    xmlWriter->writeStartElement("Trajectories");
    for (XmlTrajectory trajectory : trajectories)
    {
        trajectory.WriteToXml(xmlWriter);
    }
    xmlWriter->writeEndElement();

    xmlWriter->writeEndElement();

    return true;
}

void XmlScenery::AddMarks(PCM_Marks *marks)
{
    marksVec.push_back(XmlMarks(marks));
}

void XmlScenery::AddObject(PCM_Object &object)
{
    this->object = XmlObject(&object);
}

void XmlScenery::AddViewObject(PCM_ViewObject &viewObject)
{
    this->viewObject = XmlViewObject(&viewObject);
}

void XmlScenery::AddTrajectory(int agentId, PCM_Trajectory *trajectory)
{
    trajectories.push_back(XmlTrajectory(agentId, trajectory));
}

void XmlScenery::AddIntendedCourse(PCM_IntendedCourses &intendedCourse)
{
    this->intendedCourse = XmlIntendedCourses(&intendedCourse);
}

void XmlScenery::AddGlobalData(PCM_GlobalData &globalData)
{
    this->globalData = XmlGlobalData(&globalData);
}
