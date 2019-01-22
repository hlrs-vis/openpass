/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef MODELPCM_H
#define MODELPCM_H

#include <QObject>
#include <QStringListModel>
#include <QtConcurrent>
#include "ConfigurationGeneratorPcm/ConfigGeneratorPcm.h"
#include "FileHelper.h"
#include "GUI_Definitions.h"

class ModelPcm : public QObject
{
    Q_OBJECT

public:
    explicit ModelPcm(QObject *parent = nullptr);
    virtual ~ModelPcm();

    bool ClearCaseList();

    QAbstractItemModel *GetItemModelPcm() const;
    void SetSelectionModelPcm(QItemSelectionModel *selectionModel);

public Q_SLOTS:
    bool LoadCasesFromPcmFile(const QString &pcmFilePath);
    bool LoadCasesFromPrevResult(const QString &resDirPath);
    void SetResultFolder(const QString &resultFolder);
    void SetLogLevel(const int level);
    void SetOtherSystemFile(const QString &otherSystemFile);
    void SetCar1SystemFile(const QString &car1SystemFile);
    void SetCar2SystemFile(const QString &car2SystemFile);

    void SetInitRandomSeed(const int seed);
    void SetVariationCount(const int varCount);

    void SetShiftRadius1(const double radius);
    void SetShiftRadius2(const double radius);
    void SetVelocityScale1(const double maxScale);
    void SetVelocityScale2(const double maxScale);

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
    QString prevResultFolder = "";
    QString resultFolder = "";

#ifdef QT_DEBUG
    int logLevel = 2;
#else
    int logLevel = 0;
#endif

    QString otherSystemFile = "";
    QString car1SystemFile = "";
    QString car2SystemFile = "";

    int initRandomSeed = INIT_RANDOM_SEED; // if the seed=-1, then the random seed is actually the case number
    int variationCount = VARIATION_COUNT_DEFAULT;

    double shiftRadius1 = SHIFT_RADIUS_CAR1;
    double shiftRadius2 = SHIFT_RADIUS_CAR2;
    double velocityMaxScale1 = VELOCITY_SCALE_CAR1;
    double velocityMaxScale2 = VELOCITY_SCALE_CAR2;

    QSqlDatabase db;
    QString connection = "";

    ConfigGenerator *configGenerator;
    DatabaseReader dbReader;

    bool simulationStop = false;
    bool inputFromPCMDB = true;
};

#endif // MODELPCM_H
