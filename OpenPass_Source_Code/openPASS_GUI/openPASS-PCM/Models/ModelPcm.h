#ifndef MODELPCM_H
#define MODELPCM_H

#include <QObject>
#include <QStringListModel>
#include <QtConcurrent>
#include "ConfigurationGeneratorPcm/ConfigGeneratorPcm.h"
#include "FileHelper.h"

class ModelPcm : public QObject
{
    Q_OBJECT

public:
    explicit ModelPcm(QObject *parent = nullptr);
    virtual ~ModelPcm();

    bool Clear();

    QAbstractItemModel *GetItemModelPcm() const;
    void SetSelectionModelPcm(QItemSelectionModel *selectionModel);

public Q_SLOTS:
    bool LoadPcmFile(const QString &filepath);
    void SetResultFolder(const QString &resultFolder);
    void SetOtherSystemFile(const QString &otherSystemFile);
    void SetCar1SystemFile(const QString &car1SystemFile);
    void SetCar2SystemFile(const QString &car2SystemFile);
    void StartSimulationTrigger();
    void SimulationStop();

public:
Q_SIGNALS:
    void SimulationProgressMaximum(int valueMax);
    void SimulationProgressChanged(int value);

    void SimulationStarted();
    void SimulationFinished();

    void ShowMessage(QString title, QString message);

private:
    void StartSimulation();

    QStringListModel *listModelPcm = nullptr;
    QItemSelectionModel *selectionModelPcm = nullptr;
    QStringList pcmCaseList;
    QString currentPcmFilePath = "";
    QString baseFolder = "";
    QString resultFolder = "";
    QString otherSystemFile = "";
    QString car1SystemFile = "";
    QString car2SystemFile = "";

    QSqlDatabase db;
    QString connection = "";

    ConfigGenerator *configGenerator;
    DatabaseReader dbReader;

    bool simulationStop = false;
};

#endif // MODELPCM_H
