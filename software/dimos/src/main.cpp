#include "io/KeyTranslator.h"
#include "setup.h"
#include "can_tasks.h"
#include "gpio_tasks.h"
// qt
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QString>
#ifdef USING_TARGET_deploy
#include "unixsignals.h"
#include <csignal>
#endif

template <typename T>
inline static void report_task_errors(
    const Result<std::monostate, T> &res,
    const std::map<T, std::string>  &err_string_map,
    const std::string               &sys)
{
    if (res.index() == 1)
    {
        if (const auto can_err_kv = err_string_map.find(get<T>(res)); can_err_kv == err_string_map.end())
            qWarning("Unknown %s Setup Error", sys.c_str());
        else
            qWarning("%s Setup Error: %s", sys.c_str(), can_err_kv->second.c_str());
    }
}

int main(int argc, char *argv[])
{
    set_qt_environment();
    init_json_can();

    QGuiApplication       app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/");
    // engine.loadFromModule("MainWindow", "MainWindow"); // this does not work on Qt < 6.6
    // const QUrl url("./MainWindow/ui/MainWindow.qml"); // this also works, but is cringe
    engine.load(QUrl(u"qrc:/DimosMain/ui/MainWindow.qml"_qs));

    // kill parent application if qml quits
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QGuiApplication::quit);
    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;

#ifdef USING_TARGET_deploy
    // kill parent application if SIGTERM is recieved
    QObject::connect(new UnixSignalHandler(SIGTERM, &app), &UnixSignalHandler::raised, qApp, &QCoreApplication::quit);
#endif

    // keyfilter
    KeyTranslator k;
    engine.rootObjects().first()->installEventFilter(&k);

    // setup task threads
    report_task_errors(CanTask::setup(), CanTask::CAN_setup_errors_str, "CAN");
    report_task_errors(GPIOTask::setup(), GPIOTask::GPIO_setup_errors_str, "GPIO");
    auto status = QGuiApplication::exec();
    report_task_errors(CanTask::teardown(), CanTask::CAN_teardown_errors_str, "CAN");
    report_task_errors(GPIOTask::teardown(), GPIOTask::GPIO_teardown_errors_str, "GPIO");
    return status;
}