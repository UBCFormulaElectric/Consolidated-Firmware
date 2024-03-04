#include "io/KeyTranslator.h"
#include "setup.h"
// qt
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QString>

int main(int argc, char *argv[])
{
    set_qt_environment();
    init_json_can();

    const QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/");
    // engine.loadFromModule("MainWindow", "MainWindow"); // this does not work on Qt < 6.6
    // const QUrl url("./MainWindow/ui/MainWindow.qml"); // this also works, but is cringe
    const QUrl url(u"qrc:/DimosMain/ui/MainWindow.qml"_qs);
    engine.load(url);

    // graceful exit
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QGuiApplication::quit);
    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;

    KeyTranslator k;
    engine.rootObjects().first()->installEventFilter(&k);

    // setup task threads
    if (const Result<std::monostate, CAN_setup_errors> r = setupCanThreads(&engine); r.index() == 1)
    {
        if (const auto can_err_kv = CAN_setup_errors_str.find(get<CAN_setup_errors>(r));
            can_err_kv == CAN_setup_errors_str.end())
            qWarning("Unknown CAN Setup Error");
        else
            qWarning("CAN Setup Error: %s", can_err_kv->second.c_str());
    }
    if (const Result<std::monostate, GPIO_setup_errors> r = setupGPIOThreads(&engine); r.index() == 1)
    {
        if (const auto gpio_err_kv = GPIO_setup_errors_str.find(get<GPIO_setup_errors>(r));
            gpio_err_kv == GPIO_setup_errors_str.end())
            qWarning("Unknown GPIO Setup Error");
        else
            qWarning("GPIO Setup Error: %s", gpio_err_kv->second.c_str());
    }

    return QGuiApplication::exec();
}