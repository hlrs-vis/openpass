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
//! @file  WindowInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef WINDOWINTERFACE_H
#define WINDOWINTERFACE_H

#include "openPASS/ServiceInterface.h"
#include <QList>
#include <QMenu>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QWidget>

static ServiceManagerInterface::ID const WindowInterfaceID =
        ServiceManagerInterfaceID("openPASS.Window.WindowInterface");

class WindowInterface : public QObject,
        public ServiceInterface<WindowInterface, WindowInterfaceID>
{
    Q_OBJECT

public:
    using ID = QString;
    using Button = QPushButton;
    using ButtonTitle = QString;
    using ButtonType = unsigned short;
    using ButtonOrder = unsigned short;
    using Widget = QWidget;
    using Action = QPushButton;
    using ActionTitle = QString;
    using ActionMenu = QMenu;
    using Menu = QList<WindowInterface::Action *>;

public:
    explicit WindowInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~WindowInterface() = default;

public:
    virtual void add(WindowInterface::ID const & id,
                     WindowInterface::Button * const button,
                     WindowInterface::Widget * const widget = nullptr,
                     WindowInterface::Menu const & menu = {}) = 0;
    virtual bool remove(WindowInterface::ID const & id) = 0;
    virtual bool exists(WindowInterface::ID const & id) const = 0;

public:
    virtual void setSimulationWidget(WindowInterface::Widget * const widget = nullptr) = 0;
    virtual WindowInterface::Widget * getSimulationWidget() const = 0;

public:
    static inline WindowInterface::Button * createButton(WindowInterface::ButtonTitle const & title,
                                                         WindowInterface::ButtonType const & type = 0,
                                                         WindowInterface::ButtonOrder const & order = -1)
    {
        WindowInterface::Button * const button = new WindowInterface::Button(title);
        button->setProperty("type", type);
        button->setProperty("order", order);
        return button;
    }

public:
    template <typename Object, typename Function>
    static inline WindowInterface::Action * createAction(WindowInterface::ActionTitle const & title,
                                                         Object object,
                                                         Function function)
    {
        WindowInterface::Action * const action = new WindowInterface::Action(title);
        QObject::connect(action, &WindowInterface::Action::clicked, object, function);
        return action;
    }

    static inline WindowInterface::Action * createAction(WindowInterface::ActionTitle const & title,
                                                         WindowInterface::ActionMenu * const menu)
    {
        WindowInterface::Action * const action = new WindowInterface::Action(title);
        action->setMenu(menu);
        return action;
    }

    static inline WindowInterface::Action * createSeparator()
    {
        return nullptr;
    }
};

#endif // WINDOWINTERFACE_H
