// can
#include "can.h"
extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "Io_CanRx.h"
#include "Io_CanTx.h"
}

#include <QtGui>
#include <QQmlApplicationEngine>

void set_qt_environment()
{
    qputenv("QML_COMPAT_RESOLVE_URLS_ON_ASSIGNMENT", "1");
    // qputenv("QT_QUICK_CONTROLS_CONF", ":/qtquickcontrols2.conf");
#ifdef Q_OS_WIN
    qputenv("QT_WIN_DEBUG_CONSOLE", "attach");
#endif
}

constexpr uint32_t GIT_COMMIT_HASH  = 0x00000000;
constexpr bool     GIT_COMMIT_CLEAN = false;

void init_json_can()
{
    // IO init
    Io_CanTx_Init(reinterpret_cast<void (*)(const JsonCanMsg *)>(Can_Write));
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    // clear tables
    App_CanTx_Init();
    App_CanRx_Init();
    // TODO commit hash
    App_CanTx_dimos_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_dimos_Clean_Set(GIT_COMMIT_CLEAN);
    qInfo() << "[init_json_can] JsonCAN Initialized";
}

int main(int argc, char *argv[])
{
    set_qt_environment();
    const QGuiApplication app(argc, argv);
    init_json_can();

    QQmlApplicationEngine engine;
    engine.addImportPath(":/");
    engine.loadFromModule("MainWindow", "MainWindow");

    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QGuiApplication::quit);
    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;
    return QGuiApplication::exec();
}