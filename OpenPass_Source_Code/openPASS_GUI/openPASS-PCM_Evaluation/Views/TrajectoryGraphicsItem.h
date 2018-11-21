/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef TRAJECTORYGRAPHICSITEM_H
#define TRAJECTORYGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>

class TrajectoryGraphicsItem : public QGraphicsItem
{
public:
    TrajectoryGraphicsItem(QVector<QPointF> *trajectoryData, QPen pen, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    QPen pen;

    double minX = 0;
    double minY = 0;
    double maxX = 0;
    double maxY = 0;
};

#endif // TRAJECTORYGRAPHICSITEM_H
