/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTINPUTITEMMODEL_H
#define SYSTEMCOMPONENTINPUTITEMMODEL_H

#include "openPASS-System/SystemComponentInputItemInterface.h"

class SystemComponentInputItemModel : public SystemComponentInputItemInterface
{
    Q_OBJECT

public:
    explicit SystemComponentInputItemModel(ComponentInputItemInterface const * const input,
                                           QObject * const parent = nullptr);
    virtual ~SystemComponentInputItemModel() = default;

public:
    virtual SystemComponentItemInterface * getComponent() const override;

public:
    virtual SystemComponentInputItemInterface::ID getID() const override;
    virtual SystemComponentInputItemInterface::Type getType() const override;
    virtual SystemComponentInputItemInterface::Title getTitle() const override;
    virtual SystemComponentInputItemInterface::Unit getUnit() const override;
    virtual SystemComponentInputItemInterface::Cardinality getCardinality() const override;

protected:
    ComponentInputItemInterface const * const input;
};

#endif // SYSTEMCOMPONENTINPUTITEMMODEL_H
