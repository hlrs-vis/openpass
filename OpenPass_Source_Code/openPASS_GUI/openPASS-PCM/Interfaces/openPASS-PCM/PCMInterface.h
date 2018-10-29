/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PCMINTERFACE_H
#define PCMINTERFACE_H

#include "openPASS/ServiceInterface.h"

#include <QObject>

static ServiceManagerInterface::ID const PCMInterfaceID =
        ServiceManagerInterfaceID("openPASS.PCM.PCMInterface");

class PCMInterface : public QObject,
        public ServiceInterface<PCMInterface, PCMInterfaceID>
{
    Q_OBJECT

public:
    explicit PCMInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~PCMInterface() = default;
};

#endif // PCMINTERFACE_H
