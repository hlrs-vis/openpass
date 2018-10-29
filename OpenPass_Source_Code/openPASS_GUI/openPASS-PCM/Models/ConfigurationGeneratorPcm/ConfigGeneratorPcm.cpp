/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "ConfigGeneratorPcm.h"

ConfigGenerator::ConfigGenerator():
    ConfigGenerator(baseFolder)
{

}

ConfigGenerator::ConfigGenerator(const QString &baseFolder)
{
    configWriter = new ConfigWriter(baseFolder);
}

ConfigGenerator::~ConfigGenerator()
{
    delete configWriter;
}

bool ConfigGenerator::GenerateConfigs(DatabaseReader &dbReader,
                                      const QString &pcmCase,
                                      const QString &configPath,
                                      const QString &otherSystemFile,
                                      const QString &car1SystemFile,
                                      const QString &car2SystemFile,
                                      const QString &resultFolderName)
{
    if (pcmCase == "")
    {
        return false;
    }

    std::vector<PCM_ParticipantData> participants;
    std::vector<PCM_InitialValues> initials;
    std::vector<PCM_Trajectory *> trajectories;
    std::vector<PCM_Marks *> marksVec;
    PCM_Object object;
    PCM_ViewObject viewObject;
    PCM_IntendedCourses intendedCourses;
    PCM_GlobalData globalData;

    if (!dbReader.IsDataBaseOpen())
    {
        bool success = dbReader.OpenDataBase();
        if (!success)
        {
            return false;
        }
    }

    if (!dbReader.ReadAllData(pcmCase,
                              participants,
                              initials,
                              trajectories,
                              marksVec,
                              object,
                              viewObject,
                              intendedCourses,
                              globalData))
    {
        return false;
    }

    QString supposedCollisionTime;
    QString endTime;

    if (trajectories.size() > 0)
    {
        supposedCollisionTime = QString::number(trajectories[0]->GetEndTime());
        endTime = QString::number(supposedCollisionTime.toInt() * 2); //set endTime twice as supposedCollisionTime in order to allow simulation after collision
    }

    const QString systemConfigFile = configWriter->CreateSystemConfiguration(configPath,
                                                                             otherSystemFile,
                                                                             car1SystemFile,
                                                                             car2SystemFile,
                                                                             participants);
    if (systemConfigFile == "")
    {
        return false;
    }

    const QString runConfigFile = configWriter->CreateRunConfiguration(configPath,
                                                                       endTime,
                                                                       participants,
                                                                       initials,
                                                                       trajectories,
                                                                       supposedCollisionTime,
                                                                       resultFolderName,
                                                                       pcmCase);
    if (runConfigFile == "")
    {
        return false;
    }

    const QString sceneryConfigFile = configWriter->CreateSceneryConfiguration(configPath,
                                                                               marksVec,
                                                                               participants,
                                                                               trajectories,
                                                                               object,
                                                                               viewObject,
                                                                               intendedCourses,
                                                                               globalData);
    if (sceneryConfigFile == "")
    {
        return false;
    }

    QString scenarioFile = configPath + "/scenarioConfiguration.xosc";
    OpenScenarioWriter openScenarioWriter;
    const QString scenarioConfigFile = openScenarioWriter.CreateScenarioFile(scenarioFile,
                                                                             trajectories);

    if (scenarioConfigFile == "")
    {
        return false;
    }

    AddConfigSet(resultFolderName, systemConfigFile, runConfigFile, sceneryConfigFile,
                 scenarioConfigFile);

    dbReader.CloseDataBase();

    for (PCM_Trajectory *trajectory : trajectories)
    {
        trajectory->Clear();
        delete trajectory;
    }

    for (PCM_Marks *marks : marksVec)
    {
        delete marks;
    }

    return true;
}

const QString ConfigGenerator::GenerateFrameworkConfig(QString frameworkConfigPath)
{
    return configWriter->CreateFrameworkConfiguration(frameworkConfigPath, configSetList);
}

void ConfigGenerator::AddConfigSet(QString resultFolderName, QString systemConfig,
                                   QString runConfig, QString sceneryConfig, QString scenarioConfig)
{
    QMap<QString, QString> configSet;
    configSet.insert("ResultPath", resultFolderName);
    configSet.insert("AgentConfigFile", systemConfig);
    configSet.insert("RunConfigFile", runConfig);
    configSet.insert("SceneryConfigFile", sceneryConfig);
    configSet.insert("ScenarioConfigFile", scenarioConfig);
    configSet.insert("LogFileSlave" , resultFolderName + "/openPassSlave.log");

    configSetList.append(configSet);
}

void ConfigGenerator::Clear()
{
    configSetList.clear();
}
