#include "gpio.h"

#include <gpiod.hpp>
#include <variant>
#include <QLoggingCategory>

typedef struct
{
    std::string chip;
    int         line;
} gpio_hw_info;

// Derived from gpiofuckshit.txt
const std::map<gpio_input, gpio_hw_info> GPIO_inputs_hw_info{
    { gpio_input::GPIO1, { .chip = "/dev/gpiochip2", .line = 8 } },
    { gpio_input::GPIO2, { .chip = "/dev/gpiochip2", .line = 9 } },
    { gpio_input::GPIO3, { .chip = "/dev/gpiochip2", .line = 12 } },
    { gpio_input::GPIO4, { .chip = "/dev/gpiochip2", .line = 13 } },
    { gpio_input::GPIO5, { .chip = "/dev/gpiochip6", .line = 1 } },
    { gpio_input::GPIO6, { .chip = "/dev/gpiochip6", .line = 2 } },
    { gpio_input::GPIO7, { .chip = "/dev/gpiochip5", .line = 26 } },
    { gpio_input::GPIO8, { .chip = "/dev/gpiochip5", .line = 28 } },
};

std::map<gpio_input, gpiod::line> gpio_lines;

enum class gpiod_line_init_error
{
    INPUT_RANGE_ERROR,   // developer error
    CHIP_INIT_ERROR,     // system error
    LINE_REQUEST_ERROR,  // system error
    UNKNOWN_SYSTEM_ERROR // system error + deadly case value -> time to check logs
};
Result<gpiod::line, gpiod_line_init_error> create_gpio_input_pin(const gpio_input i)
{
    const auto [gpio_name, gpio_enum_name] = GPIO_inputs_info.at(i);
    const std::string err_prefix           = "[" + gpio_name + " on " + gpio_enum_name + "]:";
    try
    {
        const auto [chip_loc, line_num] = GPIO_inputs_hw_info.at(i);
        const gpiod::chip c(chip_loc);
        gpiod::line       l = c.get_line(line_num);
        l.request(
            { "dimos_gpio_service", gpiod::line_request::EVENT_BOTH_EDGES, gpiod::line_request::FLAG_BIAS_PULL_DOWN });
        return l;
    }
    catch (std::system_error &e)
    {
        qCritical() << err_prefix.c_str() << e.what();
        switch (e.code().value())
        {
            case 2:
                return gpiod_line_init_error::CHIP_INIT_ERROR;
            case 22:
                return gpiod_line_init_error::LINE_REQUEST_ERROR;
            default:
                return gpiod_line_init_error::UNKNOWN_SYSTEM_ERROR;
        }
    }
    // very possibly a line error
    catch (std::range_error &)
    {
        qCritical() << err_prefix.c_str() << "range error";
        return gpiod_line_init_error::INPUT_RANGE_ERROR;
    }
    // line error or line request error
    catch (std::out_of_range &)
    {
        qCritical() << err_prefix.c_str() << "out of range error";
        return gpiod_line_init_error::INPUT_RANGE_ERROR;
    }
}

// PUBLIC FUNCTIONS
std::map<gpio_input, bool> gpio_init()
{
    std::map<gpio_input, bool> has_error{};
    for (auto &i : gpio_inputs)
    {
        Result<gpiod::line, gpiod_line_init_error> r = create_gpio_input_pin(i);
        if (r.index() == 1)
        {
            has_error[i] = true;
            continue;
        }
        gpio_lines[i] = std::get<gpiod::line>(r);
    }
    return has_error;
}

Result<gpio_edge, line_read_error> wait_for_line_event(const gpio_input i)
{
    const gpiod::line &l = gpio_lines[i];

    try
    {
        const auto l_event = l.event_read(); // todo make this react to QThread::requestInterruption
        return l_event.event_type == gpiod::line_event::RISING_EDGE ? RISING_EDGE : FALLING_EDGE;
    }
    catch (std::system_error &e)
    {
        std::string s = "Line Read Error ";
        s += e.what();
        qErrnoWarning(s.c_str());
        // e.code().value() returns 1
        return LINE_READ_SYSTEM_ERROR;
    }
}