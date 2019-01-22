/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "ViewPcm.h"
#include "ui_ViewPcm.h"
#include "GUI_Definitions.h"

const WindowInterface::ID ViewPcm::ViewID = QStringLiteral("openPASS.PCM");

ViewPcm::ViewPcm(WindowInterface *const window,
                 QWidget *parent)
    : QWidget(parent)
    , window(window)
    , ui(new Ui::ViewPcm)
    , actionMenuStartSimulation(WindowInterface::createAction(tr("Start Simulation"), this,
                                                              &ViewPcm::ActionStartSimulation))
    , actionMenuStopSimulation(WindowInterface::createAction(tr("Stop Simulation"), this,
                                                             &ViewPcm::ActionStopSimulation))
    , actionMenuSaveExperiment(WindowInterface::createAction(tr("Save Experiment"), this,
                                                             &ViewPcm::ActionSaveExperiment))
    , actionMenuLoadExperiment(WindowInterface::createAction(tr("Load Experiment"), this,
                                                             &ViewPcm::ActionLoadExperiment))
{
    // Create UI
    ui->setupUi(this);

    // Customize the menu
    actionMenuStartSimulation->setEnabled(false);
    actionMenuStopSimulation->setEnabled(false);

    actionMenuSaveExperiment->setEnabled(true);
    actionMenuLoadExperiment->setEnabled(true);

    // Customize the view
#ifdef QT_DEBUG
    ui->comboBox_LogLevel->setCurrentIndex(2); // print Error/Warning/Info messages
#else
    ui->comboBox_LogLevel->setCurrentIndex(0); // print Error messages only
#endif

    ui->spinBox_VarCount->setValue(VARIATION_COUNT_DEFAULT);

    // Update the view

    // Signals-Slots connection declaration for the Menu-bar Actions

    // Register view
    window->add(ViewID, WindowInterface::createButton(tr("PCM-Simulation"), 0, 00100), this,
    {actionMenuStartSimulation, actionMenuStopSimulation, actionMenuSaveExperiment, actionMenuLoadExperiment});
    /*!
      * Registering the view in QHash structure
      * allowing creating a button (name, type, position ->00100)
      * allowing connecting with the correspondant widget
      * allowing creating menu ({} -> empty in this case)
      */

    // initialize parent folders of settings in the GUI
    QString appRootFolder = QCoreApplication::applicationDirPath();
    expConfigParentFolder = appRootFolder;
    pcmFileParentFolder = appRootFolder;
    prevResultParentFolder = appRootFolder;
    prevResultParentFolder = appRootFolder;
    resultParentFolder = appRootFolder;
    car1ParentFolder = appRootFolder;
    car2ParentFolder = appRootFolder;
    otherParentFolder = appRootFolder;
}

ViewPcm::~ViewPcm()
{
    // Deregister view
    window->remove(ViewID);

    // Destroy UI
    delete ui;
}

void ViewPcm::SetModelPcm(QAbstractItemModel *model)
{
    ui->listViewPcmCase->setModel(model);
}

QItemSelectionModel *ViewPcm::GetSelectionModelPcm()
{
    return ui->listViewPcmCase->selectionModel();
}

void ViewPcm::SetProgessBarMaximum(int valueMax)
{
    ui->progressBar->setMaximum(valueMax);
}

void ViewPcm::SetProgressBarValue(int value)
{
    ui->progressBar->setValue(value);
}

// freeze or unfreeze the input and output GUI
void ViewPcm::enableIOGUI(bool enabled)
{
    ui->radioButtonPCMDB->setEnabled(enabled);
    ui->radioButtonSimResult->setEnabled(enabled);
    ui->lineEditPcmFile->setEnabled(enabled);
    ui->lineEditPrevResultFolder->setEnabled(enabled);
    ui->lineEditResultFolder->setEnabled(enabled);
    ui->comboBox_LogLevel->setEnabled(enabled);

    ui->lineEditCar1->setEnabled(enabled);
    ui->lineEditCar2->setEnabled(enabled);
    ui->lineEditOther->setEnabled(enabled);
    ui->buttonBrowsePcmFile->setEnabled(enabled);
    ui->buttonBrowsePrevResultFolder->setEnabled(enabled);
    ui->buttonBrowseResultFolder->setEnabled(enabled);
    ui->buttonBrowseCar1->setEnabled(enabled);
    ui->buttonBrowseCar2->setEnabled(enabled);
    ui->buttonBrowseOther->setEnabled(enabled);

    ui->groupBoxVariation->setEnabled(enabled);

    ui->listViewPcmCase->setEnabled(enabled);
}

