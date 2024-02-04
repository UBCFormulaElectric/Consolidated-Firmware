#pragma once
#include <QtQml>

extern "C"
{
#include "app_canRx.h"
}

class JSONCANQML final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Class only exposes constants to QML.")
    QML_SINGLETON

public:
    static bool VC_Fault_DummyFault_get()
    {
        return app_canRx_VC_Fault_DummyFault_get();
    }
    Q_PROPERTY(bool VC_Fault_DummyFault READ VC_Fault_DummyFault_get NOTIFY notify_all_signals FINAL REQUIRED)
signals:
    void notify_all_signals();
};