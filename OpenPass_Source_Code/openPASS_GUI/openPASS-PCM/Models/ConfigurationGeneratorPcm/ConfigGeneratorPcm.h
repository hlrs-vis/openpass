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