void ViewPcm::OnSimulationStarted()
{
    actionMenuStartSimulation->setEnabled(false);
    actionMenuStopSimulation->setEnabled(true);
    actionMenuSaveExperiment->setEnabled(false);
    actionMenuLoadExperiment->setEnabled(false);

    // freeze the input and output GUI
    enableIOGUI(false);
}

void ViewPcm::OnSimulationFinished()
{
    actionMenuStartSimulation->setEnabled(true);
    actionMenuStopSimulation->setEnabled(false);
    actionMenuSaveExperiment->setEnabled(true);
    actionMenuLoadExperiment->setEnabled(true);

    // unfreeze the input and output GUI
    enableIOGUI(true);

    ui->progressBar->reset();
}

void ViewPcm::ShowMessage(QString title, QString message)
{
    QMessageBox::information(this, title, message);
}

void ViewPcm::ActionStartSimulation()
{
    if (ui->lineEditOther->text().isEmpty())
    {
        ShowMessage("ERROR", "There is no \"Other System\" specified");
        return;
    }

    if (ui->lineEditCar1->text().isEmpty())
    {
        ShowMessage("ERROR", "There is no \"Car1 System\" specified");
        return;
    }

    if (ui->lineEditCar2->text().isEmpty())
    {
        ShowMessage("ERROR", "There is no \"Car2 System\" specified");
        return;
    }

    Q_EMIT ResultFolderChanged(ui->lineEditResultFolder->text());
    Q_EMIT OtherFileChanged(ui->lineEditOther->text());
    Q_EMIT Car1FileChanged(ui->lineEditCar1->text());
    Q_EMIT Car2FileChanged(ui->lineEditCar2->text());

    Q_EMIT StartSimulation();
}

void ViewPcm::ActionStopSimulation()
{
    Q_EMIT StopSimulation();
}

void ViewPcm::ActionSaveExperiment()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const configFile = QFileDialog::getSaveFileName(
                                   this, tr("openPASS / Save Configuration"), expConfigParentFolder,
                                   QStringLiteral("Config File (*.cfg);;All files (*)"));

    if (configFile.isEmpty())
    {
        return;
    }

    expConfigParentFolder = QFileInfo(configFile).dir().canonicalPath();

    // save experiment settings as a .cfg file
    QSettings settings(configFile, QSettings::IniFormat);
    settings.clear();

    if(ui->radioButtonPCMDB->isChecked())
    {
        settings.setValue(configStringEnvPcmFile, ui->lineEditPcmFile->text());
    }
    else {
        settings.setValue(configStringEnvInputFolder, ui->lineEditPrevResultFolder->text());
    }

    settings.setValue(configStringEnvResultFolder, ui->lineEditResultFolder->text());
    settings.setValue(configStringSysCar1, ui->lineEditCar1->text());
    settings.setValue(configStringSysCar2, ui->lineEditCar2->text());
    settings.setValue(configStringSysOther, ui->lineEditOther->text());

    QModelIndexList selectionList =
        ui->listViewPcmCase->selectionModel()->selectedIndexes();
    settings.beginGroup(configStringSelection);
    QString selectionKey;
    for (QModelIndex selectedIndex : selectionList)
    {
        selectionKey = selectedIndex.data().toString();
        settings.setValue(selectionKey, true);
    }
    settings.endGroup();

    // variation settings
    if(ui->radioButton_RSCase->isChecked())
    {
        settings.setValue(configStringVarRandomSeed, configRandomSeedUsingCase);
    }
    else
    {
        settings.setValue(configStringVarRandomSeed, ui->spinBox_RSValue->text());
    }
    settings.setValue(configStringVarCount, ui->spinBox_VarCount->text());

    if(ui->groupBoxVarPos->isChecked())
    {
        settings.setValue(configStringVarTrajectoryShifting, true);
        settings.setValue(configStringVarShiftRadius1, ui->doubleSpinBox_Radius_1->value());
        settings.setValue(configStringVarShiftRadius2, ui->doubleSpinBox_Radius_2->value());
    }
    else
    {
        settings.setValue(configStringVarTrajectoryShifting, false);
    }

    if(ui->groupBoxVarVelocity->isChecked())
    {
        settings.setValue(configStringVarVelocityScaling, true);
        settings.setValue(configStringVarMaxVelScale1, ui->doubleSpinBox_VelScale_1->value());
        settings.setValue(configStringVarMaxVelScale2, ui->doubleSpinBox_VelScale_2->value());
    }
    else
    {
        settings.setValue(configStringVarVelocityScaling, false);
    }
}

