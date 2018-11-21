/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "sceneryImporterPCM.h"

bool SceneryImporterPCM::Import(const std::string &filename, PCM_Data &pcmData,
                                std::map<int, PCM_Trajectory> &trajectories)
{
    std::locale::global(std::locale("C"));

    QFile xmlFile(filename.c_str()); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    QString errorMsg;
    int errorLine;
    if (!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        return false;
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        return false;
    }
    if (documentRoot.tagName() != "PCM")
    {
        return false;
    }

    QDomNode tempNode = documentRoot.firstChild();
    while (!tempNode.isNull())
    {
        if (tempNode.toElement().tagName() == "global_data")
        {
            if (!ParseGlobalData(tempNode, pcmData))
            {
                return false;
            }
        }
        if (tempNode.toElement().tagName() == "Marks")
        {
            if (!ParseMarks(tempNode, pcmData))
            {
                return false;
            }
        }
        if (tempNode.toElement().tagName() == "Objects")
        {
            if (!ParseObjects(tempNode, pcmData))
            {
                return false;
            }
        }
        if (tempNode.toElement().tagName() == "ViewObjects")
        {
            if (!ParseViewObjects(tempNode, pcmData))
            {
                return false;
            }
        }
        if (tempNode.toElement().tagName() == "intended_course")
        {
            if (!ParseIntendedCourses(tempNode, pcmData))
            {
                return false;
            }
        }
        if (tempNode.toElement().tagName() == "Trajectories")
        {
            if (!ParseTrajectories(tempNode, trajectories))
            {
                return false;
            }
        }

        tempNode = tempNode.nextSibling();
    }

    return true;
}

bool SceneryImporterPCM::ParseGlobalData(QDomNode rootNode, PCM_Data &pcmData)
{
    if (rootNode.isNull())
    {
        return false;
    }

    PCM_GlobalData *globalData = new PCM_GlobalData();
    QDomNode globalDataNode = rootNode.firstChild();
    while (!globalDataNode.isNull())
    {
        if (globalDataNode.toElement().tagName() == "OffsetX")
        {
            double offsetX = globalDataNode.toElement().text().toDouble();
            globalData->SetOffsetX(offsetX);
        }
        else if (globalDataNode.toElement().tagName() == "OffsetY")
        {
            double offsetY = globalDataNode.toElement().text().toDouble();
            globalData->SetOffsetY(offsetY);
        }
        else if (globalDataNode.toElement().tagName() == "Participants")
        {
            int participants = globalDataNode.toElement().text().toInt();
            globalData->SetParticipants(participants);
        }
        else if (globalDataNode.toElement().tagName() == "SimulationVersion")
        {
            int simulationVersion = globalDataNode.toElement().text().toInt();
            globalData->SetSimulationVersion(simulationVersion);
        }
        globalDataNode = globalDataNode.nextSibling();
    }

    pcmData.SetPCM_GlobalData(globalData);

    return true;
}

bool SceneryImporterPCM::ParseMarks(QDomNode rootNode, PCM_Data &pcmData)
{
    if (rootNode.isNull())
    {
        return false;
    }

    for (int i = 1; i < static_cast<int>(MarkType::NumberOfMarkTypes); i++)
    {
        MarkType markType = static_cast<MarkType>(i);
        QDomNode marksNode = rootNode.firstChild();
        while (!marksNode.isNull())
        {
            if (marksNode.toElement().tagName().toStdString() == PCM_Helper::ConvertMarkTypeToDBString(
                        markType))
            {
                PCM_Marks *marks = new PCM_Marks(markType);

                if (!ParseLines(marksNode, marks))
                {
                    return false;
                }

                pcmData.AddPCM_Marks(marks);
            }
            marksNode = marksNode.nextSibling();
        }
    }
    return true;
}

bool SceneryImporterPCM::ParseObjects(QDomNode rootNode, PCM_Data &pcmData)
{
    if (rootNode.isNull())
    {
        return false;
    }

    QDomNode objectNode = rootNode.firstChild();
    while (!objectNode.isNull())
    {
        if (objectNode.toElement().tagName().toStdString() == "objects")
        {
            PCM_Object *object = new PCM_Object();

            if (!ParseLines(objectNode, object))
            {
                return false;
            }
            QDomNode objectTypeNode = objectNode.firstChild();
            while (!objectTypeNode.isNull())
            {
                if (objectTypeNode.toElement().tagName() == "ObjectType")
                {
                    int objectType = objectTypeNode.toElement().text().toInt();
                    object->SetObjectType(objectType);
                }
                objectTypeNode = objectTypeNode.nextSibling();
            }

            pcmData.SetPCM_Object(object);
        }
        objectNode = objectNode.nextSibling();
    }

    return true;
}

bool SceneryImporterPCM::ParseViewObjects(QDomNode rootNode, PCM_Data &pcmData)
{
    if (rootNode.isNull())
    {
        return false;
    }

    QDomNode viewObjectNode = rootNode.firstChild();
    while (!viewObjectNode.isNull())
    {
        if (viewObjectNode.toElement().tagName().toStdString() == "view_object")
        {
            PCM_ViewObject *viewObject = new PCM_ViewObject();

            if (!ParseLines(viewObjectNode, viewObject))
            {
                return false;
            }

            pcmData.SetPCM_ViewObject(viewObject);
        }
        viewObjectNode = viewObjectNode.nextSibling();
    }

    return true;
}

bool SceneryImporterPCM::ParseIntendedCourses(QDomNode rootNode, PCM_Data &pcmData)
{
    if (rootNode.isNull())
    {
        return false;
    }

    QDomNode courseNode = rootNode.firstChild();
    while (!courseNode.isNull())
    {
        if (courseNode.toElement().tagName().toStdString() == "course")
        {
            int betNr = courseNode.toElement().attribute("betNr").toInt();
            PCM_Course *course = new PCM_Course(betNr);

            if (!ParsePoints(courseNode, course))
            {
                return false;
            }

            pcmData.AddPCM_Course(course);
        }
        courseNode = courseNode.nextSibling();
    }

    return true;
}

