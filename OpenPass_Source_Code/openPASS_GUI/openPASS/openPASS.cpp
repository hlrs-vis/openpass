/******************************************************************************
* Copyright (c) 2017 Volkswagen Group of America.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <QApplication>
#include <QDir>
#include <QTimer>

#include "openPASS/RandomInterface.h"
#include "Models/ServiceManagerModel.h"
#include "Models/PluginManagerModel.h"

int main(int argc, char *argv[])
{
    // Initialize random interface
    RandomInterface::initialize();

    // Initialize application
    QApplication application(argc, argv);
    application.setApplicationName(QStringLiteral(APPLICATION_NAME));
    application.setApplicationVersion(QStringLiteral(APPLICATION_VERSION));

    // Initialize models
    ServiceManagerModel services;
    PluginManagerModel plugins(&services);

    // Schedule application
    QTimer::singleShot(0, [&plugins](){
        // Load plugins
        plugins.loadDirectory(QDir(QApplication::applicationDirPath()));

        // Emit signal 'started'
        Q_EMIT plugins.started();
    });

    // Execute application
    return application.exec();
}
