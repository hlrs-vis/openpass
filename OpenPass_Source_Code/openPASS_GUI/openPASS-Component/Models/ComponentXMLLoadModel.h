/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTXMLLOADMODEL_H
#define COMPONENTXMLLOADMODEL_H

#include "openPASS-Component/ComponentInputMapInterface.h"
#include "openPASS-Component/ComponentItemInterface.h"
#include "openPASS-Component/ComponentOutputMapInterface.h"
#include "openPASS-Component/ComponentParameterMapInterface.h"
#include "openPASS-Component/ComponentScheduleInterface.h"
#include "Models/ComponentXMLBaseModel.h"

class QIODevice;
class QXmlStreamReader;

class ComponentXMLLoadModel : protected ComponentXMLBaseModel
{
public:
    explicit ComponentXMLLoadModel() = delete;
    virtual ~ComponentXMLLoadModel() = default;

public:
    static bool load(QString const & filepath,
                     ComponentItemInterface * const item);
    static bool load(QIODevice * const device,
                     ComponentItemInterface * const item);

protected:
    static bool loadComponent(QXmlStreamReader & xml,
                              ComponentItemInterface * const item);
    static bool loadComponentSchedule(QXmlStreamReader & xml,
                                      ComponentScheduleInterface * const schedule);
    static bool loadComponentParameterMap(QXmlStreamReader & xml,
                                          ComponentParameterMapInterface * const parameters);
    static bool loadComponentParameterItem(QXmlStreamReader & xml,
                                           ComponentParameterMapInterface * const parameters);
    static bool loadComponentInputMap(QXmlStreamReader & xml,
                                      ComponentInputMapInterface * const inputs);
    static bool loadComponentInputItem(QXmlStreamReader & xml,
                                       ComponentInputMapInterface * const inputs);
    static bool loadComponentOutputMap(QXmlStreamReader & xml,
                                       ComponentOutputMapInterface * const outputs);
    static bool loadComponentOutputItem(QXmlStreamReader & xml,
                                        ComponentOutputMapInterface * const outputs);
};

#endif // COMPONENTXMLLOADMODEL_H
