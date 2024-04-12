#include "gpio.h"
#include "gpio_tasks.h"
#include "ui/DimSwitches/DimSwitchEmitter.h"

#include <QThread>
#include <qlogging.h>

static DimSwitchEmitter *dse = DimSwitchEmitter::getInstance();
// Functions handlers here correspond to the names given to the lines in altium.
void ROT_A(const gpio_edge edge)
{
    const Result<gpio_level, line_read_error> b_reading_res = read_gpio(gpio_input::GPIO8);
    if (b_reading_res.index() == 1)
    {
        qWarning("Got a GPIO Read Error when handling Quadrature: %d", get<line_read_error>(b_reading_res));
        return;
    }

    const gpio_level b_reading = get<gpio_level>(b_reading_res);
    // ReSharper disable once CppTooWideScope
    const bool right_rotating = ((b_reading == gpio_level::LOW) && edge == gpio_edge::RISING_EDGE) ||
                                ((b_reading == gpio_level::HIGH) && edge == gpio_edge::FALLING_EDGE);
    if (right_rotating)
        emit dse->rightRot();
    else
        emit dse->leftRot();
}
void ROT_B(const gpio_edge edge)
{
    const Result<gpio_level, line_read_error> a_reading_res = read_gpio(gpio_input::GPIO2);
    if (a_reading_res.index() == 1)
    {
        qWarning("Got a GPIO Read Error when handling Quadrature: %d", get<line_read_error>(a_reading_res));
        return;
    }

    const gpio_level a_reading = get<gpio_level>(a_reading_res);
    // ReSharper disable once CppTooWideScope
    const bool right_rotating = ((a_reading == gpio_level::HIGH) && edge == gpio_edge::RISING_EDGE) ||
                                ((a_reading == gpio_level::LOW) && edge == gpio_edge::FALLING_EDGE);
    if (right_rotating)
        emit dse->rightRot();
    else
        emit dse->leftRot();
}
void ROT_S(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->pushRot();
    else
        emit dse->pushRotReleased();
}
void OUT(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->outButtonPressed();
    else
        emit dse->outButtonReleased();
}
void ERR(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->errButtonPressed();
    else
        emit dse->errButtonReleased();
}
void STG(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->settingsButtonPressed();
    else
        emit dse->settingsButtonReleased();
}

void F1(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->f1ButtonPressed();
    else
        emit dse->f1ButtonReleased();
}
void F2(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->f2ButtonPressed();
    else
        emit dse->f2ButtonReleased();
}

// gpio handlers
const std::map<gpio_input, void (*)(gpio_edge)> gpio_handler_funcs{
    { gpio_input::GPIO1, &F1 },    { gpio_input::GPIO2, &ROT_A }, { gpio_input::GPIO3, &OUT },
    { gpio_input::GPIO4, &F2 },    { gpio_input::GPIO5, &STG },   { gpio_input::GPIO6, &ERR },
    { gpio_input::GPIO7, &ROT_S }, { gpio_input::GPIO8, &ROT_B },
};

class GPIOMonitorTask final : public QThread
{
  public:
    explicit GPIOMonitorTask(const gpio_input i) : i(i) {}

  private:
    const gpio_input i;

    void run() override;
};

void GPIOMonitorTask::run()
{
    qInfo("%s thread started", GPIO_inputs_info.at(i).enum_name.c_str());
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        const Result<gpio_edge, line_read_error> l_event = wait_for_line_event(i);
        if (l_event.index() == 1)
        {
#ifdef USING_TARGET_dev
            if (get<line_read_error>(l_event) == line_read_error::DEV_DUMMY_DATA ||
                get<line_read_error>(l_event) == line_read_error::TIMEOUT)
                continue;
#endif
            qWarning("GPIO READ ERROR: %d", static_cast<int>(get<line_read_error>(l_event)));
            continue;
        }
        gpio_handler_funcs.at(i)(std::get<gpio_edge>(l_event));
    }
    qInfo("KILL GPIO thread: %s", GPIO_inputs_info.at(i).enum_name.c_str());
}

namespace GPIOTask
{
static std::vector<std::unique_ptr<GPIOMonitorTask>> gpio_monitor_threads;
Result<std::monostate, GPIO_setup_errors>            setup()
{
    qInfo("Initializing GPIO Threads");
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

        std::unique_ptr<GPIOMonitorTask> new_gpio_thread = std::make_unique<GPIOMonitorTask>(gpio_input);
        gpio_monitor_threads.push_back(std::move(new_gpio_thread));
        gpio_monitor_threads.back()->start();
    }
    qInfo("GPIO Thread Initialization Complete, has %s errors", (any_gpio_has_err ? "some" : "no"));
    if (any_gpio_has_err)
        return GPIO_setup_errors::LINE_SETUP_ERROR;
    return std::monostate{};
}
Result<std::monostate, GPIO_teardown_errors> teardown()
{
    qInfo("Terminating GPIO Threads");
    for (auto &gpio_thread : gpio_monitor_threads)
        gpio_thread->requestInterruption();
    for (auto &gpio_thread : gpio_monitor_threads)
        gpio_thread->wait();
    qInfo("GPIO Threads Sucessfully Terminated");
    return std::monostate{};
}
} // namespace GPIOTask