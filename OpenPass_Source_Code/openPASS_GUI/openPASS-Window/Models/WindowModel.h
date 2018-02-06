/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef WINDOWMODEL_H
#define WINDOWMODEL_H

#include "openPASS-Window/WindowInterface.h"
#include <QList>
#include <QMap>
#include <QObject>
#include <QSet>

class WindowModel : public WindowInterface
{
    Q_OBJECT

public:
    struct ViewModel
    {
        WindowInterface::Button * button;
        WindowInterface::Widget * widget;
        WindowInterface::Menu menu;
    };
    using ViewList = QList<WindowModel::ViewModel>;
    using ViewMap = QMap<WindowInterface::ID, WindowModel::ViewModel>;

public:
    explicit WindowModel(QObject * const parent = nullptr);
    virtual ~WindowModel();

public:
    virtual void add(WindowInterface::ID const & id,
                     WindowInterface::Button * const button,
                     WindowInterface::Widget * const widget = nullptr,
                     WindowInterface::Menu const & menu = {}) override;
    virtual bool remove(WindowInterface::ID const & id) override;
    virtual bool exists(WindowInterface::ID const & id) const override;
    virtual WindowModel::ViewList list() const;

public:
    virtual void setSimulationWidget(WindowInterface::Widget * const widget = nullptr) override;
    virtual WindowInterface::Widget * getSimulationWidget() const override;

Q_SIGNALS:
    void modified();
    void modifiedSimulationWidget();

private:
    WindowModel::ViewMap views;
    QSet<QObject *> objects;
    WindowInterface::Widget * simulation;
};

#endif // WINDOWMODEL_H
