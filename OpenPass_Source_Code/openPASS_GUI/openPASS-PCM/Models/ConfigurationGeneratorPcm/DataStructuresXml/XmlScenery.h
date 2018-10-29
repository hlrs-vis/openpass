/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLSCENERY_H
#define XMLSCENERY_H

#include "XmlMarks.h"
#include "XmlTrajectory.h"
#include "XmlObject.h"
#include "XmlViewObject.h"
#include "XmlIntendedCourses.h"
#include "XmlGlobalData.h"

class XmlScenery: public XmlBaseClass
{
public:
    XmlScenery() = default;
    bool WriteToXml( QXmlStreamWriter *xmlWriter );

    void AddMarks(PCM_Marks *marks);
    void AddObject(PCM_Object &object);
    void AddViewObject(PCM_ViewObject &viewObject);
    void AddTrajectory(int agentId, PCM_Trajectory *trajectory);
    void AddIntendedCourse(PCM_IntendedCourses &intendedCourse);
    void AddGlobalData(PCM_GlobalData &globalData);

private:
    std::vector<XmlMarks> marksVec;              //!< vector containing the information of PCM marks
    std::vector<XmlTrajectory> trajectories;     //!< vector of all trajectories of all agents
    XmlObject object;                            //!< containing information of PCM_Object
    XmlViewObject viewObject;                    //!< containing information of PCM_ViewObject
    XmlIntendedCourses intendedCourse;          //!< containing information of PCM_IntendedCourses
    XmlGlobalData globalData;                    //!< containing information of PCM_GlobalData
};


#endif // XMLSCENERY_H
