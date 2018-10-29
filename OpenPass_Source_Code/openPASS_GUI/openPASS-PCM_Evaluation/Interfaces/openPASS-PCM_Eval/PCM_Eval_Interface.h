/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PCM_EVAL_INTERFACE_H
#define PCM_EVAL_INTERFACE_H

#include "openPASS/ServiceInterface.h"

#include <QObject>

static ServiceManagerInterface::ID const PCM_Eval_InterfaceID =
    ServiceManagerInterfaceID("openPASS.PCM_Eval.PCM_Eval_Interface");

class PCM_Eval_Interface : public QObject,
    public ServiceInterface<PCM_Eval_Interface, PCM_Eval_InterfaceID>
{
    Q_OBJECT

public:
    explicit PCM_Eval_Interface(QObject *const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~PCM_Eval_Interface() = default;
};

#endif // PCM_EVAL_INTERFACE_H
