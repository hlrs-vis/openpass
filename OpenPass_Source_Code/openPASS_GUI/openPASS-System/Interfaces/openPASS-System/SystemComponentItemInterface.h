/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTITEMINTERFACE_H
#define SYSTEMCOMPONENTITEMINTERFACE_H

#include "openPASS-Component/ComponentItemInterface.h"
#include "openPASS-System/SystemComponentInputMapInterface.h"
#include "openPASS-System/SystemComponentOutputMapInterface.h"
#include "openPASS-System/SystemComponentParameterMapInterface.h"
#include "openPASS-System/SystemComponentScheduleInterface.h"
#include <QObject>
#include <QPoint>

class SystemComponentItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = unsigned int;
    using Library = ComponentItemInterface::Name;
    using Type = ComponentItemInterface::Type;
    using Title = ComponentItemInterface::Title;
    using Schedule = SystemComponentScheduleInterface;
    using ParameterMap = SystemComponentParameterMapInterface;
    using InputMap = SystemComponentInputMapInterface;
    using OutputMap = SystemComponentOutputMapInterface;
    using Position = QPoint;

public:
    explicit SystemComponentItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentItemInterface() = default;

Q_SIGNALS:
    void modifiedTitle() const;
    void modifiedPosition() const;

public:
    virtual bool duplicate(SystemComponentItemInterface::Position const & position) = 0;
    virtual bool remove() = 0;

public:
    virtual SystemComponentItemInterface::ID getID() const = 0;
    virtual SystemComponentItemInterface::Library getLibrary() const = 0;
    virtual SystemComponentItemInterface::Type getType() const = 0;

public:
    virtual bool setTitle(SystemComponentItemInterface::Title const & title) = 0;
    virtual SystemComponentItemInterface::Title getTitle() const = 0;

public:
    virtual SystemComponentItemInterface::Schedule * getSchedule() const = 0;
    virtual SystemComponentItemInterface::ParameterMap * getParameters() const = 0;
    virtual SystemComponentItemInterface::InputMap * getInputs() const = 0;
    virtual SystemComponentItemInterface::OutputMap * getOutputs() const = 0;

public:
    virtual bool setPosition(SystemComponentItemInterface::Position const & position) = 0;
    virtual SystemComponentItemInterface::Position getPosition() const = 0;
};

#endif // SYSTEMCOMPONENTITEMINTERFACE_H
