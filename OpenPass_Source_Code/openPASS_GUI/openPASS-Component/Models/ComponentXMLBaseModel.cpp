/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentXMLBaseModel.h"

QString const ComponentXMLBaseModel::KeyComponent = QStringLiteral("component");
QString const ComponentXMLBaseModel::KeyComponentType = QStringLiteral("type");
QString const ComponentXMLBaseModel::KeyComponentTitle = QStringLiteral("title");
QString const ComponentXMLBaseModel::KeyComponentSchedule = QStringLiteral("schedule");
QString const ComponentXMLBaseModel::KeyComponentScheduleOffset = QStringLiteral("offset");
QString const ComponentXMLBaseModel::KeyComponentScheduleCycle = QStringLiteral("cycle");
QString const ComponentXMLBaseModel::KeyComponentScheduleResponse = QStringLiteral("response");
QString const ComponentXMLBaseModel::KeyComponentParameters = QStringLiteral("parameters");
QString const ComponentXMLBaseModel::KeyComponentParameter = QStringLiteral("parameter");
QString const ComponentXMLBaseModel::KeyComponentParameterID = QStringLiteral("id");
QString const ComponentXMLBaseModel::KeyComponentParameterType = QStringLiteral("type");
QString const ComponentXMLBaseModel::KeyComponentParameterTitle = QStringLiteral("title");
QString const ComponentXMLBaseModel::KeyComponentParameterUnit = QStringLiteral("unit");
QString const ComponentXMLBaseModel::KeyComponentParameterValue = QStringLiteral("value");
QString const ComponentXMLBaseModel::KeyComponentInputs = QStringLiteral("inputs");
QString const ComponentXMLBaseModel::KeyComponentInput = QStringLiteral("input");
QString const ComponentXMLBaseModel::KeyComponentInputID = QStringLiteral("id");
QString const ComponentXMLBaseModel::KeyComponentInputType = QStringLiteral("type");
QString const ComponentXMLBaseModel::KeyComponentInputTitle = QStringLiteral("title");
QString const ComponentXMLBaseModel::KeyComponentInputUnit = QStringLiteral("unit");
QString const ComponentXMLBaseModel::KeyComponentInputCardinality = QStringLiteral("cardinality");
QString const ComponentXMLBaseModel::KeyComponentOutputs = QStringLiteral("outputs");
QString const ComponentXMLBaseModel::KeyComponentOutput = QStringLiteral("output");
QString const ComponentXMLBaseModel::KeyComponentOutputID = QStringLiteral("id");
QString const ComponentXMLBaseModel::KeyComponentOutputType = QStringLiteral("type");
QString const ComponentXMLBaseModel::KeyComponentOutputTitle = QStringLiteral("title");
QString const ComponentXMLBaseModel::KeyComponentOutputUnit = QStringLiteral("unit");

QList<QString> const ComponentXMLBaseModel::KeyListComponent = {
    ComponentXMLBaseModel::KeyComponentType,
    ComponentXMLBaseModel::KeyComponentTitle,
    ComponentXMLBaseModel::KeyComponentSchedule,
    ComponentXMLBaseModel::KeyComponentParameters,
    ComponentXMLBaseModel::KeyComponentInputs,
    ComponentXMLBaseModel::KeyComponentOutputs,
};
QList<QString> const ComponentXMLBaseModel::KeyListComponentSchedule = {
    ComponentXMLBaseModel::KeyComponentScheduleOffset,
    ComponentXMLBaseModel::KeyComponentScheduleCycle,
    ComponentXMLBaseModel::KeyComponentScheduleResponse,
};
QList<QString> const ComponentXMLBaseModel::KeyListComponentParameter = {
    ComponentXMLBaseModel::KeyComponentParameterID,
    ComponentXMLBaseModel::KeyComponentParameterType,
    ComponentXMLBaseModel::KeyComponentParameterTitle,
    ComponentXMLBaseModel::KeyComponentParameterUnit,
    ComponentXMLBaseModel::KeyComponentParameterValue,
};
QList<QString> const ComponentXMLBaseModel::KeyListComponentInput = {
    ComponentXMLBaseModel::KeyComponentInputID,
    ComponentXMLBaseModel::KeyComponentInputType,
    ComponentXMLBaseModel::KeyComponentInputTitle,
    ComponentXMLBaseModel::KeyComponentInputUnit,
    ComponentXMLBaseModel::KeyComponentInputCardinality,
};
QList<QString> const ComponentXMLBaseModel::KeyListComponentOutput = {
    ComponentXMLBaseModel::KeyComponentOutputID,
    ComponentXMLBaseModel::KeyComponentOutputType,
    ComponentXMLBaseModel::KeyComponentOutputTitle,
    ComponentXMLBaseModel::KeyComponentOutputUnit,
};

QList<QString> const ComponentXMLBaseModel::ConvertComponentType = {
    QStringLiteral("Action"),
    QStringLiteral("Algorithm"),
    QStringLiteral("Sensor"),
};

QString ComponentXMLBaseModel::convertComponentTypeToString(ComponentItemInterface::Type const & type)
{
    return ConvertComponentType.value(static_cast<unsigned char>(type), ConvertComponentType.value(1));
}

ComponentItemInterface::Type ComponentXMLBaseModel::convertStringToComponentType(QString const & type)
{
    return ((ConvertComponentType.contains(type))
            ? static_cast<ComponentItemInterface::Type>(ConvertComponentType.indexOf(type))
            : ComponentItemInterface::Type::Algorithm);
}
