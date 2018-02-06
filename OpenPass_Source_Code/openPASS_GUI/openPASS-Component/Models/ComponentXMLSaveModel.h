/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTXMLSAVEMODEL_H
#define COMPONENTXMLSAVEMODEL_H

#include "openPASS-Component/ComponentInputMapInterface.h"
#include "openPASS-Component/ComponentItemInterface.h"
#include "openPASS-Component/ComponentOutputMapInterface.h"
#include "openPASS-Component/ComponentParameterMapInterface.h"
#include "openPASS-Component/ComponentScheduleInterface.h"
#include "Models/ComponentXMLBaseModel.h"

class QIODevice;
class QXmlStreamWriter;

class ComponentXMLSaveModel : protected ComponentXMLBaseModel
{
public:
    explicit ComponentXMLSaveModel() = delete;
    virtual ~ComponentXMLSaveModel() = default;

public:
    static bool save(QString const & filepath,
                     ComponentItemInterface const * const item);
    static bool save(QIODevice * const device,
                     ComponentItemInterface const * const item);

protected:
    static void saveComponent(QXmlStreamWriter & xml,
                              ComponentItemInterface const * const item);
    static void saveComponentSchedule(QXmlStreamWriter & xml,
                                      ComponentScheduleInterface const * const schedule);
    static void saveComponentParameters(QXmlStreamWriter & xml,
                                        ComponentParameterMapInterface const * const parameters);
    static void saveComponentInputs(QXmlStreamWriter & xml,
                                    ComponentInputMapInterface const * const inputs);
    static void saveComponentOutputs(QXmlStreamWriter & xml,
                                     ComponentOutputMapInterface const * const outputs);
};

#endif // COMPONENTXMLSAVEMODEL_H
