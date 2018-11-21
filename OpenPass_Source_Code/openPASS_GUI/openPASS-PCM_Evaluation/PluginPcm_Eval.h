/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PLUGINPCM_EVAL_H
#define PLUGINPCM_EVAL_H

#include <QObject>

#include "openPASS/PluginInterface.h"
#include "Models/ModelPcm_Eval.h"
#include "Views/ViewPcm_Eval.h"
#include "Presenters/PresenterPcm_Eval.h"

class ModelPcm_Eval;
class PresenterPcm_Eval;
class ViewPcm_Eval;

class PluginPcm_Eval : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.PCM_Eval")
    Q_INTERFACES(PluginInterface)

public:
    explicit PluginPcm_Eval(QObject *const parent = nullptr);
    virtual ~PluginPcm_Eval() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    ModelPcm_Eval *modelPcm_Eval;
    ViewPcm_Eval *viewPcm_Eval;
    PresenterPcm_Eval *presenterPcm_Eval;
};

#endif // PLUGINPCM_EVAL_H
