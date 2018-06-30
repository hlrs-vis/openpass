/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTMANAGERMODEL_H
#define SYSTEMCOMPONENTMANAGERMODEL_H

#include "openPASS-Component/ComponentInterface.h"
#include "openPASS-Component/ComponentMapInterface.h"
#include "openPASS-System/SystemComponentManagerInterface.h"

class SystemComponentManagerModel : public SystemComponentManagerInterface
{
    Q_OBJECT

public:
    explicit SystemComponentManagerModel(ComponentInterface * const component,
                                         QObject * const parent = nullptr);
    virtual ~SystemComponentManagerModel() = default;

public:
    virtual bool loadFromDirectory(QDir const & directory) override;
    virtual QList<ComponentItemInterface::Title> listTitlesByType(ComponentItemInterface::Type const & type) const override;
    virtual ComponentItemInterface * lookupItemByName(ComponentItemInterface::Name const & name) const override;
    virtual ComponentItemInterface * lookupItemByTitle(ComponentItemInterface::Title const & title) const override;

protected:
    ComponentMapInterface * const components;
};

#endif // SYSTEMCOMPONENTMANAGERMODEL_H
