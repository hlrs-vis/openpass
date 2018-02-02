/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTINPUTITEMVIEW_H
#define SYSTEMCOMPONENTINPUTITEMVIEW_H

#include "Views/WidgetView.h"

class SystemComponentInputItemInterface;
class QEvent;

namespace Ui {
class SystemComponentInputItemView;
}

class SystemComponentInputItemView : public WidgetView
{
    Q_OBJECT

public:
    explicit SystemComponentInputItemView(SystemComponentInputItemInterface * const input,
                                          QWidget * const parent = nullptr);
    virtual ~SystemComponentInputItemView();

Q_SIGNALS:
    void mouseEnter(SystemComponentInputItemView * const input);
    void mouseLeave(SystemComponentInputItemView * const input);
    void connectionAdd(SystemComponentInputItemView * const input);
    void connectionRemove(SystemComponentInputItemView * const input);

public:
    SystemComponentInputItemInterface * getInterface() const;

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
    SystemComponentInputItemInterface * const input;

protected:
    Ui::SystemComponentInputItemView * const ui;
};

#endif // SYSTEMCOMPONENTINPUTITEMVIEW_H
