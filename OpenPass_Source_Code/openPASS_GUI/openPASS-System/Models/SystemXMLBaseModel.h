/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMXMLBASEMODEL_H
#define SYSTEMXMLBASEMODEL_H

#include <QList>
#include <QObject>
#include <QString>

class SystemXMLBaseModel
{
public:
    explicit SystemXMLBaseModel() = delete;
    virtual ~SystemXMLBaseModel() = default;

protected:
    static QString const KeySystems;
    static QString const KeySystem;
    static QString const KeySystemID;
    static QString const KeySystemTitle;
    static QString const KeySystemPriority;
    static QString const KeySystemComponents;
    static QString const KeySystemComponent;
    static QString const KeySystemComponentID;
    static QString const KeySystemComponentLibrary;
    static QString const KeySystemComponentTitle;
    static QString const KeySystemComponentPriority;
    static QString const KeySystemComponentSchedule;
    static QString const KeySystemComponentSchedulePriority;
    static QString const KeySystemComponentScheduleOffset;
    static QString const KeySystemComponentScheduleCycle;
    static QString const KeySystemComponentScheduleResponse;
    static QString const KeySystemComponentParameters;
    static QString const KeySystemComponentParameter;
    static QString const KeySystemComponentParameterID;
    static QString const KeySystemComponentParameterType;
    static QString const KeySystemComponentParameterUnit;
    static QString const KeySystemComponentParameterValue;
    static QString const KeySystemComponentPosition;
    static QString const KeySystemComponentPositionX;
    static QString const KeySystemComponentPositionY;
    static QString const KeySystemConnections;
    static QString const KeySystemConnection;
    static QString const KeySystemConnectionID;
    static QString const KeySystemConnectionSource;
    static QString const KeySystemConnectionSourceComponent;
    static QString const KeySystemConnectionSourceOutput;
    static QString const KeySystemConnectionTarget;
    static QString const KeySystemConnectionTargetComponent;
    static QString const KeySystemConnectionTargetInput;

protected:
    static QList<QString> const KeyListSystem;
    static QList<QString> const KeyListSystemComponent;
    static QList<QString> const KeyListSystemComponentSchedule;
    static QList<QString> const KeyListSystemComponentParameter;
    static QList<QString> const KeyListSystemComponentPosition;
    static QList<QString> const KeyListSystemConnection;
    static QList<QString> const KeyListSystemConnectionSource;
    static QList<QString> const KeyListSystemConnectionTarget;
};

#endif // SYSTEMXMLBASEMODEL_H