void ViewPcm::ActionLoadExperiment()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const configFile = QFileDialog::getOpenFileName(
                                   this, tr("openPASS / Load Configuration"), expConfigParentFolder,
                                   QStringLiteral("Config File (*.cfg);;All files (*)"));

    if (configFile.isEmpty())
    {
        return;
    }

    expConfigParentFolder = QFileInfo(configFile).dir().canonicalPath();

    // load experiment settings from a .cfg file
    QSettings settings(configFile, QSettings::IniFormat);
    if(settings.contains(configStringEnvPcmFile))
    {
        ui->radioButtonPCMDB->toggle();
        ui->lineEditPcmFile->setText(settings.value(configStringEnvPcmFile).toString());
    }
    else if(settings.contains(configStringEnvInputFolder))
    {
        ui->radioButtonSimResult->toggle();
        ui->lineEditPrevResultFolder->setText(settings.value(configStringEnvInputFolder).toString());
    }

    ui->lineEditResultFolder->setText(settings.value(configStringEnvResultFolder).toString());
    ui->lineEditCar1->setText(settings.value(configStringSysCar1).toString());
    ui->lineEditCar2->setText(settings.value(configStringSysCar2).toString());
    ui->lineEditOther->setText(settings.value(configStringSysOther).toString());

    // clear existing selection
    ui->listViewPcmCase->clearSelection();
    int rowCount = ui->listViewPcmCase->model()->rowCount();

    settings.beginGroup(configStringSelection);
    QStringList selectionKeyList = settings.childKeys();
    for (QString selectionKey : selectionKeyList)
    {
        for(int row=0; row < rowCount; row++)
        {
            QString CaseNumber = ui->listViewPcmCase->model()->index(row, 0).data().toString();
            if (selectionKey.compare(CaseNumber) == 0)  // found the row number of the selected case
            {
                QModelIndex index = ui->listViewPcmCase->model()->index(row, 0);
                ui->listViewPcmCase->selectionModel()->select(index, QItemSelectionModel::Select);
                on_listViewPcmCase_clicked(index);
                break;
            }
        }
    }
    settings.endGroup();

    // load variation settings
    QVariant randomSeed = settings.value(configStringVarRandomSeed);
    if(randomSeed.toString().compare(configRandomSeedUsingCase) == 0)
    {
        ui->radioButton_RSCase->setChecked(true);
        ui->radioButton_RSValue->setChecked(false);
    }
    else
    {
        ui->spinBox_RSValue->setValue(randomSeed.toInt());
        ui->radioButton_RSCase->setChecked(false);
        ui->radioButton_RSValue->setChecked(true);
    }

    ui->spinBox_VarCount->setValue(settings.value(configStringVarCount).toInt());

    if(settings.value(configStringVarTrajectoryShifting).toBool())
    {
        ui->groupBoxVarPos->setChecked(true);
        ui->doubleSpinBox_Radius_1->setValue(settings.value(configStringVarShiftRadius1).toDouble());
        ui->doubleSpinBox_Radius_2->setValue(settings.value(configStringVarShiftRadius2).toDouble());
    }
    else
    {
        ui->groupBoxVarPos->setChecked(false);
    }

    if(settings.value(configStringVarVelocityScaling).toBool())
    {
        ui->groupBoxVarVelocity->setChecked(true);
        ui->doubleSpinBox_VelScale_1->setValue(settings.value(configStringVarMaxVelScale1).toDouble());
        ui->doubleSpinBox_VelScale_2->setValue(settings.value(configStringVarMaxVelScale2).toDouble());
    }
    else
    {
        ui->groupBoxVarVelocity->setChecked(false);
    }
}

