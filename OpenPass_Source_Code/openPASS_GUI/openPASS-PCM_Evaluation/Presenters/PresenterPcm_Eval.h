/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PRESENTERPCM_EVAL_H
#define PRESENTERPCM_EVAL_H

#include "Models/ModelPcm_Eval.h"
#include "Views/ViewPcm_Eval.h"

#include <QObject>

class PresenterPcm_Eval : public QObject
{
    Q_OBJECT

public:
    explicit PresenterPcm_Eval(ModelPcm_Eval *modelPcm_Eval,
                               ViewPcm_Eval *viewPcm_Eval,
                               QObject *parent = nullptr);
    virtual ~PresenterPcm_Eval() = default;

private:
    ModelPcm_Eval *const modelPcm_Eval;
    ViewPcm_Eval *const viewPcm_Eval;
};

#endif // PRESENTERPCM_EVAL_H
