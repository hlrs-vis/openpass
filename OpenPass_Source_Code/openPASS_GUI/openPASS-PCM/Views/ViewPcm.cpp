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

void ViewPcm::OnSimulationStarted()
{
    actionMenuStartSimulation->setEnabled(false);
    actionMenuStopSimulation->setEnabled(true);
    actionMenuSaveExperiment->setEnabled(false);
    actionMenuLoadExperiment->setEnabled(false);

    ui->buttonBrowsePcmFile->setEnabled(false);
    ui->buttonBrowseResultFolder->setEnabled(false);
    ui->listViewPcmCase->setEnabled(false);
}

void ViewPcm::OnSimulationFinished()
{
    actionMenuStartSimulation->setEnabled(true);
    actionMenuStopSimulation->setEnabled(false);
    actionMenuSaveExperiment->setEnabled(true);
    actionMenuLoadExperiment->setEnabled(true);

    ui->buttonBrowsePcmFile->setEnabled(true);
    ui->buttonBrowseResultFolder->setEnabled(true);
    ui->listViewPcmCase->setEnabled(true);
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
                                   this, tr("openPASS / Save Configuration"), root.canonicalPath(),
                                   QStringLiteral("Config File (*.cfg);;All files (*)"));

    if (configFile.isEmpty())
    {
        return;
    }

    QSettings settings(configFile, QSettings::IniFormat);
    settings.clear();
    settings.setValue(configStringEnvPcmFile, ui->lineEditPcmFile->text());
    settings.setValue(configStringEnvResultFolder, ui->lineEditResultFolder->text());
    settings.setValue(configStringSysCar1, ui->lineEditCar1->text());
    settings.setValue(configStringSysCar2, ui->lineEditCar2->text());
    settings.setValue(configStringSysOther, ui->lineEditOther->text());

    QModelIndexList selectionList =
        ui->listViewPcmCase->selectionModel()->selectedIndexes();
    int count = 0;
    settings.beginGroup(configStringSelection);
    for (QModelIndex selectedIndex : selectionList)
    {
        settings.setValue("id_" +  QString::number(count++), selectedIndex.row());
    }
    settings.endGroup();

}

void ViewPcm::ActionLoadExperiment()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const configFile = QFileDialog::getOpenFileName(
                                   this, tr("openPASS / Load Configuration"), root.canonicalPath(),
                                   QStringLiteral("Config File (*.cfg);;All files (*)"));

    if (configFile.isEmpty())
    {
        return;
    }

    QSettings settings(configFile, QSettings::IniFormat);
    ui->lineEditPcmFile->setText(settings.value(configStringEnvPcmFile).toString());
    ui->lineEditResultFolder->setText(settings.value(configStringEnvResultFolder).toString());
    ui->lineEditCar1->setText(settings.value(configStringSysCar1).toString());
    ui->lineEditCar2->setText(settings.value(configStringSysCar2).toString());
    ui->lineEditOther->setText(settings.value(configStringSysOther).toString());

    settings.beginGroup(configStringSelection);
    QStringList selectionKeyList = settings.childKeys();
    for (QString selectionKey : selectionKeyList)
    {
        int row = settings.value(selectionKey).toInt();
        QModelIndex index = ui->listViewPcmCase->model()->index(row, 0);
        if (index.isValid())
        {
            ui->listViewPcmCase->selectionModel()->select(index, QItemSelectionModel::Select);
            on_listViewPcmCase_clicked(index);
        }
    }

}

void ViewPcm::on_buttonBrowsePcmFile_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                                 this, tr("openPASS / Load PCM file"), root.canonicalPath(),
                                 QStringLiteral("PCM File (*.mdb);;All files (*)"));

    if (!filepath.isNull())
    {
        ui->lineEditPcmFile->setText(root.relativeFilePath(filepath));
    }
}

void ViewPcm::on_buttonBrowseResultFolder_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getExistingDirectory(
                                 this, tr("Open Result Folder"), root.canonicalPath(),
                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!filepath.isNull())
    {
        ui->lineEditResultFolder->setText(root.relativeFilePath(filepath));
    }
}

void ViewPcm::on_buttonBrowseOther_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QStringList fileNames = QFileDialog::getOpenFileNames(
                                this, tr("openPASS / SystemConfig for other participants"), root.canonicalPath(),
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
}

void ViewPcm::on_buttonBrowseCar1_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QStringList fileNames = QFileDialog::getOpenFileNames(
                                this, tr("openPASS / SystemConfig for cars"), root.canonicalPath(),
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
}

void ViewPcm::on_buttonBrowseCar2_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QStringList fileNames = QFileDialog::getOpenFileNames(
                                this, tr("openPASS / SystemConfig for cars"), root.canonicalPath(),
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
}

void ViewPcm::on_lineEditPcmFile_textChanged(const QString &arg1)
{
    Q_EMIT PcmSourceFileChanged(arg1);
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
