#include <chrono>

#include "can.h"
#include "can_tasks.h"

extern "C"
{
#include "io_canRx.h"
#include "io_canTx.h"
#include "app_canRx.h"
}

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

Result<std::monostate, CanTask::CAN_setup_errors> CanTask::setup()
{
    Can_Init();
    // tx 100hz
    tx100Hz.setInterval(TASK_INTERVAL_100HZ);
    tx100Hz.setSingleShot(false);
    QObject::connect(&tx100Hz, &QTimer::timeout, &tx100Hzworker, &CanTx100HzTask::run);
    tx100Hz.start();
    // tx 1hz
    tx1Hz.setInterval(TASK_INTERVAL_1HZ);
    tx1Hz.setSingleShot(false);
    QObject::connect(&tx1Hz, &QTimer::timeout, &tx1Hzworker, &CanTx1HzTask::run);
    tx1Hz.start();

    rxworker.start();
    txperiodicworker.start();

    qInfo() << "CAN Threads Successfully Initialized";
    return std::monostate{};
}

void CanRxTask::run()
{
    qInfo("Starting CanRXTask thread");
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        Result<JsonCanMsg, CanReadError> can_read_res = Can_Read();
        if (can_read_res.has_error())
        {
            switch (can_read_res.get_error())
            {
                case CanReadError::ReadInterfaceNotCreated:
                    qWarning("Can interface not created");
                    return;
                case CanReadError::Timeout:
#ifdef USING_TARGET_deploy
                    qWarning("CANRX Timeout");
                    break;
#endif
                case CanReadError::SocketReadError:
                case CanReadError::IncompleteCanFrame:
                    break;
            }
            continue;
        }

        // success
        auto message = can_read_res.get_data();
        // check with
        // io_canRx_filterMessageId
        // if we care about the message
        can_table_mutex->lock();
        io_canRx_updateRxTableWithMessage(&message);
#ifdef USING_TARGET_dev
        app_canRx_FSM_LeftWheelSpeed_update(10 - app_canRx_FSM_LeftWheelSpeed_get());
        // qInfo("New Dummy Fault Value: %d", app_canRx_VC_Fault_DummyFault_get());
#endif
        can_table_mutex->unlock();
    }
    qInfo("KILL CanRXTask thread");
}

void CanTxPeriodicTask::run()
{
    using namespace std::chrono;
    qInfo("Starting CanPeriodicTXTask thread");
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        can_table_mutex->lock();
        io_canTx_enqueueOtherPeriodicMsgs(ms.count());
        can_table_mutex->unlock();
        QThread::msleep(1); // yield to other threads, make larger if big lag problem
    }
    qInfo("KILL CanPeriodicTXTask thread");
}

void CanTx100HzTask::run()
{
    can_table_mutex->lock();
    io_canTx_enqueue100HzMsgs();
    can_table_mutex->unlock();
}

void CanTx1HzTask::run()
{
    can_table_mutex->lock();
    io_canTx_enqueue1HzMsgs();
    can_table_mutex->unlock();
}

Result<std::monostate, CanTask::CAN_teardown_errors> CanTask::teardown()
{
    rxworker.requestInterruption();
    txperiodicworker.requestInterruption();
    tx100Hz.stop();
    tx1Hz.stop();
    rxworker.terminate(); // TODO unless we can figure out how to timeout socket read
    qInfo("Waiting for CAN threads to terminate");
    rxworker.wait();
    txperiodicworker.wait();
    qInfo("CAN Threads terminated");
    return std::monostate{};
}