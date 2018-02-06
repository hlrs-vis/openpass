/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/WindowModel.h"

WindowModel::WindowModel(QObject * const parent)
    : WindowInterface(parent)
    , simulation(nullptr)
{
}

WindowModel::~WindowModel()
{
    for (QObject * const object : objects)
    {
        object->deleteLater();
    }
}

void WindowModel::add(WindowInterface::ID const & id,
                      WindowInterface::Button * const button,
                      WindowInterface::Widget * const widget,
                      WindowInterface::Menu const & menu)
{
    // Take ownership of button, actions and submenus
    objects << button;
    button->setParent(nullptr);
    for (WindowInterface::Action * const action : menu)
    {
        if (action != nullptr)
        {
            objects << action;
            action->setParent(nullptr);
            if (action->menu() != nullptr)
            {
                objects << action->menu();
                action->menu()->setParent(nullptr);
            }
        }
    }

    // Verify button for required property 'type'
    if (!button->property("type").isValid())
    {
        button->setProperty("type", 0);
    }

    // Verify button for required property 'order'
    if (!button->property("order").isValid())
    {
        button->setProperty("order", -1);
    }

    // Register view
    views.insert(id, {button, widget, menu});

    // Emit signal 'modified'
    Q_EMIT modified();
}

bool WindowModel::remove(WindowInterface::ID const & id)
{
    if (views.remove(id) > 0)
    {
        Q_EMIT modified();
        return true;
    }
    return false;
}

bool WindowModel::exists(WindowInterface::ID const & id) const
{
    return views.keys().contains(id);
}

WindowModel::ViewList WindowModel::list() const
{
    WindowModel::ViewList list;
    for (WindowModel::ViewModel const & view : views)
    {
        WindowModel::ViewList::Iterator it = list.begin();
        while (it != list.end())
        {
            if ((*it).button->property("order").toInt() >
                    view.button->property("order").toInt())
            {
                break;
            }
            ++it;
        }
        list.insert(it, view);
    }
    return list;
}

void WindowModel::setSimulationWidget(WindowInterface::Widget * const widget)
{
    simulation = widget;
    Q_EMIT modifiedSimulationWidget();
}

WindowInterface::Widget * WindowModel::getSimulationWidget() const
{
    return simulation;
}