void ViewPcm::on_buttonBrowsePcmFile_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                                 this, tr("openPASS / Read input data from a PCM file"), pcmFileParentFolder,
                                 QStringLiteral("PCM File (*.mdb);;All files (*)"));

    if (!filepath.isNull())
    {
        ui->lineEditPcmFile->setText(root.relativeFilePath(filepath));
        pcmFileParentFolder = QFileInfo(filepath).dir().canonicalPath();
    }
}

void ViewPcm::on_buttonBrowsePrevResultFolder_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const dirpath = QFileDialog::getExistingDirectory(
                                 this, tr("openPASS / Read input data from a result folder"), prevResultParentFolder,
                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirpath.isNull())
    {
        ui->lineEditPrevResultFolder->setText(root.relativeFilePath(dirpath));
        prevResultParentFolder = QFileInfo(dirpath).dir().canonicalPath();
    }
}

void ViewPcm::on_buttonBrowseResultFolder_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const dirpath = QFileDialog::getExistingDirectory(
                                 this, tr("openPASS / Output result Folder"), resultParentFolder,
                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dirpath.isNull())
    {
        ui->lineEditResultFolder->setText(root.relativeFilePath(dirpath));
        resultParentFolder = QFileInfo(dirpath).dir().canonicalPath();
    }
}

void ViewPcm::on_buttonBrowseOther_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QStringList fileNames = QFileDialog::getOpenFileNames(
                                this, tr("openPASS / SystemConfig for other participants"), otherParentFolder,
                                QStringLiteral("SystemConfig File (*.xml);;All files (*)"));

    QString listString;
    for (QString fileName : fileNames)
    {
        if (listString.count() != 0)
        {
            listString.append(";");
        }
        listString.append(root.relativeFilePath(fileName));
    }
    ui->lineEditOther->setText(listString);

    if(fileNames.count() > 0)
    {
        otherParentFolder = QFileInfo(fileNames.back()).dir().canonicalPath();
    }
}

void ViewPcm::on_buttonBrowseCar1_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QStringList fileNames = QFileDialog::getOpenFileNames(
                                this, tr("openPASS / SystemConfig for cars"), car1ParentFolder,
                                QStringLiteral("SystemConfig File (*.xml);;All files (*)"));

    QString listString;
    for (QString fileName : fileNames)
    {
        if (listString.count() != 0)
        {
            listString.append(";");
        }
        listString.append(root.relativeFilePath(fileName));
    }
    ui->lineEditCar1->setText(listString);

    if(fileNames.count() > 0)
    {
        car1ParentFolder = QFileInfo(fileNames.back()).dir().canonicalPath();
    }
}

void ViewPcm::on_buttonBrowseCar2_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QStringList fileNames = QFileDialog::getOpenFileNames(
                                this, tr("openPASS / SystemConfig for cars"), car2ParentFolder,
                                QStringLiteral("SystemConfig File (*.xml);;All files (*)"));

    QString listString;
    for (QString fileName : fileNames)
    {
        if (listString.count() != 0)
        {
            listString.append(";");
        }
        listString.append(root.relativeFilePath(fileName));
    }
    ui->lineEditCar2->setText(listString);

    if(fileNames.count() > 0)
    {
        car2ParentFolder = QFileInfo(fileNames.back()).dir().canonicalPath();
    }
}

