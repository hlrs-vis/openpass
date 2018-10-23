/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef WINDOWPRESENTER_H
#define WINDOWPRESENTER_H

#include "Models/WindowModel.h"
#include <QObject>

class WindowPresenter : public QObject
{
    Q_OBJECT

public:
    explicit WindowPresenter(WindowModel * const windowModel,
                             QObject * const parent = nullptr);
    virtual ~WindowPresenter() = default;

public:
    WindowModel::ViewList list() const;

public:
    WindowInterface::Widget * getSimulationWidget() const;

Q_SIGNALS:
    void modified();
    void modifiedSimulationWidget();

private:
    WindowModel * const windowModel;

};

#endif // WINDOWPRESENTER_H
