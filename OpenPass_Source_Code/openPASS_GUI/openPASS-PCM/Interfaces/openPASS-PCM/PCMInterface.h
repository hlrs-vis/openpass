#ifndef PCMINTERFACE_H
#define PCMINTERFACE_H

#include "openPASS/ServiceInterface.h"

#include <QObject>

static ServiceManagerInterface::ID const PCMInterfaceID =
        ServiceManagerInterfaceID("openPASS.PCM.PCMInterface");

class PCMInterface : public QObject,
        public ServiceInterface<PCMInterface, PCMInterfaceID>
{
    Q_OBJECT

public:
    explicit PCMInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~PCMInterface() = default;
};

#endif // PCMINTERFACE_H