void ViewPcm::on_lineEditPcmFile_textChanged(const QString &arg1)
{
    Q_EMIT PcmSourceFileChanged(arg1);
}


void ViewPcm::on_lineEditPrevResultFolder_textChanged(const QString &arg1)
{
    Q_EMIT PrevResultFolderChanged(arg1);
}

void ViewPcm::on_lineEditResultFolder_textChanged(const QString &arg1)
{
    Q_EMIT ResultFolderChanged(arg1);
}

void ViewPcm::on_lineEditOther_textChanged(const QString &arg1)
{
    Q_EMIT OtherFileChanged(arg1);
}

void ViewPcm::on_lineEditCar1_textChanged(const QString &arg1)
{
    Q_EMIT Car1FileChanged(arg1);
}

void ViewPcm::on_lineEditCar2_textChanged(const QString &arg1)
{
    Q_EMIT Car2FileChanged(arg1);
}

void ViewPcm::on_listViewPcmCase_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    int count = ui->listViewPcmCase->selectionModel()->selection().count();
    if ( count > 0)
    {
        actionMenuStartSimulation->setEnabled(true);
    }
    else
    {
        actionMenuStartSimulation->setEnabled(false);
    }
}

void ViewPcm::on_radioButtonPCMDB_toggled(bool checked)
{
    if(checked)
    {
        ui->stackPCMData->setCurrentIndex(0);

        Q_EMIT PcmSourceFileChanged(ui->lineEditPcmFile->text());
    }
}

void ViewPcm::on_radioButtonSimResult_toggled(bool checked)
{
    if(checked)
    {
        ui->stackPCMData->setCurrentIndex(1);

        Q_EMIT PrevResultFolderChanged(ui->lineEditPrevResultFolder->text());
    }
}

void ViewPcm::on_comboBox_LogLevel_currentIndexChanged(int index)
{
    Q_EMIT LogLevelChanged(index);
}

void ViewPcm::on_radioButton_RSCase_toggled(bool checked)
{
    if(checked)
    {
        Q_EMIT RandomSeedChanged(-1);
    }
}

void ViewPcm::on_radioButton_RSValue_toggled(bool checked)
{
    if(checked)
    {
        Q_EMIT RandomSeedChanged(ui->spinBox_RSValue->value());
    }
}

void ViewPcm::on_spinBox_VarCount_valueChanged(int value)
{
    Q_EMIT VariationCountChanged(value);
}

void ViewPcm::on_spinBox_RSValue_valueChanged(int value)
{
    Q_EMIT RandomSeedChanged(value);
}

void ViewPcm::on_doubleSpinBox_Radius_1_valueChanged(double radius)
{
    Q_EMIT ShiftRadius1Changed(radius);
}

void ViewPcm::on_doubleSpinBox_Radius_2_valueChanged(double radius)
{
    Q_EMIT ShiftRadius2Changed(radius);
}

void ViewPcm::on_groupBoxVarPos_toggled(bool checked)
{
    Q_EMIT ShiftRadius1Changed((checked ? ui->doubleSpinBox_Radius_1->value() : -1));
    Q_EMIT ShiftRadius2Changed((checked ? ui->doubleSpinBox_Radius_2->value() : -1));
}

void ViewPcm::on_doubleSpinBox_VelScale_1_valueChanged(double maxScale)
{
    Q_EMIT VelocityScale1Changed(maxScale);
}

void ViewPcm::on_doubleSpinBox_VelScale_2_valueChanged(double maxScale)
{
    Q_EMIT VelocityScale2Changed(maxScale);
}

void ViewPcm::on_groupBoxVarVelocity_toggled(bool checked)
{
    Q_EMIT VelocityScale1Changed((checked ? (ui->doubleSpinBox_VelScale_1->value()) : INFINITY));
    Q_EMIT VelocityScale2Changed((checked ? (ui->doubleSpinBox_VelScale_2->value()) : INFINITY));
}




