/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "FileHelper.h"

bool FileHelper::CopyFiles(QString extension, QString sourceDirectory, QString targetDirectory)
{
    QStringList nameFilter("*" + extension);
    QDir sourceDir(sourceDirectory);
    QStringList fileList = sourceDir.entryList(nameFilter);

    int successAll = 0;
    for (QString fileEntity : fileList)
    {
        QFile file(fileEntity);
        bool success = file.copy(sourceDirectory + "/" + fileEntity,
                                 targetDirectory + "/" + fileEntity);
        if (!success)
        {
            successAll++;
        }
    }

    if (successAll == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool FileHelper::DeleteFiles(QString extension, QString sourceDirectory)
{
    QStringList nameFilter("*" + extension);
    QDir sourceDir(sourceDirectory);
    QStringList fileList = sourceDir.entryList(nameFilter);

    int successAll = 0;
    for (QString fileEntity : fileList)
    {
        QFile file(fileEntity);
        bool success = file.remove();
        if (!success)
        {
            successAll++;
        }
    }

    if (successAll == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
