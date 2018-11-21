/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

//-----------------------------------------------------------------------------
//! @file  PluginManagerModel.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef PLUGINSMODEL_H
#define PLUGINSMODEL_H

#include "openPASS/PluginManagerInterface.h"
#include <QList>

class QPluginLoader;

class PluginManagerModel : public PluginManagerInterface
{
    Q_OBJECT

public:
    explicit PluginManagerModel(ServiceManagerInterface * const services,
                                QObject * const parent = nullptr);
    virtual ~PluginManagerModel();

public:
    virtual bool loadDirectory(QDir const & directory) override;
    virtual bool isLoaded(QString const & iid) const override;

protected:
    static QString getPluginIID(QPluginLoader * const loader);

protected:
    ServiceManagerInterface * const services;
    QList<QPluginLoader *> plugins;
};

#endif // PLUGINSMODEL_H
