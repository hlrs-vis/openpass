/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTMANAGERVIEW_H
#define SYSTEMCOMPONENTMANAGERVIEW_H

#include <QWidget>

class ProjectInterface;
class SystemComponentManagerInterface;

namespace Ui {
class SystemComponentManagerView;
}

class SystemComponentManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit SystemComponentManagerView(SystemComponentManagerInterface * const components,
                                        ProjectInterface * const project,
                                        QWidget * const parent = nullptr);
    virtual ~SystemComponentManagerView();

public Q_SLOTS:
    virtual void show();
    virtual void hide();
    virtual void updateManagerView();

protected Q_SLOTS:
    void on_button_clicked();

private:
    SystemComponentManagerInterface * _components;
    ProjectInterface * const _project;

protected:
    Ui::SystemComponentManagerView * const ui;
};

#endif // SYSTEMCOMPONENTMANAGERVIEW_H