bool SceneryImporterPCM::ParseLines(QDomNode lineContainerNode, PCM_LineContainer *lineContainer)
{
    if (lineContainerNode.isNull())
    {
        return false;
    }
    if (lineContainer == nullptr)
    {
        return false;
    }
    QDomNode lineNode = lineContainerNode.firstChild();
    while (!lineNode.isNull())
    {
        if (lineNode.toElement().tagName() == "line")
        {
            int lineId = lineNode.toElement().attribute("id").toInt();
            PCM_Line *line = new PCM_Line(lineId);

            if (!ParsePoints(lineNode, line))
            {
                return false;
            }

            lineContainer->AddPCM_Line(line);
        }
        lineNode = lineNode.nextSibling();
    }
    return true;
}

bool SceneryImporterPCM::ParsePoints(QDomNode pointContainerNode,
                                     PCM_PointContainer *pointContainer)
{
    if (pointContainerNode.isNull())
    {
        return false;
    }
    if (pointContainer == nullptr)
    {
        return false;
    }
    QDomNode pointNode = pointContainerNode.firstChild();
    while (!pointNode.isNull())
    {
        if (pointNode.toElement().tagName() == "point")
        {
            int pointId = pointNode.toElement().attribute("id").toInt();
            double x = 0, y = 0, z = 0;
            QDomNode coordinateNode = pointNode.firstChild();
            while (!coordinateNode.isNull())
            {
                if (coordinateNode.toElement().tagName() == "x")
                {
                    x = coordinateNode.toElement().text().toDouble();
                }
                else if (coordinateNode.toElement().tagName() == "y")
                {
                    y = coordinateNode.toElement().text().toDouble();
                }
                else if (coordinateNode.toElement().tagName() == "z")
                {
                    z = coordinateNode.toElement().text().toDouble();
                }
                coordinateNode = coordinateNode.nextSibling();
            }
            PCM_Point *point = new PCM_Point(pointId, x, y, z);
            pointContainer->AddPCM_Point(point);
        }
        pointNode = pointNode.nextSibling();
    }
    return true;
}

bool SceneryImporterPCM::ParseTrajectories(QDomNode trajectoriesNode,
                                           std::map<int, PCM_Trajectory> &trajectories)
{
    if (trajectoriesNode.isNull())
    {
        return false;
    }
    if (trajectoriesNode.toElement().text() == "")
    {
        return false;
    }

    QDomNode trajectoryNode = trajectoriesNode.firstChild();
    while (!trajectoryNode.isNull())
    {
        if (trajectoryNode.toElement().tagName() == "Trajectory")
        {
            int id = trajectoryNode.toElement().attribute("id").toInt();
            std::vector<int> *timeVec = new std::vector<int>();
            std::vector<double> *xPosVec = new std::vector<double>();
            std::vector<double> *yPosVec = new std::vector<double>();
            std::vector<double> *xVelVec = new std::vector<double>();
            std::vector<double> *yVelVec = new std::vector<double>();
            std::vector<double> *angleVec = new std::vector<double>();
            QDomNode tempNode = trajectoryNode.firstChild();
            while (!tempNode.isNull())
            {
                if (tempNode.toElement().tagName() == "Time")
                {
                    ParseIntVector(tempNode, timeVec);
                }
                else if (tempNode.toElement().tagName() == "XPos")
                {
                    ParseDoubleVector(tempNode, xPosVec);
                }
                else if (tempNode.toElement().tagName() == "YPos")
                {
                    ParseDoubleVector(tempNode, yPosVec);
                }
                else if (tempNode.toElement().tagName() == "VelocityX")
                {
                    ParseDoubleVector(tempNode, xVelVec);
                }
                else if (tempNode.toElement().tagName() == "VelocityY")
                {
                    ParseDoubleVector(tempNode, yVelVec);
                }
                else if (tempNode.toElement().tagName() == "Angle")
                {
                    ParseDoubleVector(tempNode, angleVec);
                }

                tempNode = tempNode.nextSibling();
            }

            trajectories.emplace(std::make_pair(id, PCM_Trajectory(timeVec,
                                                                   xPosVec,
                                                                   yPosVec,
                                                                   xVelVec,
                                                                   yVelVec,
                                                                   angleVec)));
        }
        trajectoryNode = trajectoryNode.nextSibling();
    }

    return true;
}

bool SceneryImporterPCM::ParseIntVector(QDomNode rootNode, std::vector<int> *intVector)
{
    if (rootNode.isNull())
    {
        return false;
    }
    if (rootNode.toElement().text() == "")
    {
        return false;
    }

    try
    {
        std::stringstream valueStream(rootNode.toElement().text().toStdString());

        int item;
        while (valueStream >> item)
        {
            intVector->push_back(item);

            if (valueStream.peek() == ',')
            {
                valueStream.ignore();
            }
        }
    }
    catch (...)
    {
        return false;
    }

    return true;

}

bool SceneryImporterPCM::ParseDoubleVector(QDomNode rootNode, std::vector<double> *doubleVector)
{
    if (rootNode.isNull())
    {
        return false;
    }
    if (rootNode.toElement().text() == "")
    {
        return false;
    }

    try
    {
        std::stringstream valueStream(rootNode.toElement().text().toStdString());

        double item;
        while (valueStream >> item)
        {
            doubleVector->push_back(item);

            if (valueStream.peek() == ',')
            {
                valueStream.ignore();
            }
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}
