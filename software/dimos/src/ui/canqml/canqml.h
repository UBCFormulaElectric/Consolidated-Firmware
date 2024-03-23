#pragma once
#include <QtQml>

extern "C"
{
#include "app_canRx.h"
}

#define CONCAT_HELPER(x, y) x##y
#define REGISTER_CAN_MESSAGE(name)          \
    static bool CONCAT_HELPER(name, _get)() \
    {                                       \
        return app_canRx_##name##_get();    \
    }                                       \
    Q_PROPERTY(bool name READ CONCAT_HELPER(name, _get) NOTIFY notify_all_signals FINAL REQUIRED STORED false)

class CanQML final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    static constexpr uint16_t frame_rate_hz           = 30;
    static constexpr uint16_t ui_update_interval_msec = 1000 / frame_rate_hz;

    // timer shit
    QTimer   uiUpdate{};
    explicit CanQML(QObject *parent = nullptr) : QObject(parent)
    {
        // ui update
        uiUpdate.setInterval(ui_update_interval_msec);
        uiUpdate.setSingleShot(false);
        connect(&uiUpdate, &QTimer::timeout, this, &notify_all_signals);
        uiUpdate.start();
    }
    ~CanQML() override { uiUpdate.stop(); };

    // singleton activity
    static CanQML *instance;

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

    Q_PROPERTY(int first_error_node READ get_first_error_node NOTIFY notify_all_signals FINAL REQUIRED STORED false)
    static int get_first_error_node()
    {
        return -1; // TODO implement faults when they arrive
    }
    // Signals
    REGISTER_CAN_MESSAGE(VC_Fault_DummyFault)

  signals:
    void notify_all_signals();
};