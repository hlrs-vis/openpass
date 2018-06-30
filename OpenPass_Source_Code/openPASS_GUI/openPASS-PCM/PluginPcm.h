#ifndef PLUGINPCM_H
#define PLUGINPCM_H

#include <QObject>

#include "openPASS/PluginInterface.h"
#include "Models/ModelPcm.h"
#include "Views/ViewPcm.h"
#include "Presenters/PresenterPcm.h"

class ModelPcm;
class PresenterPcm;
class ViewPcm;

class PluginPcm : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.PCM")
    Q_INTERFACES(PluginInterface)

public:
    explicit PluginPcm(QObject *const parent = nullptr);
    virtual ~PluginPcm() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    ModelPcm *modelPcm;
    ViewPcm *viewPcm;
    PresenterPcm *presenterPcm;
};

#endif // PLUGINPCM_H
