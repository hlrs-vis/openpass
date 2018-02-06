#ifndef VIEWPCM_EVAL_H
#define VIEWPCM_EVAL_H

#include "openPASS-Window/WindowInterface.h"

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QAbstractItemModel>
#include <QTabWidget>
#include <QTableView>
#include "TrafficScene.h"

namespace Ui {
class ViewPcm_Eval;
}

class ViewPcm_Eval : public QWidget
{
    Q_OBJECT

public:
    explicit ViewPcm_Eval(WindowInterface *const window,
                          QWidget *parent = 0);
    virtual ~ViewPcm_Eval();

    void SetModelPcm(QAbstractItemModel *model);
    QItemSelectionModel *GetSelectionModelPcm();

public Q_SLOTS:
    void AddTableModel(QAbstractTableModel *tableModel, const QString &tableName);
    void AddTrajectory(QVector<QPointF> *trajectoryData);
    void AddMarks(QVector<QPointF> *marksData, MarkType markType);
    void AddObject(QVector<QPointF> *objectData, ObjectType objectType);
    void OnClear();

public:
Q_SIGNALS:
    void ResultFileChanged(const QString &newResultFile) const;
    void ResultPathChanged(const QString &newResultPath) const;

private Q_SLOTS:
    void ActionBrowse();
    void OnTabClose(int index);

private:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void showEvent(QShowEvent *event);

    static WindowInterface::ID const ViewID;
    WindowInterface *const window;
    Ui::ViewPcm_Eval *ui;
    WindowInterface::Action *const actionMenuBrowse;

    QTabWidget *tabwidget = nullptr;
    TrafficScene *trafficScene = nullptr;

};

#endif // VIEWPCM_EVAL_H
