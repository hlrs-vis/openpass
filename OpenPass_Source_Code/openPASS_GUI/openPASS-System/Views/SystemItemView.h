/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMITEMVIEW_H
#define SYSTEMITEMVIEW_H

#include <QPoint>
#include <QScrollArea>
#include <QString>

class SystemComponentManagerInterface;
class SystemItemInterface;
class QDrawEnterEvent;
class QDropEvent;
class QMouseEvent;
class QWheelEvent;

class SystemItemView : public QScrollArea
{
    Q_OBJECT

public:
    explicit SystemItemView(SystemItemInterface * const system,
                            SystemComponentManagerInterface const * const components,
                            QWidget * const parent = nullptr);
    virtual ~SystemItemView() = default;

protected:
    virtual void dragEnterEvent(QDragEnterEvent * event) override;
    virtual void dropEvent(QDropEvent * event) override;

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;

protected:
    virtual void wheelEvent(QWheelEvent * event) override;

protected:
    static QString const DragDropMimeDataFormat;
    static QPoint const MouseInvalidPosition;

protected:
    SystemItemInterface * const system;
    SystemComponentManagerInterface const * const components;

protected:
    QPoint mouseGlobalPosition;
};

#endif // SYSTEMITEMVIEW_H
