/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef VIEWPCM_H
#define VIEWPCM_H

#include "openPASS-Window/WindowInterface.h"

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QSettings>

namespace Ui {
class ViewPcm;
}

class ViewPcm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewPcm(WindowInterface *const window,
                     QWidget *parent = 0);
    virtual ~ViewPcm();

    void SetModelPcm(QAbstractItemModel *model);
    QItemSelectionModel *GetSelectionModelPcm();

public Q_SLOTS:
    void SetProgessBarMaximum(int valueMax);
    void SetProgressBarValue(int value);
    void OnSimulationStarted();
    void OnSimulationFinished();

    void ShowMessage(QString title, QString message);

public:
Q_SIGNALS:
    void PcmSourceFileChanged(const QString &newSourceFile) const;
    void PrevResultFolderChanged(const QString &prevResultFolder) const;
    void ResultFolderChanged(const QString &newResultFolder) const;
    void LogLevelChanged(const int level) const;
    void OtherFileChanged(const QString &newResultFolder) const;
    void Car1FileChanged(const QString &newResultFolder) const;
    void Car2FileChanged(const QString &newResultFolder) const;
    void RandomSeedChanged(const int seed) const;
    void VariationCountChanged(const int varCount) const;
    void ShiftRadius1Changed(const double radius) const;
    void ShiftRadius2Changed(const double radius) const;
    void VelocityScale1Changed(const double maxScale) const;
    void VelocityScale2Changed(const double maxScale) const;
    void StartSimulation() const;
    void StopSimulation() const;

private:
    static WindowInterface::ID const ViewID;
    WindowInterface *const window;
    Ui::ViewPcm *ui;

    WindowInterface::Action *const actionMenuStartSimulation;
    WindowInterface::Action *const actionMenuStopSimulation;
    WindowInterface::Action *const actionMenuSaveExperiment;
    WindowInterface::Action *const actionMenuLoadExperiment;

    const QString configStringEnvPcmFile = "Environment/PCMFile";
    const QString configStringEnvInputFolder = "Environment/InputFolder";
    const QString configStringEnvResultFolder = "Environment/ResultFolder";
    const QString configStringSysCar1 = "System/Car1";
    const QString configStringSysCar2 = "System/Car2";
    const QString configStringSysOther = "System/Other";
    const QString configStringSelection = "Selection";
    const QString configStringVarRandomSeed = "Variation/RandomSeed";
    const QString configStringVarCount = "Variation/Count";
    const QString configStringVarTrajectoryShifting = "Variation/TrajectoryShifting";
    const QString configStringVarShiftRadius1 = "Variation/ShiftRadius1";
    const QString configStringVarShiftRadius2 = "Variation/ShiftRadius2";
    const QString configStringVarVelocityScaling = "Variation/VelocityScaling";
    const QString configStringVarMaxVelScale1 = "Variation/MaxVelocityScale1";
    const QString configStringVarMaxVelScale2 = "Variation/MaxVelocityScale2";

    const QString configRandomSeedUsingCase = "Case_Number";

    QString expConfigParentFolder;
    QString pcmFileParentFolder;
    QString prevResultParentFolder;
    QString resultParentFolder;
    QString car1ParentFolder;
    QString car2ParentFolder;
    QString otherParentFolder;

private Q_SLOTS:
    void ActionStartSimulation();
    void ActionStopSimulation();
    void ActionSaveExperiment();
    void ActionLoadExperiment();

    void on_buttonBrowsePcmFile_clicked();
    void on_buttonBrowseResultFolder_clicked();
    void on_buttonBrowseOther_clicked();
    void on_buttonBrowseCar1_clicked();
    void on_buttonBrowseCar2_clicked();
    void on_lineEditPcmFile_textChanged(const QString &arg1);
    void on_lineEditResultFolder_textChanged(const QString &arg1);
    void on_lineEditOther_textChanged(const QString &arg1);
    void on_lineEditCar1_textChanged(const QString &arg1);
    void on_lineEditCar2_textChanged(const QString &arg1);
    void on_listViewPcmCase_clicked(const QModelIndex &index);

    void on_radioButtonPCMDB_toggled(bool checked);
    void on_radioButtonSimResult_toggled(bool checked);

    void enableIOGUI(bool enabled);
    void on_buttonBrowsePrevResultFolder_clicked();
    void on_lineEditPrevResultFolder_textChanged(const QString &arg1);
    void on_comboBox_LogLevel_currentIndexChanged(int index);
    void on_radioButton_RSCase_toggled(bool checked);
    void on_radioButton_RSValue_toggled(bool checked);
    void on_spinBox_VarCount_valueChanged(int value);
    void on_spinBox_RSValue_valueChanged(int value);
    void on_doubleSpinBox_Radius_1_valueChanged(double radius);
    void on_doubleSpinBox_Radius_2_valueChanged(double radius);
    void on_groupBoxVarPos_toggled(bool checked);
    void on_doubleSpinBox_VelScale_1_valueChanged(double maxScale);
    void on_doubleSpinBox_VelScale_2_valueChanged(double maxScale);
    void on_groupBoxVarVelocity_toggled(bool checked);
};

#endif // VIEWPCM_H
