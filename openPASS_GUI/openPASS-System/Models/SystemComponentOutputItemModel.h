/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTOUTPUTITEMMODEL_H
#define SYSTEMCOMPONENTOUTPUTITEMMODEL_H

#include "openPASS-System/SystemComponentOutputItemInterface.h"

class SystemComponentOutputItemModel : public SystemComponentOutputItemInterface
{
    Q_OBJECT

public:
    explicit SystemComponentOutputItemModel(ComponentOutputItemInterface const * const output,
                                            QObject * const parent = nullptr);
    virtual ~SystemComponentOutputItemModel() = default;

public:
    virtual SystemComponentItemInterface * getComponent() const override;

public:
    virtual SystemComponentOutputItemInterface::ID getID() const override;
    virtual SystemComponentOutputItemInterface::Type getType() const override;
    virtual SystemComponentOutputItemInterface::Title getTitle() const override;
    virtual SystemComponentOutputItemInterface::Unit getUnit() const override;

protected:
    ComponentOutputItemInterface const * const output;
};

#endif // SYSTEMCOMPONENTOUTPUTITEMMODEL_H
