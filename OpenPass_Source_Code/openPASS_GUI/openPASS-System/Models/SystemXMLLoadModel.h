/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMXMLLOADMODEL_H
#define SYSTEMXMLLOADMODEL_H

#include "openPASS-System/SystemComponentInputMapInterface.h"
#include "openPASS-System/SystemComponentItemInterface.h"
#include "openPASS-System/SystemComponentManagerInterface.h"
#include "openPASS-System/SystemComponentMapInterface.h"
#include "openPASS-System/SystemComponentOutputMapInterface.h"
#include "openPASS-System/SystemComponentParameterMapInterface.h"
#include "openPASS-System/SystemComponentScheduleInterface.h"
#include "openPASS-System/SystemConnectionMapInterface.h"
#include "openPASS-System/SystemItemInterface.h"
#include "Models/SystemXMLBaseModel.h"

class QIODevice;
class QXmlStreamReader;

class SystemXMLLoadModel : public SystemXMLBaseModel
{
public:
    explicit SystemXMLLoadModel() = delete;
    virtual ~SystemXMLLoadModel() = default;

public:
    static bool load(QString const & filepath,
                     SystemItemInterface * const system,
                     SystemComponentManagerInterface const * const manager);
    static bool load(QIODevice * const device,
                     SystemItemInterface * const system,
                     SystemComponentManagerInterface const * const manager);

protected:
    static bool loadSystemItem(QXmlStreamReader & xml,
                               SystemItemInterface * const system,
                               SystemComponentManagerInterface const * const manager);

protected:
    static bool loadSystemComponentMap(QXmlStreamReader & xml,
                                       SystemComponentMapInterface * const components,
                                       SystemComponentManagerInterface const * const manager);
    static bool loadSystemComponentItem(QXmlStreamReader & xml,
                                        SystemComponentMapInterface * const components,
                                        SystemComponentManagerInterface const * const manager);
    static bool loadSystemComponentSchedule(QXmlStreamReader & xml,
                                            SystemComponentScheduleInterface * const schedule);
    static bool loadSystemComponentParameterMap(QXmlStreamReader & xml,
                                                SystemComponentParameterMapInterface * const parameters);
    static bool loadSystemComponentParameterItem(QXmlStreamReader & xml,
                                                 SystemComponentParameterMapInterface * const parameters);
    static bool loadSystemComponentPosition(QXmlStreamReader & xml,
                                            SystemComponentItemInterface * const component);

protected:
    static bool loadSystemConnectionMap(QXmlStreamReader & xml,
                                        SystemComponentMapInterface const * const components,
                                        SystemConnectionMapInterface * const connections);
    static bool loadSystemConnectionItem(QXmlStreamReader & xml,
                                         SystemComponentMapInterface const * const components,
                                         SystemConnectionMapInterface * const connections);
    static bool loadSystemConnectionItemSource(QXmlStreamReader & xml,
                                               SystemComponentMapInterface::ID & sourceComponentID,
                                               SystemComponentOutputMapInterface::ID & sourceOutputID);
    static bool loadSystemConnectionItemTarget(QXmlStreamReader & xml,
                                               SystemComponentMapInterface::ID & targetComponentID,
                                               SystemComponentInputMapInterface::ID & targetInputID);
};

#endif // SYSTEMXMLLOADMODEL_H
