/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>
#include <QFile>
#include <QDir>

class FileHelper
{
public:
    FileHelper() = delete;
    FileHelper(const FileHelper &) = delete;
    FileHelper(FileHelper &&) = delete;
    FileHelper &operator=(const FileHelper &) = delete;
    FileHelper &operator=(FileHelper &&) = delete;
    virtual ~FileHelper() = delete;

    static bool CopyFiles(QString extension, QString sourceDirectory, QString targetDirectory);
    static bool DeleteFiles(QString extension, QString sourceDirectory);
};

#endif // FILEHELPER_H
