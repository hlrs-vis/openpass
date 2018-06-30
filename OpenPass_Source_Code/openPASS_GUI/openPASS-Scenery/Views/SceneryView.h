/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SCENERYVIEW_H
#define SCENERYVIEW_H

#include "openPASS-Window/WindowInterface.h"

#include <QString>
#include <QWidget>

namespace Ui {
class SceneryView;
}

class SceneryPresenter;

class SceneryView : public QWidget
{
    Q_OBJECT

public:
    explicit SceneryView(WindowInterface * const window,
                         SceneryPresenter * const sceneryPresenter,
                         QWidget *parent = 0);
    virtual ~SceneryView();

private Q_SLOTS: // Update action from the Browser Button
    void updateView();

private Q_SLOTS:
    void on_browseButton_clicked();
    void on_showButton_clicked();
    void on_hideButton_clicked();

private:
    static WindowInterface::ID const ViewID;

private:
    WindowInterface * const window;
    SceneryPresenter * const _sceneryPresenter;

private:
    Ui::SceneryView *ui;
};

#endif // SCENERYVIEW_H
