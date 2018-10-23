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
