/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef AGENTVIEW_H
#define AGENTVIEW_H

#include "openPASS-Window/WindowInterface.h"

#include <QString>
#include <QWidget>

namespace Ui {
class AgentView;
}

class AgentPresenter;

class AgentView : public QWidget
{
    Q_OBJECT

public:
    explicit AgentView(WindowInterface * const window,
                       AgentPresenter * const agentPresenter,
                       QWidget *parent = 0);
    virtual ~AgentView();

private Q_SLOTS: // Update action from the Browser Button
    void updateView();

private Q_SLOTS:
    void on_browseButton_clicked();

private:
    static WindowInterface::ID const ViewID;

private:
    WindowInterface * const window;
    AgentPresenter * const _agentPresenter;

private:
    Ui::AgentView *ui;

};

#endif // AGENTVIEW_H
