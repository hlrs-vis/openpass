#include "Presenters/PresenterPcm.h"

#include "Models/ModelPcm.h"

PresenterPcm::PresenterPcm(ModelPcm *modelPcm, ViewPcm *viewPcm,
                           QObject *parent)
    : QObject(parent)
    , modelPcm(modelPcm)
    , viewPcm(viewPcm)
{
    viewPcm->SetModelPcm(modelPcm->GetItemModelPcm());
    modelPcm->SetSelectionModelPcm(viewPcm->GetSelectionModelPcm());

    connect(viewPcm, &ViewPcm::PcmSourceFileChanged,
            modelPcm, &ModelPcm::LoadPcmFile);

    connect(viewPcm, &ViewPcm::ResultFolderChanged,
            modelPcm, &ModelPcm::SetResultFolder);

    connect(viewPcm, &ViewPcm::OtherFileChanged,
            modelPcm, &ModelPcm::SetOtherSystemFile);

    connect(viewPcm, &ViewPcm::Car1FileChanged,
            modelPcm, &ModelPcm::SetCar1SystemFile);

    connect(viewPcm, &ViewPcm::Car2FileChanged,
            modelPcm, &ModelPcm::SetCar2SystemFile);

    connect(viewPcm, &ViewPcm::StartSimulation,
            modelPcm, &ModelPcm::StartSimulationTrigger);

    connect(modelPcm, &ModelPcm::SimulationProgressMaximum,
            viewPcm, &ViewPcm::SetProgessBarMaximum);

    connect(modelPcm, &ModelPcm::SimulationProgressChanged,
            viewPcm, &ViewPcm::SetProgressBarValue);

    connect(modelPcm, &ModelPcm::SimulationStarted,
            viewPcm, &ViewPcm::OnSimulationStarted);

    connect(modelPcm, &ModelPcm::SimulationFinished,
            viewPcm, &ViewPcm::OnSimulationFinished);

    connect(modelPcm, &ModelPcm::ShowMessage,
            viewPcm, &ViewPcm::ShowMessage);

    connect(viewPcm, &ViewPcm::StopSimulation,
            modelPcm, &ModelPcm::SimulationStop);
}

