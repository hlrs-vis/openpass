/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCONNECTIONITEMMODEL_H
#define SYSTEMCONNECTIONITEMMODEL_H

#include "openPASS-System/SystemConnectionItemInterface.h"

class SystemConnectionItemModel : public SystemConnectionItemInterface
{
    Q_OBJECT

public:
    explicit SystemConnectionItemModel(SystemConnectionItemInterface::Source const * const source,
                                       SystemConnectionItemInterface::Target const * const target,
                                       QObject * const parent = nullptr);
    virtual ~SystemConnectionItemModel() = default;

public:
    virtual SystemConnectionItemInterface::ID getID() const override;
    virtual SystemConnectionItemInterface::Source const * getSource() const override;
    virtual SystemConnectionItemInterface::Target const * getTarget() const override;
    virtual bool isCompatible() const override;

public:
    static bool isCompatible(SystemConnectionItemInterface::Source const * const source,
                             SystemConnectionItemInterface::Target const * const target);

protected:
    SystemConnectionItemInterface::Source const * const source;
    SystemConnectionItemInterface::Target const * const target;
};

#endif // SYSTEMCONNECTIONITEMMODEL_H
