/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCONNECTIONITEMINTERFACE_H
#define SYSTEMCONNECTIONITEMINTERFACE_H

#include "openPASS-System/SystemComponentInputItemInterface.h"
#include "openPASS-System/SystemComponentOutputItemInterface.h"
#include <QObject>

class SystemConnectionItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = unsigned int;
    using Source = SystemComponentOutputItemInterface;
    using Target = SystemComponentInputItemInterface;

public:
    explicit SystemConnectionItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemConnectionItemInterface() = default;

public:
    virtual SystemConnectionItemInterface::ID getID() const = 0;
    virtual SystemConnectionItemInterface::Source const * getSource() const = 0;
    virtual SystemConnectionItemInterface::Target const * getTarget() const = 0;
    virtual bool isCompatible() const = 0;
};

#endif // SYSTEMCONNECTIONITEMINTERFACE_H
