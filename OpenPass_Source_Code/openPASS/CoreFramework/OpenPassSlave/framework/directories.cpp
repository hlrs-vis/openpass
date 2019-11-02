/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "directories.h"
#include <QDir>

Directories::Directories(const std::string& applicationDir,
                         const std::string& libraryDir,
                         const std::string& configurationDir,
                         const std::string& outputDir):
    baseDir{Directories::Resolve(applicationDir, ".")},
    configurationDir{Directories::Resolve(applicationDir, configurationDir)},
    libraryDir{Directories::Resolve(applicationDir, libraryDir)},
    outputDir{Directories::Resolve(applicationDir, outputDir)}
{}

const std::string Directories::Resolve(const std::string& applicationPath, const std::string& path)
{
    auto qpath = QString::fromStdString(path);

    return QDir::isRelativePath(qpath) ?
           QDir(QString::fromStdString(applicationPath) + QDir::separator() + qpath).absolutePath().toStdString() :
           QDir(qpath).absolutePath().toStdString();
}

const std::string Directories::Concat(const std::string& path, const std::string& file)
{
    return QDir(QString::fromStdString(path) +
                QDir::separator() +
                QString::fromStdString(file)).absolutePath().toStdString();
}
