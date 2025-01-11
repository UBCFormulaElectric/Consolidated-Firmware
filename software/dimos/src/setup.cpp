#include "setup.h"
#include "can.h"
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
#include "io_canTx.h"
}

#include <QThread>

void set_qt_environment()
{
    qputenv("QML_COMPAT_RESOLVE_URLS_ON_ASSIGNMENT", "1");
    // qputenv("QT_QUICK_CONTROLS_CONF", ":/qtquickcontrols2.conf");
#ifdef Q_OS_WIN
    qputenv("QT_WIN_DEBUG_CONSOLE", "attach");
#endif

    qSetMessagePattern("[%{time hh:mm:ss.z}] %{function}: \"%{message}\""
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