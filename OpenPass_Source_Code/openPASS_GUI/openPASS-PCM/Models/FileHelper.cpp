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

bool FileHelper::RemoveDirectory(QString targetDirectory)
{
    bool success = true;
    QDir targetDir(targetDirectory);

    if (targetDir.exists())
    {
        for (QFileInfo info : targetDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden
                                                      | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir())
            {
                success = RemoveDirectory(info.absoluteFilePath());
            }
            else
            {
                success = QFile::remove(info.absoluteFilePath());
            }

            if (!success)
            {
                return success;
            }
        }
        success = targetDir.rmdir(targetDirectory);
    }

    return success;
}
