/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef CONFIGWRITER_H
#define CONFIGWRITER_H

#include <QDir>
#include "pcm_initialValues.h"
#include "DataStructuresXml/XmlAgentEquipment.h"
#include "DataStructuresXml/XmlRunConfig.h"
#include "DataStructuresXml/XmlScenery.h"
#include "XoscScenario.h"
#include "XmlMergeHelper.h"

class ConfigWriter
{
public:
    ConfigWriter();
    ConfigWriter(const QString &baseFolder);
    virtual ~ConfigWriter() = default;

    const QString CreateRunConfiguration(const QString &configPath,
                                         const QString &endTime,
                                         std::vector<PCM_ParticipantData> &participants,
                                         std::vector<PCM_InitialValues> &initials,
                                         std::vector<PCM_Trajectory *> &trajectories,
                                         const QString &supposedCollisionTime,
                                         const QString &resultFolderName,
                                         const QString &pcmCase,
                                         const int randomSeed);

    const QString CreateSystemConfiguration(const QString &configPath,
                                            const QString &otherSystemFile,
                                            const QString &car1SystemFile,
                                            const QString &car2SystemFile,
                                            std::vector<PCM_ParticipantData> &participants);

    const QString CreateSceneryConfiguration(const QString &configPath,
                                             std::vector<PCM_Marks *> &marksVec,
                                             std::vector<PCM_ParticipantData> &participants,
                                             std::vector<PCM_Trajectory *> &trajectories,
                                             PCM_Object &object,
                                             PCM_ViewObject &viewObject,
                                             PCM_IntendedCourses &intendedCourses,
                                             PCM_GlobalData &globalData);

    const QString CreateFrameworkConfiguration(const QString frameworkConfigPath,
                                               QList<QMap<QString, QString> > configList, const int logLevel);

private:
    const QString WriteRunConfiguration(XmlRunConfig &runConfig,
                                        const QString &configPath);
    const QString WriteSceneryConfiguration(XmlScenery &sceneryConfig,
                                            const QString &configPath);

    QString baseFolder = ".";
    QDir baseDirectory;
};

#endif // CONFIGWRITER_H
