/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PRESENTERPCM_H
#define PRESENTERPCM_H

#include "Models/ModelPcm.h"
#include "Views/ViewPcm.h"

#include <QObject>

class PresenterPcm : public QObject
{
    Q_OBJECT

public:
    explicit PresenterPcm(ModelPcm *modelPcm,
                          ViewPcm *viewPcm,
                          QObject *parent = nullptr);
    virtual ~PresenterPcm() = default;

private:
    ModelPcm *const modelPcm;
    ViewPcm *const viewPcm;
};

#endif // PRESENTERPCM_H
