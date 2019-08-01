/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "processManager.h"
#include <algorithm>

ProcessManager::ProcessManager(QObject* parent):
    QObject(parent)
{
    idealProcessCount = QThread::idealThreadCount();
}

void ProcessManager::RemoveProcess(QProcess* process)
{
    processMap.remove(process);
    delete process;
}

ProcessManager& ProcessManager::getInstance()
{
    static ProcessManager instance;
    return instance;
}

bool ProcessManager::StartProcess(const std::string& processPath,
                                  const std::list<std::pair<std::string, std::string>>& arguments)
{
    auto qProcessPath = QString::fromStdString(processPath);

    QStringList qArguments;
    for (const std::pair<std::string, std::string>& argument : arguments)
    {
        qArguments << QString::fromStdString(argument.first) << QString::fromStdString(argument.second);
    }

    while (processMap.size() > idealProcessCount)
    {
        processMap.begin().key()->waitForFinished(1);
    }

    QProcess* newProcess = new QProcess();
    newProcess->start(qProcessPath, qArguments);

    if (newProcess->processId() == 0)
    {
        LOG_INTERN(LogLevel::Error) << processPath << " not started, check path.";
        return false;
    }

    LOG_INTERN(LogLevel::DebugCore) << std::endl << "### process start pid: " <<
                                    QString::number(newProcess->processId()).toStdString() << "###";

    connect(newProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [this, newProcess](int exitCode, QProcess::ExitStatus exitStatus)
    {
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        RemoveProcess(newProcess);
    });

    processMap.insert(newProcess, newProcess->processId());

    return true;
}

void ProcessManager::WaitAndClear()
{
    QMapIterator<QProcess*, int> processMapIterator(processMap);
    while (processMapIterator.hasNext())
    {
        processMapIterator.next();
        QProcess* process = processMapIterator.key();
        process->waitForFinished(-1); //default timeout 30s, -1 :infinity
    }
}

void ProcessManager::KillAll()
{
    QMapIterator<QProcess*, int> processMapIterator(processMap);
    while (processMapIterator.hasNext())
    {
        processMapIterator.next();
        QProcess* process = processMapIterator.key();
        process->kill();
        RemoveProcess(process);
    }
}
