/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemItemView.h"

#include "openPASS-System/SystemComponentManagerInterface.h"
#include "openPASS-System/SystemItemInterface.h"
#include "Views/SystemBaseView.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QModelIndex>
#include <QMouseEvent>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QVariant>
#include <QWheelEvent>

QString const SystemItemView::DragDropMimeDataFormat =
        QStringLiteral("application/x-qabstractitemmodeldatalist");
QPoint const SystemItemView::MouseInvalidPosition = QPoint(-1, -1);

SystemItemView::SystemItemView(SystemItemInterface * const system,
                               SystemComponentManagerInterface const * const components,
                               QWidget * const parent)
    : QScrollArea(parent)
    , system(system)
    , components(components)

{
    setAcceptDrops(true);
    setFrameShape(QFrame::NoFrame);
    setWidget(new SystemBaseView(system, this));
}

void SystemItemView::dragEnterEvent(QDragEnterEvent * event)
{
    if (event->mimeData()->hasFormat(DragDropMimeDataFormat))
    {
        event->acceptProposedAction();
    }
}

void SystemItemView::dropEvent(QDropEvent * event)
{
    if (event->mimeData()->hasFormat(DragDropMimeDataFormat))
    {
        QStandardItemModel * const item = new QStandardItemModel();
        if (item->dropMimeData(event->mimeData(), Qt::CopyAction, 0, 0, QModelIndex()))
        {
            system->getComponents()->add(components->lookupItemByTitle(
                                             item->data(item->index(0, 0)).toString()),
                                         widget()->mapFromParent(event->pos()));
        }
        delete item;
        event->accept();
    }
}

void SystemItemView::mousePressEvent(QMouseEvent * event)
{
    mouseGlobalPosition = MouseInvalidPosition;
    if (event->button() == Qt::RightButton)
    {
        mouseGlobalPosition = event->globalPos();
    }
}

void SystemItemView::mouseMoveEvent(QMouseEvent * event)
{
    if (mouseGlobalPosition != MouseInvalidPosition)
    {
        QPoint const mouseDelta = (event->globalPos() - mouseGlobalPosition) * 2;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - mouseDelta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - mouseDelta.y());
        mouseGlobalPosition = event->globalPos();
    }
}

void SystemItemView::wheelEvent(QWheelEvent * event)
{
    event->ignore();
}
