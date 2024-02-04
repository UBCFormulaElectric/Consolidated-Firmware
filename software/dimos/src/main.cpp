// can
#include "can.h"
#include "jsoncanqml.h"
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
#include "io_canRx.h"
#include "io_canTx.h"
}

#include <QtGui>
#include <QQmlApplicationEngine>
#include <QString>

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

enum CAN_setup_errors
{
};
static QTimer                            tx100Hz{}, tx1Hz{}, uiUpdate{};
static std::unique_ptr<QThread>          CanRxTaskThread, CanTxPeriodicTaskThread;
static JSONCANQML                        jsoncan_qml_interface;
Result<std::monostate, CAN_setup_errors> setupCanThreads(const QQmlApplicationEngine *engine_ref)
{
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
    // ui update
    uiUpdate.setInterval(can_handlers::TASK_INTERVAL_UI_UPDATE);
    uiUpdate.setSingleShot(false);
    QObject::connect(&uiUpdate, &QTimer::timeout, &jsoncan_qml_interface, jsoncan_qml_interface.notify_all_signals);
    QObject::connect(engine_ref, &QQmlApplicationEngine::quit, &uiUpdate, &QTimer::stop);
    uiUpdate.start();
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

enum GPIO_setup_errors
{
    LINE_SETUP_ERROR
};
const std::map<GPIO_setup_errors, std::string>                         GPIO_setup_errors_str = { { LINE_SETUP_ERROR,
                                                                                                   "[main_GPIO] Line Setup Error" } };
static std::array<std::optional<std::unique_ptr<QThread>>, GPIO_COUNT> gpio_monitor_threads;

Result<std::monostate, GPIO_setup_errors> setupGPIOThreads(const QQmlApplicationEngine *engine_ref)
{
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
        gpio_monitor_threads[i] = std::unique_ptr<QThread>(QThread::create(&gpio_handlers::gpio_monitor, GPIO_ENUM));
        gpio_monitor_threads[i].value()->start();
        QObject::connect(
            engine_ref, &QQmlApplicationEngine::quit, gpio_monitor_threads[i].value().get(),
            &QThread::requestInterruption);
    }

    if (has_gpio_err)
    {
        return LINE_SETUP_ERROR;
    }
    qInfo("GPIO Threads Sucessfully Initialized");
    return std::monostate{};
}

int main(int argc, char *argv[])
{
    set_qt_environment();
    init_json_can();
    qmlRegisterSingletonInstance("JSONCANQML", 1, 0, "JSONCANQML", &jsoncan_qml_interface);

    const QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/");
    // engine.loadFromModule("MainWindow", "MainWindow"); // this does not work on Qt < 6.6
    // const QUrl url("./MainWindow/ui/MainWindow.qml"); // this also works, but is cringe
    const QUrl url(u"qrc:/MainWindow/ui/MainWindow.qml"_qs);
    engine.load(url);

    // graceful exit
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QGuiApplication::quit);
    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;

    // setup task threads
    if (const Result<std::monostate, CAN_setup_errors> r = setupCanThreads(&engine); r.index() == 1)
        qWarning() << "CAN setup error " << get<CAN_setup_errors>(r);
    if (const Result<std::monostate, GPIO_setup_errors> r = setupGPIOThreads(&engine); r.index() == 1)
        qWarning() << QString::fromStdString(GPIO_setup_errors_str.find(get<GPIO_setup_errors>(r))->second);

    return QGuiApplication::exec();
}