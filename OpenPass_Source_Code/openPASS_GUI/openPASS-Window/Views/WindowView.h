/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef WINDOWVIEW_H
#define WINDOWVIEW_H

#include "openPASS-Window/WindowInterface.h"
#include <QMainWindow>
#include <QMap>
#include <QPair>

class WindowPresenter;

namespace Ui {
class WindowView;
}

class WindowView : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowView(WindowPresenter * const windowPresenter,
                        QWidget * const parent = nullptr);
    virtual ~WindowView();

private Q_SLOTS:
    void sidebar_clicked();

private Q_SLOTS:
    void clearSimulation();
    void updateSimulation();

private Q_SLOTS:
    void clearViews();
    void updateViews();

private Q_SLOTS:
    void clearMenu();

private:
    using ViewItem = QPair<WindowInterface::Widget *, WindowInterface::Menu>;
    using ViewMap = QMap<WindowInterface::Button *, WindowView::ViewItem>;

private:
    WindowPresenter * const windowPresenter;

private:
    Ui::WindowView * const ui;
    WindowView::ViewMap views;

};

#endif // WINDOWVIEW_H
