// can
#include "can.h"
extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "Io_CanRx.h"
#include "Io_CanTx.h"
#include "App_CommitInfo.h"
}

#include <QtGui>
#include <QQmlApplicationEngine>

#include <QThread>
#include <map>
#include "can_tasks.h"
#include "gpio_tasks.h"

void set_qt_environment()
{
    qputenv("QML_COMPAT_RESOLVE_URLS_ON_ASSIGNMENT", "1");
    // qputenv("QT_QUICK_CONTROLS_CONF", ":/qtquickcontrols2.conf");
#ifdef Q_OS_WIN
    qputenv("QT_WIN_DEBUG_CONSOLE", "attach");
#endif
}

void init_json_can()
{
    Io_CanTx_Init(reinterpret_cast<void (*)(const JsonCanMsg *)>(Can_Write));
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    App_CanTx_Init();
    App_CanRx_Init();
    qInfo() << "[init_json_can] JsonCAN Initialized";

    // set values for commit info
    App_CanTx_dimos_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_dimos_Clean_Set(GIT_COMMIT_CLEAN);
}

enum CAN_setup_errors {};
static QTimer tx100Hz{}, tx1Hz{};
static std::unique_ptr<QThread> CanRxTaskThread, CanTxPeriodicTaskThread;
Result<std::monostate, CAN_setup_errors> setupCanThreads(const QQmlApplicationEngine * engine_ref) {
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
    QObject::connect(engine_ref, &QQmlApplicationEngine::quit, CanTxPeriodicTaskThread.get(), &QThread::requestInterruption);

    qInfo() << "[CAN] CAN Threads Successfully Initialized";
    return std::monostate{};
}

enum GPIO_setup_errors {
    LINE_SETUP_ERROR
};
const std::map<GPIO_setup_errors, std::string> GPIO_setup_errors_str = {
    {LINE_SETUP_ERROR, "[GPIO] Line Setup Error"}
};
static std::optional<std::unique_ptr<QThread>> gpio_monitor_threads[GPIO_COUNT];
Result<std::monostate, GPIO_setup_errors> setupGPIOThreads(const QQmlApplicationEngine * engine_ref) {
    const std::array<bool, GPIO_COUNT> gpio_has_err = gpio_init();
    bool                               has_gpio_err = false;
    for (int i = 0; i < GPIO_COUNT; i++)
    {
        const auto GPIO_ENUM = static_cast<gpio_input>(i);
        if (gpio_has_err[i])
        {
            has_gpio_err = true;
            continue;
        }
        gpio_monitor_threads[i] = std::make_unique<QThread>(QThread::create(&gpio_handlers::gpio_monitor, GPIO_ENUM));
        QObject::connect(engine_ref, &QQmlApplicationEngine::quit, gpio_monitor_threads[i].value().get(), &QThread::requestInterruption);
        gpio_monitor_threads[i].value()->start();
    }
    if (has_gpio_err) return LINE_SETUP_ERROR;

    qInfo() << "[GPIO] GPIO Threads Sucessfully Initialized";
    return std::monostate{};
}

int main(int argc, char *argv[])
{
    set_qt_environment();
    const QGuiApplication app(argc, argv);
    init_json_can();

    QQmlApplicationEngine engine;
    engine.addImportPath(":/");
    engine.loadFromModule("MainWindow", "MainWindow");
    // graceful exit
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QGuiApplication::quit);
    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;

    // setup task threads
    if(const Result<std::monostate, CAN_setup_errors> r = setupCanThreads(&engine); r.index() == 1) {
        qWarning() << "CAN setup error " << get<CAN_setup_errors>(r);
    }
    if(const Result<std::monostate, GPIO_setup_errors> r = setupGPIOThreads(&engine); r.index() == 1) {
        qWarning() << GPIO_setup_errors_str.find(get<GPIO_setup_errors>(r))->second;
    }

    return QGuiApplication::exec();
}