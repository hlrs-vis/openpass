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
    void ResultFolderChanged(const QString &newResultFolder) const;
    void OtherFileChanged(const QString &newResultFolder) const;
    void Car1FileChanged(const QString &newResultFolder) const;
    void Car2FileChanged(const QString &newResultFolder) const;
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

    const QString configStringEnvPcmFile = "Enviroment/File";
    const QString configStringEnvResultFolder = "Enviroment/ResultFolder";
    const QString configStringSysCar1 = "System/Car1";
    const QString configStringSysCar2 = "System/Car2";
    const QString configStringSysOther = "System/Other";
    const QString configStringSelection = "Selection";

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

};

#endif // VIEWPCM_H
