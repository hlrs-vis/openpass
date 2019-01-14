/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
            modelPcm, &ModelPcm::LoadCasesFromPcmFile);

    connect(viewPcm, &ViewPcm::PrevResultFolderChanged,
            modelPcm, &ModelPcm::LoadCasesFromPrevResult);

    connect(viewPcm, &ViewPcm::ResultFolderChanged,
            modelPcm, &ModelPcm::SetResultFolder);

    connect(viewPcm, &ViewPcm::LogLevelChanged,
            modelPcm, &ModelPcm::SetLogLevel);

    connect(viewPcm, &ViewPcm::OtherFileChanged,
            modelPcm, &ModelPcm::SetOtherSystemFile);

    connect(viewPcm, &ViewPcm::Car1FileChanged,
            modelPcm, &ModelPcm::SetCar1SystemFile);

    connect(viewPcm, &ViewPcm::Car2FileChanged,
            modelPcm, &ModelPcm::SetCar2SystemFile);

    connect(viewPcm, &ViewPcm::RandomSeedChanged,
            modelPcm, &ModelPcm::SetInitRandomSeed);

    connect(viewPcm, &ViewPcm::VariationCountChanged,
            modelPcm, &ModelPcm::SetVariationCount);

    connect(viewPcm, &ViewPcm::ShiftRadius1Changed,
            modelPcm, &ModelPcm::SetShiftRadius1);

    connect(viewPcm, &ViewPcm::ShiftRadius2Changed,
            modelPcm, &ModelPcm::SetShiftRadius2);

    connect(viewPcm, &ViewPcm::VelocityScale1Changed,
            modelPcm, &ModelPcm::SetVelocityScale1);

    connect(viewPcm, &ViewPcm::VelocityScale2Changed,
            modelPcm, &ModelPcm::SetVelocityScale2);

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

