/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMXMLSAVEMODEL_H
#define SYSTEMXMLSAVEMODEL_H

#include "Models/SystemXMLBaseModel.h"

class QIODevice;
class QXmlStreamWriter;
class SystemComponentMapInterface;
class SystemComponentParameterMapInterface;
class SystemComponentScheduleInterface;
class SystemConnectionMapInterface;
class SystemItemInterface;
class SystemMapInterface;

class SystemXMLSaveModel : public SystemXMLBaseModel
{
public:
    explicit SystemXMLSaveModel() = delete;
    virtual ~SystemXMLSaveModel() = default;

public:
    static bool save(QString const & filepath,
                     SystemMapInterface const * const systems);
    static bool save(QIODevice * const device,
                     SystemMapInterface const * const systems);

protected:
    static void saveSystem(QXmlStreamWriter & xml,
                           SystemItemInterface const * const system);
    static void saveSystemComponent(QXmlStreamWriter & xml,
                                    SystemComponentMapInterface const * const components);
    static void saveSystemComponentSchedule(QXmlStreamWriter & xml,
                                            SystemComponentScheduleInterface const * const schedule);
    static void saveSystemComponentParameters(QXmlStreamWriter & xml,
                                              SystemComponentParameterMapInterface const * const parameters);
    static void saveSystemConnection(QXmlStreamWriter & xml,
                                     SystemConnectionMapInterface const * const connections);
};

#endif // SYSTEMXMLSAVEMODEL_H
