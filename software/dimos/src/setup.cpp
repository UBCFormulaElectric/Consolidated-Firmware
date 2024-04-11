#include "setup.h"
#include "can.h"
#include "gpio.h"
#include "gpio_tasks.h"
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
#include "io_canTx.h"
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

    qInfo("GPIO Thread Initialization Complete");
    if (any_gpio_has_err)
        return GPIO_setup_errors::LINE_SETUP_ERROR;
    qInfo("GPIO Threads Sucessfully Initialized");
    return std::monostate{};
}