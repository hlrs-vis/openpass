/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTXMLBASEMODEL_H
#define COMPONENTXMLBASEMODEL_H

#include "openPASS-Component/ComponentItemInterface.h"
#include <QList>
#include <QString>

class ComponentXMLBaseModel
{
public:
    explicit ComponentXMLBaseModel() = delete;
    virtual ~ComponentXMLBaseModel() = default;

protected:
    static QString const KeyComponent;
    static QString const KeyComponentType;
    static QString const KeyComponentTitle;
    static QString const KeyComponentSchedule;
    static QString const KeyComponentScheduleOffset;
    static QString const KeyComponentScheduleCycle;
    static QString const KeyComponentScheduleResponse;
    static QString const KeyComponentParameters;
    static QString const KeyComponentParameter;
    static QString const KeyComponentParameterID;
    static QString const KeyComponentParameterType;
    static QString const KeyComponentParameterTitle;
    static QString const KeyComponentParameterUnit;
    static QString const KeyComponentParameterValue;
    static QString const KeyComponentInputs;
    static QString const KeyComponentInput;
    static QString const KeyComponentInputID;
    static QString const KeyComponentInputType;
    static QString const KeyComponentInputTitle;
    static QString const KeyComponentInputUnit;
    static QString const KeyComponentInputCardinality;
    static QString const KeyComponentOutputs;
    static QString const KeyComponentOutput;
    static QString const KeyComponentOutputID;
    static QString const KeyComponentOutputType;
    static QString const KeyComponentOutputTitle;
    static QString const KeyComponentOutputUnit;

protected:
    static QList<QString> const KeyListComponent;
    static QList<QString> const KeyListComponentSchedule;
    static QList<QString> const KeyListComponentParameter;
    static QList<QString> const KeyListComponentInput;
    static QList<QString> const KeyListComponentOutput;

protected:
    static QList<QString> const ConvertComponentType;

protected:
    static QString convertComponentTypeToString(ComponentItemInterface::Type const & type);
    static ComponentItemInterface::Type convertStringToComponentType(QString const & type);
};

#endif // COMPONENTXMLBASEMODEL_H
