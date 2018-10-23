/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMBASEVIEW_H
#define SYSTEMBASEVIEW_H

#include <QColor>
#include <QList>
#include <QMap>
#include <QPair>
#include <QPoint>
#include <QWidget>

class SystemComponentInputItemInterface;
class SystemComponentOutputItemInterface;
class SystemComponentItemInterface;
class SystemItemInterface;
class SystemComponentInputItemView;
class SystemComponentOutputItemView;
class SystemComponentItemView;
class QPaintEvent;

class SystemBaseView : public QWidget
{
    Q_OBJECT

public:
    explicit SystemBaseView(SystemItemInterface * const system,
                            QWidget * const parent = nullptr);
    virtual ~SystemBaseView() = default;

public Q_SLOTS:
    void updateInternalComponents();
    void updateInternalConnections();
    void updateInternalConnectionsCurves();
    void updateSize();

public Q_SLOTS:
    void componentsMode(bool const & modify);
    void componentsUpdate();
    void componentsClear();
    void componentsAdd(SystemComponentItemInterface * const component);
    void componentsRemove(SystemComponentItemInterface * const component);

public Q_SLOTS:
    void connectionsMode(bool const & modify);
    void connectionsUpdate();

public Q_SLOTS:
    void inputMouseEnter(SystemComponentInputItemView * const input);
    void inputMouseLeave(SystemComponentInputItemView * const input);
    void inputConnectionAdd(SystemComponentInputItemView * const input);
    void inputConnectionRemove(SystemComponentInputItemView * const input);

public Q_SLOTS:
    void outputMouseEnter(SystemComponentOutputItemView * const output);
    void outputMouseLeave(SystemComponentOutputItemView * const output);
    void outputConnectionAdd(SystemComponentOutputItemView * const output);
    void outputConnectionRemove(SystemComponentOutputItemView * const output);

public Q_SLOTS:
    void pressEscape();

protected:
    static QPoint const FrameSize;
    static QMap<bool, QColor> const ConnectionsCurveColorMap;

protected:
    virtual void paintEvent(QPaintEvent * event) override;

protected:
    SystemItemInterface * const system;

protected:
    bool componentsModifyMode;
    bool connectionsModifyMode;

protected:
    SystemComponentInputItemView * inputActive;
    SystemComponentInputItemView * inputConnection;
    SystemComponentOutputItemView * outputActive;
    SystemComponentOutputItemView * outputConnection;

protected:
    QMap<SystemComponentItemInterface *, SystemComponentItemView *> internalComponentsMap;
    QMap<SystemComponentInputItemInterface const *, SystemComponentInputItemView *> internalComponentsInputMap;
    QMap<SystemComponentOutputItemInterface const *, SystemComponentOutputItemView *> internalComponentsOutputMap;
    QList<SystemComponentInputItemView const *> internalConnectionsInputList;
    QList<SystemComponentOutputItemView const *> internalConnectionsOutputList;
    QMap<bool, QList<QPair<QPoint, QPoint>>> internalConnectionsCurveMap;
};

#endif // SYSTEMBASEVIEW_H
