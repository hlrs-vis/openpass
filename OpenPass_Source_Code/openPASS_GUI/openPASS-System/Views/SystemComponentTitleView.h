/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTTITLEVIEW_H
#define SYSTEMCOMPONENTTITLEVIEW_H

#include "Views/WidgetView.h"
#include <QPoint>

class SystemComponentItemInterface;
class QMouseEvent;

namespace Ui {
class SystemComponentTitleView;
}

class SystemComponentTitleView : public WidgetView
{
    Q_OBJECT

public:
    explicit SystemComponentTitleView(SystemComponentItemInterface * const component,
                                      QWidget * const parent = nullptr);
    virtual ~SystemComponentTitleView();

public Q_SLOTS:
    void updateTitle();

protected:
    static QPoint const MouseInvalidPosition;

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;

protected:
    SystemComponentItemInterface * const component;

protected:
    QPoint mouseGlobalPosition;

protected:
    Ui::SystemComponentTitleView * const ui;
};

#endif // SYSTEMCOMPONENTTITLEVIEW_H
