/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QMap>
#include "log.h"

class ProcessManager : public QObject
{
    Q_OBJECT
public:
    static ProcessManager &getInstance();

    ProcessManager(const ProcessManager &) = delete;
    ProcessManager(ProcessManager &&) = delete;
    ProcessManager &operator=(const ProcessManager &) = delete;
    ProcessManager &operator=(ProcessManager &&) = delete;
    virtual ~ProcessManager();

    bool StartProcess(QString &processPath, QStringList &arguments);
    void WaitAndClear();
    void KillAll();

signals:

public slots:

private:
    ProcessManager(QObject *parent = nullptr);

    void RemoveProcess(QProcess *process);

    int idealProcessCount;
    QMap<QProcess *, int> processMap;
};

#endif // PROCESSMANAGER_H
