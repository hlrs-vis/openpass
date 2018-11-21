/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "scenarioImporterPCM.h"

bool ScenarioImporterPCM::Import(const std::string &filename,
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
    if (documentRoot.tagName() != "OpenSCENARIO")
    {
        return false;
    }

    QDomNode storyBoardNode = documentRoot.firstChild();
    while (!storyBoardNode.isNull())
    {
        if (storyBoardNode.toElement().tagName() == "Storyboard")
        {
            QDomNode storyNode = storyBoardNode.firstChild();
            while (!storyNode.isNull())
            {
                if (storyNode.toElement().tagName() == "Story")
                {
                    int id = storyNode.toElement().attribute("owner").toInt();
                    QDomNode tempNode = storyNode.lastChild();
                    while (!tempNode.isNull())
                    {

                        if (tempNode.toElement().tagName() == "Trajectory")
                        {
                            if (!ParseTrajectories(id, tempNode, trajectories))
                            {
                                return false;
                            }
                        }
                        tempNode = tempNode.lastChild();
                    }
                }
                storyNode = storyNode.nextSibling();
            }
        }
        storyBoardNode = storyBoardNode.nextSibling();
    }

    return true;
}

bool ScenarioImporterPCM::ParseTrajectories(int id,
                                            QDomNode trajectoriesNode,
                                            std::map<int, PCM_Trajectory> &trajectories)
{
    if (trajectoriesNode.isNull())
    {
        return false;
    }

    std::vector<int> *timeVec = new std::vector<int>();
    std::vector<double> *xPosVec = new std::vector<double>();
    std::vector<double> *yPosVec = new std::vector<double>();
    std::vector<double> *xVelVec = new std::vector<double>();
    std::vector<double> *yVelVec = new std::vector<double>();
    std::vector<double> *angleVec = new std::vector<double>();

    QDomNode vertexNode = trajectoriesNode.firstChild();
    while (!vertexNode.isNull())
    {
        if (vertexNode.toElement().attribute("reference").toInt() == 0)
        {
            int time = vertexNode.toElement().attribute("reference").toInt();
            timeVec->push_back(time);
            QDomNode positionNode = vertexNode.firstChild();
            QDomNode worldNode = positionNode.firstChild();
            double x = worldNode.toElement().attribute("x").toDouble();
            xPosVec->push_back(x);
            double y = worldNode.toElement().attribute("y").toDouble();
            yPosVec->push_back(y);
            double h = worldNode.toElement().attribute("h").toDouble();
            angleVec->push_back(h);
        }
        if (vertexNode.toElement().attribute("reference").toInt() != 0)
        {
            int time = vertexNode.toElement().attribute("reference").toInt();
            timeVec->push_back(time);
            QDomNode positionNode = vertexNode.firstChild();
            QDomNode worldNode = positionNode.firstChild();
            double x = worldNode.toElement().attribute("x").toDouble();
            xPosVec->push_back(x);
            double y = worldNode.toElement().attribute("y").toDouble();
            yPosVec->push_back(y);
            double h = worldNode.toElement().attribute("h").toDouble();
            angleVec->push_back(h);
            // approximation for velocity
            double vel = sqrt(pow((xPosVec->end()[-2] - xPosVec->end()[-1]), 2)
                              + pow((yPosVec->end()[-2] - yPosVec->end()[-1]), 2))
                         / fabs(timeVec->end()[-2] - timeVec->end()[-1]) * 1000;

            xVelVec->push_back(vel);
        }
        vertexNode = vertexNode.nextSibling();
    }
    // copy last velocity value to fill velocity vector
    double lastVel = xVelVec->back();

    xVelVec->push_back(lastVel);

    auto it = trajectories.find(id);
    if (it != trajectories.end())
    {
        trajectories.erase(it);
    }

    trajectories.emplace(std::make_pair(id, PCM_Trajectory(timeVec,
                                                           xPosVec,
                                                           yPosVec,
                                                           xVelVec,
                                                           yVelVec,
                                                           angleVec)));

    return true;
}

