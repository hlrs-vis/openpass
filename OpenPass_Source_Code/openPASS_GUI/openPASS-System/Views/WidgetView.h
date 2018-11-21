/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef WIDGETVIEW_H
#define WIDGETVIEW_H

#include <QWidget>

class QPaintEvent;

class WidgetView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetView(QWidget * const parent = nullptr);
    virtual ~WidgetView() = default;

protected:
    virtual void paintEvent(QPaintEvent * event) override;
};

#endif // WIDGETVIEW_H
