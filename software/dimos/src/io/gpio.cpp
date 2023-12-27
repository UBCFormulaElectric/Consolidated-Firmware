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
    { GPIO1, { .chip = "/dev/gpiochip2", .line = 8 } },  { GPIO2, { .chip = "/dev/gpiochip2", .line = 9 } },
    { GPIO3, { .chip = "/dev/gpiochip2", .line = 12 } }, { GPIO4, { .chip = "/dev/gpiochip2", .line = 13 } },
    { GPIO5, { .chip = "/dev/gpiochip6", .line = 1 } },  { GPIO6, { .chip = "/dev/gpiochip6", .line = 2 } },
    { GPIO7, { .chip = "/dev/gpiochip5", .line = 26 } }, { GPIO8, { .chip = "/dev/gpiochip5", .line = 28 } },
};

std::array<gpiod::line, GPIO_COUNT> gpio_lines;

enum gpiod_line_init_error
{
    INPUT_RANGE_ERROR,   // developer error
    CHIP_INIT_ERROR,     // system error
    LINE_REQUEST_ERROR,  // system error
    UNKNOWN_SYSTEM_ERROR // system error + deadly case value -> time to check logs
};
Result<gpiod::line, gpiod_line_init_error> create_gpio_input_pin(const gpio_input i)
{
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
        qErrnoWarning(e.what());
        switch (e.code().value())
        {
            case 2:
                return CHIP_INIT_ERROR;
            case 22:
                return LINE_REQUEST_ERROR;
            default:
                return UNKNOWN_SYSTEM_ERROR;
        }
    }
    catch (std::range_error &)
    {
        qErrnoWarning("range error");
        // very possibly a line error
        return INPUT_RANGE_ERROR;
    }
    catch (std::out_of_range &)
    {
        qErrnoWarning("out of range error");
        // line error or line request error
        return INPUT_RANGE_ERROR;
    }
}

// PUBLIC FUNCTIONS

std::array<bool, GPIO_COUNT> gpio_init()
{
    std::array<bool, GPIO_COUNT> has_error{};
    has_error.fill(false);
    for (int i = 0; i < GPIO_COUNT; i++)
    {
        Result<gpiod::line, gpiod_line_init_error> r = create_gpio_input_pin(static_cast<gpio_input>(i));
        if (r.index() == 1)
        {
            has_error[i] = true;
            continue;
        }
        gpio_lines[i] = std::get<gpiod::line>(r);
    }
    return has_error;
}

Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i)
{
    const gpiod::line &l = gpio_lines[i];

    try
    {
        const auto l_event = l.event_read();
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