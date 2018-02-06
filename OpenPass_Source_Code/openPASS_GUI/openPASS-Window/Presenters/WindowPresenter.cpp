/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Presenters/WindowPresenter.h"

WindowPresenter::WindowPresenter(WindowModel * const windowModel,
                                 QObject * const parent) :
    QObject(parent),
    windowModel(windowModel)
{
    // Connect WindowModel with WindowPresenter
    connect(windowModel, &WindowModel::modified,
            this, &WindowPresenter::modified);
    connect(windowModel, &WindowModel::modifiedSimulationWidget,
            this, &WindowPresenter::modifiedSimulationWidget);
}

WindowModel::ViewList WindowPresenter::list() const
{
    return windowModel->list();
}

WindowInterface::Widget * WindowPresenter::getSimulationWidget() const
{
    return windowModel->getSimulationWidget();
}
