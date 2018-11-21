/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "Presenters/PresenterPcm_Eval.h"

PresenterPcm_Eval::PresenterPcm_Eval(ModelPcm_Eval *modelPcm_Eval, ViewPcm_Eval *viewPcm_Eval,
                                     QObject *parent)
    : QObject(parent)
    , modelPcm_Eval(modelPcm_Eval)
    , viewPcm_Eval(viewPcm_Eval)
{
    viewPcm_Eval->SetModelPcm(modelPcm_Eval->GetItemModelPcm());
    modelPcm_Eval->SetSelectionModelPcm(viewPcm_Eval->GetSelectionModelPcm());

    connect(viewPcm_Eval, &ViewPcm_Eval::ResultFileChanged,
            modelPcm_Eval, &ModelPcm_Eval::LoadFileData);

    connect(viewPcm_Eval, &ViewPcm_Eval::ResultPathChanged,
            modelPcm_Eval, &ModelPcm_Eval::LoadPathData);

    connect(modelPcm_Eval, &ModelPcm_Eval::TableModelCreated,
            viewPcm_Eval, &ViewPcm_Eval::AddTableModel);

    connect(modelPcm_Eval, &ModelPcm_Eval::TrajectoryCreated,
            viewPcm_Eval, &ViewPcm_Eval::AddTrajectory);

    connect(modelPcm_Eval, &ModelPcm_Eval::Clear,
            viewPcm_Eval, &ViewPcm_Eval::OnClear);

    connect(modelPcm_Eval, &ModelPcm_Eval::MarksCreated,
            viewPcm_Eval, &ViewPcm_Eval::AddMarks);

    connect(modelPcm_Eval, &ModelPcm_Eval::ObjectCreated,
            viewPcm_Eval, &ViewPcm_Eval::AddObject);
}

