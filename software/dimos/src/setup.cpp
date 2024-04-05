#include "setup.h"
#include "can.h"
#include "gpio.h"
#include "can_tasks.h"
#include "gpio_tasks.h"
#include "ui/canqml/canqml.h"
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
#include "io_canTx.h"
    // #include "io_canRx.h"
}

#include <QThread>
// std
#include <map>

void set_qt_environment()
{
    qputenv("QML_COMPAT_RESOLVE_URLS_ON_ASSIGNMENT", "1");
    // qputenv("QT_QUICK_CONTROLS_CONF", ":/qtquickcontrols2.conf");
#ifdef Q_OS_WIN
    qputenv("QT_WIN_DEBUG_CONSOLE", "attach");
#endif

    qSetMessagePattern("[%{time hh:mm:ss.z} | %{function}] %{message}"
                       // "%{file}:%{line}"
    );
}

void init_json_can()
{
    io_canTx_init(reinterpret_cast<void (*)(const JsonCanMsg *)>(Can_Write));
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    app_canTx_init();
    app_canRx_init();
    qInfo() << "JsonCAN Initialized";

    // set values for commit info
    app_canTx_dimos_Hash_set(GIT_COMMIT_HASH);
    app_canTx_dimos_Clean_set(GIT_COMMIT_CLEAN);
}

static QTimer                            tx100Hz{}, tx1Hz{};
static std::unique_ptr<QThread>          CanRxTaskThread, CanTxPeriodicTaskThread;
Result<std::monostate, CAN_setup_errors> setupCanThreads(const QQmlApplicationEngine *engine_ref)
{
    Can_Init();
    // tx 100hz
    tx100Hz.setInterval(can_handlers::TASK_INTERVAL_100HZ);
    tx100Hz.setSingleShot(false);
    QObject::connect(&tx100Hz, &QTimer::timeout, can_handlers::CanTx100Hz);
    QObject::connect(engine_ref, &QQmlApplicationEngine::quit, &tx100Hz, &QTimer::stop);
    tx100Hz.start();
    // tx 1hz
    tx1Hz.setInterval(can_handlers::TASK_INTERVAL_1HZ);
    tx1Hz.setSingleShot(false);
    QObject::connect(&tx1Hz, &QTimer::timeout, can_handlers::CanTx1Hz);
    QObject::connect(engine_ref, &QQmlApplicationEngine::quit, &tx1Hz, &QTimer::stop);
    tx1Hz.start();
    // rx
    CanRxTaskThread = std::unique_ptr<QThread>(QThread::create(&can_handlers::CanRXTask));
    CanRxTaskThread->start();
    QObject::connect(engine_ref, &QQmlApplicationEngine::quit, CanRxTaskThread.get(), &QThread::requestInterruption);
    // tx periodic
    CanTxPeriodicTaskThread = std::unique_ptr<QThread>(QThread::create(&can_handlers::CanPeriodicTXTask));
    CanTxPeriodicTaskThread->start();
    QObject::connect(
        engine_ref, &QQmlApplicationEngine::quit, CanTxPeriodicTaskThread.get(), &QThread::requestInterruption);

    qInfo() << "CAN Threads Successfully Initialized";
    return std::monostate{};
}

static std::vector<std::unique_ptr<QThread>> gpio_monitor_threads;
Result<std::monostate, GPIO_setup_errors>    setupGPIOThreads(const QQmlApplicationEngine *engine_ref)
{
    const std::map<gpio_input, bool> gpio_has_err     = gpio_init();
    bool                             any_gpio_has_err = false;
    for (auto &gpio_input : gpio_inputs)
    {
        if (const auto gpiokvpair = gpio_has_err.find(gpio_input);
            gpiokvpair == gpio_has_err.end() || gpiokvpair->second)
        {
            any_gpio_has_err = true;
            continue;
        }

        auto new_gpio_thread = std::unique_ptr<QThread>(QThread::create(&gpio_handlers::gpio_monitor, gpio_input));
        gpio_monitor_threads.push_back(std::move(new_gpio_thread));
        gpio_monitor_threads.back()->start();
        QObject::connect(
            engine_ref, &QQmlApplicationEngine::quit, gpio_monitor_threads.back().get(), &QThread::requestInterruption);
    }

    if (any_gpio_has_err)
        return GPIO_setup_errors::LINE_SETUP_ERROR;
    qInfo("GPIO Threads Sucessfully Initialized");
    return std::monostate{};
}