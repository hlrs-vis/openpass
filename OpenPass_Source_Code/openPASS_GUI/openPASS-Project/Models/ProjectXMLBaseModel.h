/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef PROJECTXMLBASEMODEL_H
#define PROJECTXMLBASEMODEL_H

#include <QList>
#include <QString>

class ProjectXMLBaseModel
{
public:
    explicit ProjectXMLBaseModel() = delete;
    virtual ~ProjectXMLBaseModel() = default;

protected:
    static QString const KeyFrameworkConfiguration;
    static QString const KeyLibraryPath;
    static QString const KeyResultPath;
    static QString const KeyAgentConfigFile;
    static QString const KeyRunConfigFile;
    static QString const KeySceneryConfigFile;
    static QString const KeyLogFileMaster;
    static QString const KeyLogFileSlave;
    static QString const KeyLogLevel;
    static QString const KeySlavePath;

protected:
    static QList<QString> const KeyListFrameworkConfig;
};

#endif // PROJECTXMLBASEMODEL_H
