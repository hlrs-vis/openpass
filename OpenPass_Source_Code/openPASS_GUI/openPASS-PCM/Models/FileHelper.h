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
    static bool RemoveDirectory(QString targetDirectory);
};

#endif // FILEHELPER_H
