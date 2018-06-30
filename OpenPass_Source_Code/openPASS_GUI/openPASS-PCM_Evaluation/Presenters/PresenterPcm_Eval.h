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
