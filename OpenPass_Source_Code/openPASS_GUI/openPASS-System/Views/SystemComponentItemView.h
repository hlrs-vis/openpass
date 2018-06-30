/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTITEMVIEW_H
#define SYSTEMCOMPONENTITEMVIEW_H

#include "openPASS-System/SystemComponentItemInterface.h"
#include "Views/WidgetView.h"
#include <QMap>
#include <QString>

class SystemComponentInputItemView;
class SystemComponentOutputItemView;
class QKeyEvent;

namespace Ui {
class SystemComponentItemView;
}

class SystemComponentItemView : public WidgetView
{
    Q_OBJECT

public:
    explicit SystemComponentItemView(SystemComponentItemInterface * const component,
                                     QWidget * const parent = nullptr);
    virtual ~SystemComponentItemView();

Q_SIGNALS:
    void moved();

Q_SIGNALS:
    void inputMouseEnter(SystemComponentInputItemView * const input);
    void inputMouseLeave(SystemComponentInputItemView * const input);
    void inputConnectionAdd(SystemComponentInputItemView * const input);
    void inputConnectionRemove(SystemComponentInputItemView * const input);

Q_SIGNALS:
    void outputMouseEnter(SystemComponentOutputItemView * const output);
    void outputMouseLeave(SystemComponentOutputItemView * const output);
    void outputConnectionAdd(SystemComponentOutputItemView * const output);
    void outputConnectionRemove(SystemComponentOutputItemView * const output);

Q_SIGNALS:
    void pressEscape();

public:
    SystemComponentItemInterface * getInterface() const;

protected:
    virtual void keyPressEvent(QKeyEvent * event) override;

public Q_SLOTS:
    void updateType();
    void updatePosition();

protected:
    static QMap<SystemComponentItemInterface::Type, QString> const TypeColorMap;
    static QString const TypeColorDefault;

protected Q_SLOTS:
    void on_renameEdit_returnPressed();
    void on_renameCancel_clicked();
    void on_renameSave_clicked();

protected Q_SLOTS:
    void on_actionRename_clicked();
    void on_actionDuplicate_clicked();
    void on_actionRemove_clicked();

protected:
    SystemComponentItemInterface * const component;

protected:
    Ui::SystemComponentItemView * const ui;
};

#endif // SYSTEMCOMPONENTITEMVIEW_H
