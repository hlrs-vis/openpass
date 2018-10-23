#ifndef PCM_EVAL_INTERFACE_H
#define PCM_EVAL_INTERFACE_H

#include "openPASS/ServiceInterface.h"

#include <QObject>

static ServiceManagerInterface::ID const PCM_Eval_InterfaceID =
    ServiceManagerInterfaceID("openPASS.PCM_Eval.PCM_Eval_Interface");

class PCM_Eval_Interface : public QObject,
    public ServiceInterface<PCM_Eval_Interface, PCM_Eval_InterfaceID>
{
    Q_OBJECT

public:
    explicit PCM_Eval_Interface(QObject *const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~PCM_Eval_Interface() = default;
};

#endif // PCM_EVAL_INTERFACE_H
