/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemXMLBaseModel.h"

QString const SystemXMLBaseModel::KeySystems = QStringLiteral("systems");
QString const SystemXMLBaseModel::KeySystem = QStringLiteral("system");
QString const SystemXMLBaseModel::KeySystemID = QStringLiteral("id");
QString const SystemXMLBaseModel::KeySystemTitle = QStringLiteral("title");
QString const SystemXMLBaseModel::KeySystemPriority = QStringLiteral("priority");
QString const SystemXMLBaseModel::KeySystemComponents = QStringLiteral("components");
QString const SystemXMLBaseModel::KeySystemComponent = QStringLiteral("component");
QString const SystemXMLBaseModel::KeySystemComponentID = QStringLiteral("id");
QString const SystemXMLBaseModel::KeySystemComponentLibrary = QStringLiteral("library");
QString const SystemXMLBaseModel::KeySystemComponentTitle = QStringLiteral("title");
QString const SystemXMLBaseModel::KeySystemComponentPriority = QStringLiteral("priority");
QString const SystemXMLBaseModel::KeySystemComponentSchedule = QStringLiteral("schedule");
QString const SystemXMLBaseModel::KeySystemComponentSchedulePriority = QStringLiteral("priority");
QString const SystemXMLBaseModel::KeySystemComponentScheduleOffset = QStringLiteral("offset");
QString const SystemXMLBaseModel::KeySystemComponentScheduleCycle = QStringLiteral("cycle");
QString const SystemXMLBaseModel::KeySystemComponentScheduleResponse = QStringLiteral("response");
QString const SystemXMLBaseModel::KeySystemComponentParameters = QStringLiteral("parameters");
QString const SystemXMLBaseModel::KeySystemComponentParameter = QStringLiteral("parameter");
QString const SystemXMLBaseModel::KeySystemComponentParameterID = QStringLiteral("id");
QString const SystemXMLBaseModel::KeySystemComponentParameterType = QStringLiteral("type");
QString const SystemXMLBaseModel::KeySystemComponentParameterUnit = QStringLiteral("unit");
QString const SystemXMLBaseModel::KeySystemComponentParameterValue = QStringLiteral("value");
QString const SystemXMLBaseModel::KeySystemComponentPosition = QStringLiteral("position");
QString const SystemXMLBaseModel::KeySystemComponentPositionX = QStringLiteral("x");
QString const SystemXMLBaseModel::KeySystemComponentPositionY = QStringLiteral("y");
QString const SystemXMLBaseModel::KeySystemConnections = QStringLiteral("connections");
QString const SystemXMLBaseModel::KeySystemConnection = QStringLiteral("connection");
QString const SystemXMLBaseModel::KeySystemConnectionID = QStringLiteral("id");
QString const SystemXMLBaseModel::KeySystemConnectionSource = QStringLiteral("source");
QString const SystemXMLBaseModel::KeySystemConnectionSourceComponent = QStringLiteral("component");
QString const SystemXMLBaseModel::KeySystemConnectionSourceOutput = QStringLiteral("output");
QString const SystemXMLBaseModel::KeySystemConnectionTarget = QStringLiteral("target");
QString const SystemXMLBaseModel::KeySystemConnectionTargetComponent = QStringLiteral("component");
QString const SystemXMLBaseModel::KeySystemConnectionTargetInput = QStringLiteral("input");

QList<QString> const SystemXMLBaseModel::KeyListSystem = {
    SystemXMLBaseModel::KeySystemID,
    SystemXMLBaseModel::KeySystemTitle,
    SystemXMLBaseModel::KeySystemPriority,
    SystemXMLBaseModel::KeySystemComponents,
    SystemXMLBaseModel::KeySystemConnections,
};
QList<QString> const SystemXMLBaseModel::KeyListSystemComponent = {
    SystemXMLBaseModel::KeySystemComponentID,
    SystemXMLBaseModel::KeySystemComponentLibrary,
    SystemXMLBaseModel::KeySystemComponentTitle,
    SystemXMLBaseModel::KeySystemComponentSchedule,
    SystemXMLBaseModel::KeySystemComponentParameters,
};
QList<QString> const SystemXMLBaseModel::KeyListSystemComponentSchedule = {
    SystemXMLBaseModel::KeySystemComponentScheduleOffset,
    SystemXMLBaseModel::KeySystemComponentScheduleCycle,
    SystemXMLBaseModel::KeySystemComponentScheduleResponse,
};
QList<QString> const SystemXMLBaseModel::KeyListSystemComponentParameter = {
    SystemXMLBaseModel::KeySystemComponentParameterID,
    SystemXMLBaseModel::KeySystemComponentParameterType,
    SystemXMLBaseModel::KeySystemComponentParameterUnit,
    SystemXMLBaseModel::KeySystemComponentParameterValue,
};
QList<QString> const SystemXMLBaseModel::KeyListSystemComponentPosition = {
    SystemXMLBaseModel::KeySystemComponentPositionX,
    SystemXMLBaseModel::KeySystemComponentPositionY,
};
QList<QString> const SystemXMLBaseModel::KeyListSystemConnection = {
    SystemXMLBaseModel::KeySystemConnectionID,
    SystemXMLBaseModel::KeySystemConnectionSource,
    SystemXMLBaseModel::KeySystemConnectionTarget,
};
QList<QString> const SystemXMLBaseModel::KeyListSystemConnectionSource = {
    SystemXMLBaseModel::KeySystemConnectionSourceComponent,
    SystemXMLBaseModel::KeySystemConnectionSourceOutput,
};
QList<QString> const SystemXMLBaseModel::KeyListSystemConnectionTarget = {
    SystemXMLBaseModel::KeySystemConnectionTargetComponent,
    SystemXMLBaseModel::KeySystemConnectionTargetInput,
};
