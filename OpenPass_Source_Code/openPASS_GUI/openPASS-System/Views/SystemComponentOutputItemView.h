/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTOUTPUTITEMVIEW_H
#define SYSTEMCOMPONENTOUTPUTITEMVIEW_H

#include "Views/WidgetView.h"

class SystemComponentOutputItemInterface;
class QEvent;

namespace Ui {
class SystemComponentOutputItemView;
}

class SystemComponentOutputItemView : public WidgetView
{
    Q_OBJECT

public:
    explicit SystemComponentOutputItemView(SystemComponentOutputItemInterface * const output,
                                           QWidget * const parent = nullptr);
    virtual ~SystemComponentOutputItemView();

Q_SIGNALS:
    void mouseEnter(SystemComponentOutputItemView * const output);
    void mouseLeave(SystemComponentOutputItemView * const output);
    void connectionAdd(SystemComponentOutputItemView * const output);
    void connectionRemove(SystemComponentOutputItemView * const output);

public:
    SystemComponentOutputItemInterface * getInterface() const;

public:
    void setAddChecked(bool const & checked);
    void setAddEnabled(bool const & enabled);

protected:
    virtual void enterEvent(QEvent * event) override;
    virtual void leaveEvent(QEvent * event) override;

protected Q_SLOTS:
    void on_add_clicked();
    void on_remove_clicked();

protected:
    SystemComponentOutputItemInterface * const output;

protected:
    Ui::SystemComponentOutputItemView * const ui;
};

#endif // SYSTEMCOMPONENTOUTPUTITEMVIEW_H
