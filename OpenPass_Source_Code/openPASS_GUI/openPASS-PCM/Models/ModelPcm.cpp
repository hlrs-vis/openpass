/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "Models/ModelPcm.h"

ModelPcm::ModelPcm(QObject *parent)
    : QObject(parent)
{
    listModelPcm = new QStringListModel(this);
    listModelPcm->setStringList(pcmCaseList);

    baseFolder = QCoreApplication::applicationDirPath();

    configGenerator = new ConfigGenerator(baseFolder);
}

ModelPcm::~ModelPcm()
{
    Clear();
    if (listModelPcm != nullptr)
    {
        delete listModelPcm;
    }

    delete configGenerator;
}

QAbstractItemModel *ModelPcm::GetItemModelPcm() const
{
    return listModelPcm;
}

void ModelPcm::SetSelectionModelPcm(QItemSelectionModel *selectionModel)
{
    selectionModelPcm = selectionModel;
}

void ModelPcm::StartSimulationTrigger()
{
    simulationStop = false;
    QtConcurrent::run(this, StartSimulation);
}

void ModelPcm::SimulationStop()
{
    simulationStop = true;
}

bool ModelPcm::Clear()
{
    pcmCaseList.clear();
    listModelPcm->setStringList(pcmCaseList);

    dbReader.CloseDataBase();

    return true;
}

bool ModelPcm::LoadPcmFile(const QString &pcmFilePath)
{
    Clear();

    currentPcmFilePath = pcmFilePath;
    dbReader.SetDatabase(currentPcmFilePath);
    bool success = dbReader.OpenDataBase();

    if (success)
    {
        success = dbReader.ReadCaseList(pcmCaseList);
    }

    if (success)
    {
        listModelPcm->setStringList(pcmCaseList);
        success = listModelPcm != nullptr;
    }

    dbReader.CloseDataBase();

    return success;
}

void ModelPcm::SetResultFolder(const QString &resultFolder)
{
    this->resultFolder = resultFolder;
}

void ModelPcm::SetOtherSystemFile(const QString &otherSystemFile)
{
    this->otherSystemFile = otherSystemFile;
}

void ModelPcm::SetCar1SystemFile(const QString &car1SystemFile)
{
    this->car1SystemFile = car1SystemFile;
}

void ModelPcm::SetCar2SystemFile(const QString &car2SystemFile)
{
    this->car2SystemFile = car2SystemFile;
}

void ModelPcm::StartSimulation()
{
    Q_EMIT SimulationStarted();

    QString baseFolder = QCoreApplication::applicationDirPath();

    QModelIndexList pcmCaseIndexList = selectionModelPcm->selectedIndexes();
    QStringList otherSytemList = otherSystemFile.split(";");
    QStringList car1SystemList = car1SystemFile.split(";");
    QStringList car2SystemList = car2SystemFile.split(";");

    Q_EMIT SimulationProgressMaximum(pcmCaseIndexList.count()
                                     * otherSytemList.count()
                                     * car1SystemList.count()
                                     * car2SystemList.count()
                                     * 2);

    int progress = 0;
    Q_EMIT SimulationProgressChanged(progress++);

    configGenerator->Clear();

    for (QModelIndex &pcmCaseIndex : pcmCaseIndexList)
    {
        if (simulationStop)
        {
            Q_EMIT ShowMessage("Information", "Simulation aborted.");
            Q_EMIT SimulationFinished();
            return;
        }

        int otherSystemCount = 0;
        for (QString otherSystem : otherSytemList)
        {

            if (otherSystem.isEmpty())
            {
                continue;
            }
            int car1SystemCount = 0;
            for (QString car1System : car1SystemList)
            {
                if (car1System.isEmpty())
                {
                    continue;
                }
                int car2SystemCount = 0;
                for (QString car2System : car2SystemList)
                {
                    if (car2System.isEmpty())
                    {
                        continue;
                    }
                    QString pcmCase = QString("%1").arg(pcmCaseIndex.data().toInt());
                    QString configPath = resultFolder + "/"
                            + pcmCase
                            + "-" + QString::number(otherSystemCount)
                            + "-" + QString::number(car1SystemCount)
                            + "-" + QString::number(car2SystemCount);
                    QString resultPathPcmCase = configPath + "/Results";

                    QDir configDir(configPath);
                    if (configDir.exists())
                    {
                        if(!configDir.removeRecursively())
                        {
                            qDebug("\t Error when deleting the directgory %s", configPath.toStdString().c_str());
                            return;
                        }
                    }

                    if(!configDir.mkpath(configDir.absolutePath()))
                    {
                         qDebug("\t Error when creating the directgory %s", configPath.toStdString().c_str());
                        return;
                    }

//                    qDebug("\t GenerateConfigs for pcmCase: %d at %s", pcmCaseIndex.data().toInt(), configPath.toStdString().c_str());
                    if (!configGenerator->GenerateConfigs(dbReader,
                                                          pcmCase,
                                                          configPath,
                                                          otherSystem,
                                                          car1System,
                                                          car2System,
                                                          resultPathPcmCase))
                    {
                        Q_EMIT ShowMessage("ERROR",
                                           "Failed to generate configuration file for case: " + pcmCase);
                        Q_EMIT SimulationFinished();
                        return;
                    }
                    car2SystemCount++;

                    Q_EMIT SimulationProgressChanged(progress++);
                }
                car1SystemCount++;
            }
            otherSystemCount++;
        }
    }

    const QString frameworkConfigFile = configGenerator->GenerateFrameworkConfig(resultFolder);
    if (frameworkConfigFile == "")
    {
        Q_EMIT ShowMessage("ERROR", "Failed to generate framework configuration file");
        Q_EMIT SimulationFinished();
        return;
    }
    QString masterPath = baseFolder + "/OpenPassMaster.exe";
    QProcess *masterProcess = new QProcess();
    QStringList arguments;
    arguments << "--frameworkConfigFile" << frameworkConfigFile;

    masterProcess->start(masterPath, arguments);
    masterProcess->waitForFinished(-1);
    if (masterProcess->exitCode() != 0)
    {
        Q_EMIT ShowMessage("ERROR", "Simulation aborted. Master returned with -1");
        Q_EMIT SimulationFinished();
        delete masterProcess;
        return;
    }
    delete masterProcess;

    Q_EMIT SimulationProgressChanged(progress++);

    Q_EMIT ShowMessage("Information", "Simulation successfully finished.");
    Q_EMIT SimulationFinished();
}
