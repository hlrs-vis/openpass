#ifndef TRAFFICSCENE_H
#define TRAFFICSCENE_H

#include <QGraphicsScene>
#include <QAbstractTableModel>
#include <QGraphicsLineItem>
#include "TrajectoryGraphicsItem.h"
#include "globalDefinitions.h"

class TrafficScene : public QGraphicsScene
{
public:
    TrafficScene(QObject *parent = 0);

    void AddTrajectory(QVector<QPointF> *trajectoryData);
    void AddMark(QVector<QPointF> *line, MarkType markType);
    void AddObject(QVector<QPointF> *line, ObjectType objectType);
    void RemoveTrajectory(int index);

private:
    void AddLine(QVector<QPointF> *line, QPen pen);
    QList<TrajectoryGraphicsItem *> trajectories;

    std::vector<int> colors;
};

#endif // TRAFFICSCENE_H
