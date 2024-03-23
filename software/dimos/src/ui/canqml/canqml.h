#pragma once
#include <QtQml>

extern "C"
{
#include "app_canRx.h"
}

class CanQML final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    static CanQML *instance;
    explicit       CanQML(QObject *parent = nullptr) : QObject(parent) {}

  public:
    static CanQML *getInstance()
    {
        if (instance == nullptr)
            instance = new CanQML();
        return instance;
    }
    static CanQML *create(const QQmlEngine *qmlEngine, const QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine);
        Q_UNUSED(jsEngine);
        return getInstance();
    }
    static bool VC_Fault_DummyFault_get() { return app_canRx_VC_Fault_DummyFault_get(); }
    Q_PROPERTY(
        bool VC_Fault_DummyFault READ VC_Fault_DummyFault_get NOTIFY notify_all_signals FINAL REQUIRED STORED false)
  signals:
    void notify_all_signals();
};