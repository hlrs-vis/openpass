/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef CONFIGGENERATOR_H
#define CONFIGGENERATOR_H

#include "DatabaseReaderPcm.h"
#include "ConfigWriter.h"
#include "openScenarioWriter.h"
#include "StochasticsPCM.h"

class ConfigGenerator
{
public:
    ConfigGenerator();
    ConfigGenerator(const QString &baseFolder);
    virtual ~ConfigGenerator();

    bool GenerateConfigFromDB(DatabaseReader &dbReader,
                         const QString &pcmCase,
                         const QString &caseOutputFolder,
                         const QString &otherSystemFile,
                         const QString &car1SystemFile,
                         const QString &car2SystemFile,
                         const int randomSeed,
                         const std::vector<double> &shiftRadiusVec,
                         const std::vector<double> &velMaxScaleVec);

    bool GenerateConfigFromPrevResult(const QString &prevCaseResultFolder,
                         const QString &caseOutputFolder,
                         const QString &otherSystemFile,
                         const QString &car1SystemFile,
                         const QString &car2SystemFile,
                         const int randomSeed,
                         const std::vector<double> &shiftRadiusVec,
                         const std::vector<double> &velMaxScaleVec);

    const QString GenerateFrameworkConfig(const QString frameworkConfigPath, const int logLevel);

    void AddConfigSet(QString resultFolderName,
                      QString systemConfig,
                      QString runConfig,
                      QString sceneryConfig,
                      QString scenarioConfig);

    void Clear();

private:
    ConfigWriter *configWriter;
    QString baseFolder = ".";

    QList<QMap<QString, QString>> configSetList;

    void CalculateShiftPosition(const double shiftRadius, StochasticsPCM &stochastics, double &deltaX, double &deltaY);
    void CalculateVelocityScale(const double velocityMaxScale, StochasticsPCM &stochastics, double &velocityScale);

    void ShiftTrajectory(const double deltaX, const double deltaY, PCM_InitialValues &initValues, PCM_Trajectory * &trajectory);
    void ScaleVelocity(const double velScale, PCM_InitialValues &initValues, PCM_Trajectory * &trajectory);

    void ApplyVariation(const int randomSeed, const std::vector<double> &shiftRadiusVec, const std::vector<double> &velMaxScaleVec,
                       std::vector<PCM_InitialValues> &initials, std::vector<PCM_Trajectory *> &trajectories);
};

#endif // CONFIGGENERATOR_H
