#ifndef MODELPCM_EVAL_H
#define MODELPCM_EVAL_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <QStringListModel>
#include <QItemSelectionModel>
#include "TableModelCsv.h"
#include "sceneryImporterPCM.h"

class ModelPcm_Eval : public QObject
{
    Q_OBJECT

public:
    explicit ModelPcm_Eval(QObject *parent = nullptr);
    virtual ~ModelPcm_Eval();

    QAbstractItemModel *GetItemModelPcm() const;
    void SetSelectionModelPcm(QItemSelectionModel *selectionModel);

public:
Q_SIGNALS:
    void TableModelCreated(QAbstractTableModel *newModel, const QString &tableName) const;
    void TrajectoryCreated(QVector<QPointF> *trajectoryData);
    void MarksCreated(QVector<QPointF> *marksData, MarkType markType);
    void ObjectCreated(QVector<QPointF> *objectData, ObjectType objectType);

    void Clear();

public Q_SLOTS:
    bool LoadFileData(const QString &fileName);
    bool LoadSceneryData(const QString &sceneryFile);
    void LoadPathData(const QString &pathName);

private Q_SLOTS:
    void OnSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    QVector<QPointF> ConvertPcmLine(const PCM_Line *pcmLine, QVector<QPointF> &line);
    QMap<QString, TableModelCsv *> tableMap;

    QStringListModel *listModelPcm = nullptr;
    QItemSelectionModel *selectionModelPcm = nullptr;
    QModelIndexList selectedIndexList;

    QStringList resultList;
    QString rootPath = "";
};

#endif // MODELPCM_EVAL_H
