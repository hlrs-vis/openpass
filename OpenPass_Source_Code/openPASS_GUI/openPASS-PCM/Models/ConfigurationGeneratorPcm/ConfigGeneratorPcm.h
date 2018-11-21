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

class ConfigGenerator
{
public:
    ConfigGenerator();
    ConfigGenerator(const QString &baseFolder);
    virtual ~ConfigGenerator();

    bool GenerateConfigs(DatabaseReader &dbReader,
                         const QString &pcmCase,
                         const QString &configPath,
                         const QString &otherSystemFile,
                         const QString &car1SystemFile,
                         const QString &car2SystemFile,
                         const QString &resultFolderName);

    const QString GenerateFrameworkConfig(QString frameworkConfigPath);

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
};

#endif // CONFIGGENERATOR_H
