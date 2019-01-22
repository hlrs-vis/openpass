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
#include "runConfigImporterPCM.h"
#include "GUI_Definitions.h"

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

// generate 3 config files: systemConfiguration.xml, runConfiguration.xml, and sceneryConfiguration.xml
bool ConfigGenerator::GenerateConfigFromDB(DatabaseReader &dbReader,
                                      const QString &pcmCase,
                                      const QString &caseOutputFolder,
                                      const QString &otherSystemFile,
                                      const QString &car1SystemFile,
                                      const QString &car2SystemFile,
                                      const int randomSeed,
                                      const std::vector<double> &shiftRadiusVec,
                                      const std::vector<double> &velMaxScaleVec)
{
    if (pcmCase == "")
    {
        return false;
    }

    QString caseResultFolder = caseOutputFolder + "/" + DIRNAME_CASE_RESULTS;

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

    //modify the initials and trajectories by trajectory shifting and velocity scaling
    ApplyVariation(randomSeed, shiftRadiusVec, velMaxScaleVec, initials, trajectories);

    QString supposedCollisionTime;
    QString endTime;

    if (trajectories.size() > 0)
    {
        supposedCollisionTime = QString::number(trajectories[0]->GetEndTime());
        endTime = QString::number(supposedCollisionTime.toInt() * 2); //set endTime twice as supposedCollisionTime in order to allow simulation after collision
    }

    const QString systemConfigFile = configWriter->CreateSystemConfiguration(caseOutputFolder,
                                                                             otherSystemFile,
                                                                             car1SystemFile,
                                                                             car2SystemFile,
                                                                             participants);
    if (systemConfigFile == "")
    {
        return false;
    }

    const QString runConfigFile = configWriter->CreateRunConfiguration(caseOutputFolder,
                                                                       endTime,
                                                                       participants,
                                                                       initials,
                                                                       trajectories,
                                                                       supposedCollisionTime,
                                                                       caseResultFolder,
                                                                       pcmCase,
                                                                       randomSeed);
    if (runConfigFile == "")
    {
        return false;
    }

    const QString sceneryConfigFile = configWriter->CreateSceneryConfiguration(caseOutputFolder,
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

    QString scenarioFile = caseOutputFolder + "/" + FILENAME_SCENARIO_CONIG;
    OpenScenarioWriter openScenarioWriter;
    const QString scenarioConfigFile = openScenarioWriter.CreateScenarioFile(scenarioFile,
                                                                             trajectories);

    if (scenarioConfigFile == "")
    {
        return false;
    }

    AddConfigSet(caseResultFolder, systemConfigFile, runConfigFile, sceneryConfigFile,
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

// generate 3 config files: systemConfiguration.xml, runConfiguration.xml, and sceneryConfiguration.xml
bool ConfigGenerator::GenerateConfigFromPrevResult(const QString &prevCaseOutputFolder,
                     const QString &caseOutputFolder,
                     const QString &otherSystemFile,
                     const QString &car1SystemFile,
                     const QString &car2SystemFile,
                     const int randomSeed,
                     const std::vector<double> &shiftRadiusVec,
                     const std::vector<double> &velMaxScaleVec)
{
    QString caseResultFolder = caseOutputFolder + "/" + DIRNAME_CASE_RESULTS;

    // get participants data from runConfiguration.xml of previous results
    QString prevRunConfigFilePath = prevCaseOutputFolder + '/' + FILENAME_RUN_CONIG;

    RunConfigImporterPCM* runConfigImporter = new RunConfigImporterPCM();
    if(!runConfigImporter->Import(prevRunConfigFilePath))
    {
        return false;
    }

    std::vector<PCM_InitialValues> initials = runConfigImporter->GetInitials();
    std::vector<PCM_ParticipantData> participants = runConfigImporter->GetParticipants();
    std::vector<PCM_Trajectory *> trajectories = runConfigImporter->GetTrajectories();

    //modify the initials and trajectories by trajectory shifting and velocity scaling
    ApplyVariation(randomSeed, shiftRadiusVec, velMaxScaleVec, initials, trajectories);

    QString supposedCollisionTime;
    QString endTime;

    if (trajectories.size() > 0)
    {
        supposedCollisionTime = QString::number(trajectories.at(0)->GetEndTime());
        endTime = QString::number(supposedCollisionTime.toInt() * 2); //set endTime twice as supposedCollisionTime in order to allow simulation after collision
    }

    // generate run config file because the caseResultFolder is different from previous result folder
    const QString runConfigFilePath = configWriter->CreateRunConfiguration(caseOutputFolder, endTime,
                                                                       participants, initials, trajectories,
                                                                       supposedCollisionTime, caseResultFolder,
                                                                       runConfigImporter->GetCaseNumber(),
                                                                       randomSeed);
    if (runConfigFilePath == "")
    {
        return false;
    }

    // generate system config file
    const QString systemConfigFile = configWriter->CreateSystemConfiguration(caseOutputFolder,
                                                                             otherSystemFile,
                                                                             car1SystemFile,
                                                                             car2SystemFile,
                                                                             participants);
    if (systemConfigFile == "")
    {
        return false;
    }

    // copy scenery config files from the previous result folder to the new result folder
    QString prevSceneryConfigFilePath = prevCaseOutputFolder + '/' + FILENAME_SCENERY_CONIG;
    QString sceneryConfigFilePath = caseOutputFolder + '/' + FILENAME_SCENERY_CONIG;
    QFile::copy(prevSceneryConfigFilePath, sceneryConfigFilePath);

    QString prevScenarioConfigFilePath = prevCaseOutputFolder + '/' + FILENAME_SCENARIO_CONIG;
    QString scenarioConfigFilePath = caseOutputFolder + '/' + FILENAME_SCENARIO_CONIG;
    QFile::copy(prevScenarioConfigFilePath, scenarioConfigFilePath);

    AddConfigSet(caseResultFolder, systemConfigFile, runConfigFilePath,
                 sceneryConfigFilePath, scenarioConfigFilePath);

    for (PCM_Trajectory *trajectory : trajectories)
    {
        trajectory->Clear();
        delete trajectory;
    }

    delete runConfigImporter;
    return true;
}

const QString ConfigGenerator::GenerateFrameworkConfig(const QString frameworkConfigPath, const int logLevel)
{
    return configWriter->CreateFrameworkConfiguration(frameworkConfigPath, configSetList, logLevel);
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
    configSet.insert("LogFileSlave" , resultFolderName + "/" + FILENAME_OPENPASSSLAVE_LOG);

    configSetList.append(configSet);
}

void ConfigGenerator::Clear()
{
    configSetList.clear();
}

void ConfigGenerator::CalculateShiftPosition(const double shiftRadius, StochasticsPCM &stochastics, double &deltaX, double &deltaY)
{
    double shiftDistance = stochastics.GetUniformDistributed(0, shiftRadius);
    double shiftAngle = stochastics.GetUniformDistributed(0, M_PI * 2);
    deltaX = std::cos(shiftAngle) * shiftDistance;
    deltaY = std::sin(shiftAngle) * shiftDistance;
}

void ConfigGenerator::CalculateVelocityScale(const double velocityMaxScale, StochasticsPCM &stochastics, double &velocityScale)
{
    double min = 1.0 - velocityMaxScale/100;
    double max = 1.0 + velocityMaxScale/100;
    velocityScale = stochastics.GetUniformDistributed(min, max);
}

void ConfigGenerator::ShiftTrajectory(const double deltaX, const double deltaY, PCM_InitialValues &initValues, PCM_Trajectory * &trajectory)
{
    // shift initial positions
    double xpos = initValues.GetXpos().toDouble();
    initValues.SetXpos(QString::number(xpos+deltaX, 'g', initValues.GetXpos().length()-1)); // set precision the same as the original value

    double ypos = initValues.GetYpos().toDouble();
    initValues.SetYpos(QString::number(ypos+deltaY, 'g', initValues.GetYpos().length()-1)); // set precision the same as the original value

    // shift trajectory
    const std::vector<double> * xposVecOld = trajectory->GetXPosVec();
    std::vector<double> * xposVecNew = new std::vector<double>;
    for(double xpos: (* xposVecOld))
    {
        xposVecNew->push_back(xpos + deltaX);
    }

    const std::vector<double> * yposVecOld = trajectory->GetYPosVec();
    std::vector<double> * yposVecNew = new std::vector<double>;
    for(double ypos: (* yposVecOld))
    {
        yposVecNew->push_back(ypos + deltaY);
    }

    trajectory = new PCM_Trajectory(trajectory->GetTimeVec(),
                                    xposVecNew, yposVecNew,
                                    trajectory->GetUVelVec(),
                                    trajectory->GetVVelVec(),
                                    trajectory->GetPsiVec());
    delete xposVecOld;
    delete yposVecOld;
}

void ConfigGenerator::ScaleVelocity(const double velScale, PCM_InitialValues &initValues, PCM_Trajectory * &trajectory)
{
    // scale initial velocity
    double vx = initValues.GetVx().toDouble();
    initValues.SetVx(QString::number(vx * velScale, 'g', initValues.GetVx().length()-1)); // set precision the same as the original value

    double vy = initValues.GetVy().toDouble();
    initValues.SetVy(QString::number(vy * velScale, 'g', initValues.GetVy().length()-1)); // set precision the same as the original value

    // change trajectory by scaling velocity
    // first calculate new X-positions
    const std::vector<double> * xposVecOld = trajectory->GetXPosVec();
    double prevXpos = xposVecOld->at(0);

    std::vector<double> * xposVecNew = new std::vector<double>;
    xposVecNew->push_back(prevXpos);

    if(xposVecOld->size() >1)
    {
        std::vector<double> * vxVec = new std::vector<double>;
        for (std::vector<double>::const_iterator it = xposVecOld->begin()+1; it != xposVecOld->end(); ++it)
        {
            vxVec->push_back((*it) - (* (it-1)));
        }

        double xpos;
        for(double vx: (* vxVec))
        {
            xpos = prevXpos + vx;
            xposVecNew->push_back(xpos);
            prevXpos = xpos;
        }
    }

    // calculate new Y-positions
    const std::vector<double> * yposVecOld = trajectory->GetYPosVec();
    double prevYpos = yposVecOld->at(0);

    std::vector<double> * yposVecNew = new std::vector<double>;
    yposVecNew->push_back(prevYpos);

    if(yposVecOld->size() >1)
    {
        std::vector<double> * vyVec = new std::vector<double>;
        for (std::vector<double>::const_iterator it = yposVecOld->begin()+1; it != yposVecOld->end(); ++it)
        {
            vyVec->push_back((*it) - (* (it-1)));
        }

        double ypos;
        for(double vy: (* vyVec))
        {
            ypos = prevYpos + vy;
            yposVecNew->push_back(ypos);
            prevYpos = ypos;
        }
    }

    // calculate new longitudinal velocity
    const std::vector<double> * uVelVecOld = trajectory->GetUVelVec();
    std::vector<double> * uVelVecNew = new std::vector<double>;
    for(double uVel: (* uVelVecOld))
    {
        uVelVecNew->push_back(uVel * velScale);
    }

    // calculate new transversal velocity
    const std::vector<double> * vVelVecOld = trajectory->GetVVelVec();
    std::vector<double> * vVelVecNew = new std::vector<double>;
    for(double vVel: (* vVelVecOld))
    {
        vVelVecNew->push_back(vVel * velScale);
    }

    trajectory = new PCM_Trajectory(trajectory->GetTimeVec(),
                                    xposVecNew, yposVecNew,
                                    uVelVecNew, vVelVecNew,
                                    trajectory->GetPsiVec());
    delete xposVecOld;
    delete yposVecOld;
    delete uVelVecOld;
    delete vVelVecOld;
}

void ConfigGenerator::ApplyVariation(const int randomSeed, const std::vector<double> &shiftRadiusVec, const std::vector<double> &velMaxScaleVec,
                                    std::vector<PCM_InitialValues> &initials, std::vector<PCM_Trajectory *> &trajectories)
{
    if(!((shiftRadiusVec.size() == velMaxScaleVec.size())
            && (velMaxScaleVec.size() == initials.size())
            && (initials.size() == trajectories.size())))
    {
        qFatal("Error: inconsistent inputs while appling variations");
        return;
    }

    StochasticsPCM stochastics((std::uint32_t)randomSeed);
    double shiftRadius, deltaX, deltaY, velMaxScale, velScale;

    for(std::vector<int>::size_type i = 0; i != shiftRadiusVec.size(); i++)
    {
        shiftRadius = shiftRadiusVec.at(i);
        if(shiftRadius > 0)
        {
            CalculateShiftPosition(shiftRadius, stochastics, deltaX, deltaY);
            qDebug("ApplyVariation: shift car1 with radius of %.2f and random seed %d: (%.2f, %.2f)", shiftRadius, randomSeed, deltaX, deltaY);
            ShiftTrajectory(deltaX, deltaY, initials.at(i), trajectories.at(i));
        }
    }

    for(std::vector<int>::size_type i = 0; i != velMaxScaleVec.size(); i++)
    {
        velMaxScale = velMaxScaleVec.at(i);
        if((!qFuzzyIsNull(velMaxScale)) && (velMaxScale != INFINITY))
        {
            CalculateVelocityScale(velMaxScale, stochastics, velScale);
            qDebug("ApplyVariation: scale velocity 1 by %.4f (max %.2f) and random seed %d", velScale, velMaxScale, randomSeed);
            ScaleVelocity(velScale, initials.at(i), trajectories.at(i));
        }
    }
}
