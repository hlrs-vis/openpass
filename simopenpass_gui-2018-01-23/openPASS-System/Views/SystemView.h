/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMVIEW_H
#define SYSTEMVIEW_H

#include "openPASS-Window/WindowInterface.h"
#include "Views/WidgetView.h"

class ProjectInterface;
class SystemInterface;

namespace Ui {
class SystemView;
}

class SystemView : public WidgetView
{
    Q_OBJECT

public:
    explicit SystemView(WindowInterface * const window,
                        ProjectInterface * const project,
                        SystemInterface * const system,
                        QWidget * const parent = nullptr);
    virtual ~SystemView();

protected:
    static WindowInterface::ID const ViewID;

protected Q_SLOTS:
    void actionNewSystem_clicked();
    void actionLoadSystem_clicked();
    void actionSaveSystem_clicked();
    void actionSaveScreenshot_clicked();

protected:
    WindowInterface * const window;
    ProjectInterface * const project;
    SystemInterface * const system;

protected:
    Ui::SystemView * const ui;
};

#endif // SYSTEMVIEW_H
