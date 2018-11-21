/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef DATABASEREADER_H
#define DATABASEREADER_H

#include <QStringList>
#include <QtSql>
#include <iostream>

#include "pcm_participantData.h"
#include "pcm_initialValues.h"
#include "pcm_trajectory.h"
#include "pcm_marks.h"
#include "pcm_object.h"
#include "pcm_viewObject.h"
#include "pcm_intendedCourse.h"
#include "pcm_globalData.h"
#include "globalDefinitions.h"

class DatabaseReader
{
public:
    DatabaseReader();

    virtual ~DatabaseReader();

    bool SetDatabase(QString &dbName);

    bool OpenDataBase();
    bool CloseDataBase();
    bool IsDataBaseOpen();

    bool ReadCaseList(QStringList &caseList);

    bool ReadAllData(const QString &pcmCase,
                     std::vector<PCM_ParticipantData> &participants,
                     std::vector<PCM_InitialValues> &initials,
                     std::vector<PCM_Trajectory *> &trajectories,
                     std::vector<PCM_Marks *> &marksVec,
                     PCM_Object &object,
                     PCM_ViewObject &viewObject,
                     PCM_IntendedCourses &intendedCourses,
                     PCM_GlobalData &globalData);

    bool ReadParticipantData(const QString &pcmCase,
                             std::vector<PCM_ParticipantData> &participants);
    bool ReadDynamicsData(const QString &pcmCase,
                          std::vector<PCM_InitialValues> &initials);
    bool ReadTrajectoryData(const QString &pcmCase,
                            std::vector<PCM_Trajectory *> &trajectories);
    bool ReadMarksData(const QString &pcmCase,
                       std::vector<PCM_Marks *> &marksVec);
    bool ReadObjectsData(const QString &pcmCase,
                         PCM_Object &objects);
    bool ReadViewObjectsData(const QString &pcmCase,
                             PCM_ViewObject &viewObject);
    bool ReadIntendedCourseData(const QString &pcmCase,
                                PCM_IntendedCourses &intendedCources);
    bool ReadGlobalData(const QString &pcmCase,
                        PCM_GlobalData &globalData);

private:
    bool AddLineData(PCM_LineContainer *lineContainer,
                     int lineNo,
                     int pointNo,
                     double x,
                     double y,
                     double z);

    QString databaseName = "";
    QSqlDatabase db;
    QString connection;
};

#endif // DATABASEREADER_H
