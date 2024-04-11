#pragma once
#include <QThread>
#include <QMutex>

#include "ui/canqml/canqml.h"
#include "types.h"

enum class CAN_setup_errors
{
};
static const std::map<CAN_setup_errors, std::string> CAN_setup_errors_str = {

};

class CanTask
{
  public:
    Result<std::monostate, CAN_setup_errors> setup();
    ~CanTask();

  private:
#ifdef USING_TARGET_dev
    static constexpr uint16_t TASK_INTERVAL_100HZ = 1000;
    static constexpr uint16_t TASK_INTERVAL_1HZ   = 1000;
#elif USING_TARGET_deploy
    static constexpr uint16_t TASK_INTERVAL_100HZ = 10;
    static constexpr uint16_t TASK_INTERVAL_1HZ   = 1000;
#endif
    QTimer tx100Hz{}, tx1Hz{};
    QMutex can_table_mutex{};

    class CanRxTask final : public QThread
    {
        QMutex *can_table_mutex;

      public:
        explicit CanRxTask(QMutex *can_table_mutex) : can_table_mutex{ can_table_mutex } {};
        void run() override;
    } rxworker{ &can_table_mutex };
    class CanTxPeriodicTask final : public QThread
    {
        QMutex *can_table_mutex;

      public:
        void run() override;
        explicit CanTxPeriodicTask(QMutex *can_table_mutex) : can_table_mutex{ can_table_mutex } {};
    } txperiodicworker{ &can_table_mutex };

    class CanTx100HzTask final : public QObject
    {
        QMutex *can_table_mutex;

      public slots:
        void run();
        explicit CanTx100HzTask(QMutex *can_table_mutex) : can_table_mutex{ can_table_mutex } {};
    } tx100Hzworker{ &can_table_mutex };

    class CanTx1HzTask final : public QObject
    {
        QMutex *can_table_mutex;

      public slots:
        void run();
        explicit CanTx1HzTask(QMutex *can_table_mutex) : can_table_mutex{ can_table_mutex } {};
    } tx1Hzworker{ &can_table_mutex };
};