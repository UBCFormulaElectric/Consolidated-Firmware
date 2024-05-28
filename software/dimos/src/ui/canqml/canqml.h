#pragma once
#include <QtQml>
extern "C"
{
#include "app_canRx.h"
#include "app_canAlerts.h"
}

class Q_Fault_Warning_Info : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString description MEMBER description CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QString id MEMBER id CONSTANT)
  public:
    explicit Q_Fault_Warning_Info(const Fault_Warning_Info &info)
      : description(info.description), name(info.name), id(info.id), int_id(info.id)
    {
    }

    bool operator==(const Fault_Warning_Info &other) const { return id == other.id; }

    [[nodiscard]] int get_id() const { return int_id; }

  private:
    const QString description;
    const QString name;
    const QString id;
    const int     int_id;
};

#define CONCAT_HELPER(x, y) x##y
#define REGISTER_CAN_MESSAGE(name, type)    \
    static type CONCAT_HELPER(name, _get)() \
    {                                       \
        return app_canRx_##name##_get();    \
    }                                       \
    Q_PROPERTY(type name READ CONCAT_HELPER(name, _get) NOTIFY notify_all_signals FINAL REQUIRED STORED false)

class CanQML final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    // timer shit
    static constexpr uint16_t FRAME_RATE_HZ            = 30;
    static constexpr uint16_t UI_UPDATE_INTERVAL_MSEC  = 1000 / FRAME_RATE_HZ;
    static constexpr uint16_t FAULT_POLL_INTERVAL_MSEC = 500;
    QTimer                    uiUpdate{}, faultPoll{};

    bool has_fault = false;
    Q_PROPERTY(bool has_fault MEMBER has_fault NOTIFY faultChanged FINAL)
    QList<Q_Fault_Warning_Info *> faults{}, warnings{};
    Q_PROPERTY(QList<Q_Fault_Warning_Info *> faults MEMBER faults FINAL NOTIFY faults_changed)
    Q_PROPERTY(QList<Q_Fault_Warning_Info *> warnings MEMBER warnings FINAL NOTIFY warnings_changed)

    explicit CanQML(QObject *parent = nullptr) : QObject(parent)
    {
        // ui update
        uiUpdate.setInterval(UI_UPDATE_INTERVAL_MSEC);
        uiUpdate.setSingleShot(false);
        connect(&uiUpdate, &QTimer::timeout, this, &CanQML::notify_all_signals);
        uiUpdate.start();

        faultPoll.setInterval(FAULT_POLL_INTERVAL_MSEC);
        faultPoll.setSingleShot(false);
        connect(&faultPoll, &QTimer::timeout, this, &CanQML::fault_poll);
        faultPoll.start();
    }
    ~CanQML() override
    {
        uiUpdate.stop();
        faultPoll.stop();
    };

  public:
    static CanQML *getInstance();
    static CanQML *create(const QQmlEngine *qmlEngine, const QJSEngine *jsEngine);

    // Signals
    REGISTER_CAN_MESSAGE(FSM_LeftWheelSpeed, float)
    REGISTER_CAN_MESSAGE(FSM_PappsMappedPedalPercentage, float)
    REGISTER_CAN_MESSAGE(FSM_SappsMappedPedalPercentage, float)
    REGISTER_CAN_MESSAGE(BMS_Soc, float)
    REGISTER_CAN_MESSAGE(FSM_FrontBrakePressure, int)
    REGISTER_CAN_MESSAGE(FSM_RearBrakePressure, int)
    REGISTER_CAN_MESSAGE(BMS_MaxCellTemperature, int)
    REGISTER_CAN_MESSAGE(BMS_TractiveSystemPower, int)

  public slots:
    void fault_poll();

  private:
    bool fault_consolidation(QList<Q_Fault_Warning_Info *> &a, Fault_Warning_Info *b, int b_size);

  signals:
    void faultChanged();
    void faults_changed();
    void warnings_changed();
    void notify_all_signals();
};