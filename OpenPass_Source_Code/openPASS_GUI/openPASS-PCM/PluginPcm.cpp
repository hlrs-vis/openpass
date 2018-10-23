#include "PluginPcm.h"

#include "openPASS-Window/WindowInterface.h"

PluginPcm::PluginPcm(QObject *const parent)
    : QObject(parent)
    , modelPcm(nullptr)
    , viewPcm(nullptr)
    , presenterPcm(nullptr)
{
}

bool PluginPcm::initialize()
{
    WindowInterface *const window = WindowInterface::instance();
    if (window)
    {
        modelPcm = new ModelPcm();
        viewPcm = new ViewPcm(window);
        presenterPcm = new PresenterPcm(modelPcm, viewPcm);

        return true;
    }
    return false;
}

bool PluginPcm::deinitialize()
{
    delete presenterPcm;
    delete viewPcm;
    delete modelPcm;
    return true;
}
