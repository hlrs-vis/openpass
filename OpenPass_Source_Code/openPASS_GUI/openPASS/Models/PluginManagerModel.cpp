/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/PluginManagerModel.h"

#include "openPASS/PluginInterface.h"
#include "openPASS/ServiceManagerInterface.h"
#include <QLibrary>
#include <QPluginLoader>
#include <limits>

PluginManagerModel::PluginManagerModel(ServiceManagerInterface * const services,
                                       QObject * const parent)
    : PluginManagerInterface(parent)
    , services(services)
{
}

PluginManagerModel::~PluginManagerModel()
{
    // As long as there are loaded openPASS plugins
    while (plugins.count() > 0)
    {
        // Take last openPASS plugin
        QPluginLoader * const loader = plugins.takeLast();
        PluginInterface * const plugin =
                qobject_cast<PluginInterface *>(loader->instance());

        // Deinitialize and unload plugin
        plugin->deinitialize();
        loader->unload();

        // Destroy plugin
        delete loader;
    }
}

bool PluginManagerModel::loadDirectory(QDir const & directory)
{
    // List of preloaded openPASS plugins
    QList<QPluginLoader *> preloaded;

    // For each file in directory
    for (QString const & filename : directory.entryList(QDir::Files | QDir::Readable, QDir::Name))
    {
        // Generate absolute filepath
        QString const filepath = directory.absoluteFilePath(filename);

        // Is the file a valid library?
        if (QLibrary::isLibrary(filepath))
        {
            // Preload plugin
            QPluginLoader * const loader = new QPluginLoader(filepath);
            PluginInterface * const plugin =
                    qobject_cast<PluginInterface *>(loader->instance());

            // Is the plugin a valid openPASS plugin and is the plugin not already loaded?
            if ((plugin) && (!isLoaded(getPluginIID(loader))))
            {
                // Append plugin to list of preloaded openPASS plugins
                preloaded.append(loader);
            }
            else
            {
                // Destroy plugin
                delete loader;
            }
        }
    }

    // Verify dependencies of preloaded openPASS plugins
    unsigned int loaded = std::numeric_limits<unsigned int>::max();
    while ((preloaded.count() > 0) && (loaded > 0))
    {
        // How many openPASS plugins have we loaded in this iteration?
        loaded = 0;

        // For each preloaded openPASS plugin
        for (int index = 0; index < preloaded.count(); ++index)
        {
            // Retrieve preloaded openPASS plugin
            QPluginLoader * const loader = preloaded.value(index);
            PluginInterface * const plugin =
                    qobject_cast<PluginInterface *>(loader->instance());

            // Initialize plugin
            if (plugin->services(services) && plugin->initialize())
            {
                // Migrate plugin from preloaded to loaded
                plugins.append(loader);
                preloaded.removeOne(loader);
                --index;

                // Increase number of loaded plugins
                ++loaded;
            }
        }
    }

    // Are there still preloaded openPASS plugins?
    if (preloaded.count() > 0)
    {
        // Destroy failed preloaded openPASS plugins
        for (QPluginLoader * const loader : preloaded)
        {
            delete loader;
        }

        // Return failure
        return false;
    }

    // Return success
    return true;
}

bool PluginManagerModel::isLoaded(QString const & iid) const
{
    // For each loaded openPASS plugin
    for (QPluginLoader * const loader : plugins)
    {
        // Is the IID of the plugin the same as the queried IID?
        if (getPluginIID(loader) == iid)
        {
            // Return success
            return true;
        }
    }

    // Return failure
    return false;
}

QString PluginManagerModel::getPluginIID(QPluginLoader * const loader)
{
    // Return plugin IID
    return loader->metaData().value(QStringLiteral("IID")).toString();
}
