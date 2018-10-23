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
