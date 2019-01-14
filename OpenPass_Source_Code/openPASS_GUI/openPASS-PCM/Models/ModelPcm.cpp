/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "ModelPcm.h"
#include "GUI_Definitions.h"

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
    ClearCaseList();
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

    //todo: terminate running simulation threads if existing
}

bool ModelPcm::ClearCaseList()
{
    pcmCaseList.clear();
    listModelPcm->setStringList(pcmCaseList);
    return true;
}

bool ModelPcm::LoadCasesFromPcmFile(const QString &pcmFilePath)
{
    inputFromPCMDB = true;
    ClearCaseList();

    dbReader.CloseDataBase();
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

bool ModelPcm::LoadCasesFromPrevResult(const QString &prevResultFolder)
{
    inputFromPCMDB = false;
    ClearCaseList();

    if(prevResultFolder.length()>0 && QDir(prevResultFolder).exists())
    {
        this->prevResultFolder = prevResultFolder;

        // show case ids in listModelPcm
        QDirIterator it(prevResultFolder, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        while (it.hasNext())
        {
            QString caseNumber = QFileInfo(it.next()).baseName(); // get the base name of the subfolder without path prefix
            if(QRegExp(REGEX_CASE_NUMBER).exactMatch(caseNumber)) // check if the subfolder name is numeric
            {
                pcmCaseList.append(caseNumber);
            }
        }
        listModelPcm->setStringList(pcmCaseList);
        return true;
    }
    return false;
}

void ModelPcm::SetResultFolder(const QString &resultFolder)
{
    this->resultFolder = resultFolder;
}

void ModelPcm::SetLogLevel(const int level)
{
    this->logLevel = level;
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

void ModelPcm::SetInitRandomSeed(const int seed)
{
    this->initRandomSeed = seed;
}

void ModelPcm::SetVariationCount(const int varCount)
{
    this->variationCount = varCount;
}

void ModelPcm::SetShiftRadius1(const double radius)
{
    this->shiftRadius1 = radius;
}

void ModelPcm::SetShiftRadius2(const double radius)
{
    this->shiftRadius2 = radius;
}

void ModelPcm::SetVelocityScale1(const double maxScale)
{
    this->velocityMaxScale1 = maxScale;
}

void ModelPcm::SetVelocityScale2(const double maxScale)
{
    this->velocityMaxScale2 = maxScale;
}

void ModelPcm::StartSimulation()
{
    Q_EMIT SimulationStarted();

    if(resultFolder.length() <= 0)
    {
        Q_EMIT ShowMessage("ERROR", "Invalid result directory [" + resultFolder + "]");
        Q_EMIT SimulationFinished();
        return;
    }

//    //when reading input from previous results, forbid the result folder and previous result folder to be the same
//    if((!inputFromPCMDB) && (resultFolder.compare(prevResultFolder) == 0))
//    {
//        Q_EMIT ShowMessage("ERROR", "The result directory [" + resultFolder + "] should be different from the previous result folder");
//        Q_EMIT SimulationFinished();
//        return;
//    }

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
                    QString systemName = QString::number(otherSystemCount)
                                        + "-" + QString::number(car1SystemCount)
                                        + "-" + QString::number(car2SystemCount);
                    QString caseSystemFolder = resultFolder + "/" + pcmCase + "/" + systemName;

                    for(int varIndex = 0; varIndex <= variationCount; varIndex++)
                    {
                        QString varName = (varIndex == 0) ? DIR_NO_VARIATION :
                                                QString("Var_%1").arg(varIndex, 5, 10, QChar('0')); // zero padding if var index less than 5 digits
                        QString caseSystemVarFolder = caseSystemFolder + "/" + varName;

                        QDir caseSystemVarDir(caseSystemVarFolder);
                        if (caseSystemVarDir.exists())
                        {
                            if(!caseSystemVarDir.removeRecursively())
                            {
                                Q_EMIT ShowMessage("ERROR", "Failed to delete directory " + caseSystemVarFolder + " due to access control from another program");
                                Q_EMIT SimulationFinished();
                                return;
                            }
                        }
                        if(!caseSystemVarDir.mkpath(caseSystemVarDir.absolutePath()))
                        {
                            Q_EMIT ShowMessage("ERROR", "Failed to create directory " + caseSystemVarFolder);
                            Q_EMIT SimulationFinished();
                            return;
                        }

                        // the random seed uses PCM case number if the inital seed is negative. Otherwise it uses the inital seed.
                        int randomSeed = (initRandomSeed < 0) ? (pcmCaseIndex.data().toInt() + varIndex)
                                                              : (initRandomSeed + varIndex);

//                        qDebug("StartSimulation: var %s with random seed: %d", qPrintable(varName), randomSeed);

                        if(inputFromPCMDB)
                        {
                            if (!configGenerator->GenerateConfigFromDB(dbReader,
                                      pcmCase,
                                      caseSystemVarFolder,
                                      otherSystem, car1System, car2System,
                                      randomSeed,
                                      (varIndex == 0)?std::vector<double>{-1, -1}: std::vector<double>{shiftRadius1, shiftRadius2},
                                      (varIndex == 0)?std::vector<double>{INFINITY, INFINITY}: std::vector<double>{velocityMaxScale1, velocityMaxScale2}))
                            {
                                Q_EMIT ShowMessage("ERROR", "Failed to generate configuration file for case: " + pcmCase);
                                Q_EMIT SimulationFinished();
                                return;
                            }
                        }
                        else
                        {
                            // Generate Configs from previous result folder
                            QString prevCaseFolder = prevResultFolder + "/" + pcmCase;
                            QString prevCaseSystemVarFolder, prevSystemName;

                            QDirIterator it(prevCaseFolder, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
                            while (it.hasNext())
                            {
                                prevSystemName = QFileInfo(it.next()).baseName(); // get the base name of the subfolder without path prefix
                                if(QRegExp(REGEX_CASE_SYSTEM).exactMatch(prevSystemName)) // check if the subfolder name matches the pattern like "0-0-0"
                                {
                                    prevCaseSystemVarFolder = prevCaseFolder + "/" + prevSystemName + "/" + DIR_NO_VARIATION;
                                    QDir prevCaseSystemVarDir(prevCaseSystemVarFolder);
                                    if (prevCaseSystemVarDir.exists())
                                    {
                                        break; // found a valid previous result folder
                                    }

                                }
                            }

                            if(prevCaseSystemVarFolder.isEmpty())
                            {
                                Q_EMIT ShowMessage("ERROR", "No valid case result folder in previous results for case " + pcmCase);
                                Q_EMIT SimulationFinished();
                                return;
                            }

                            if (!configGenerator->GenerateConfigFromPrevResult(prevCaseSystemVarFolder,
                                      caseSystemVarFolder,
                                      otherSystem, car1System, car2System,
                                      randomSeed,
                                      (varIndex == 0)?std::vector<double>{-1, -1}: std::vector<double>{shiftRadius1, shiftRadius2},
                                      (varIndex == 0)?std::vector<double>{INFINITY, INFINITY}: std::vector<double>{velocityMaxScale1, velocityMaxScale2}))
                            {
                                Q_EMIT ShowMessage("ERROR", "Failed to generate configuration files for case: " + pcmCase);
                                Q_EMIT SimulationFinished();
                                return;
                            }
                        }
                    }

                    car2SystemCount++;

                    Q_EMIT SimulationProgressChanged(progress++);
                }
                car1SystemCount++;
            }
            otherSystemCount++;
        }
    }

    const QString frameworkConfigFile = configGenerator->GenerateFrameworkConfig(resultFolder, logLevel);
    if (frameworkConfigFile == "")
    {
        Q_EMIT ShowMessage("ERROR", "Failed to generate framework configuration file");
        Q_EMIT SimulationFinished();
        return;
    }

    // execute the master process
    QString masterPath = baseFolder + "/" + FILENAME_OPENPASSMASTER_EXE;
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
