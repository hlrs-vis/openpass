/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/


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
