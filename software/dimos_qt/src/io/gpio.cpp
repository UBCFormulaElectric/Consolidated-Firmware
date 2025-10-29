#include "gpio.h"

#include <gpiod.hpp>
#include <variant>
#include <QLoggingCategory>

typedef struct
{
    std::string chip;
    int         line;
} gpio_hw_metadata;

// Derived from gpiofuckshit.txt
const std::map<gpio_input, gpio_hw_metadata> gpio_input_hw_metadata{
    { gpio_input::GPIO1, { .chip = "/dev/gpiochip2", .line = 8 } },
    { gpio_input::GPIO2, { .chip = "/dev/gpiochip2", .line = 9 } },
    { gpio_input::GPIO3, { .chip = "/dev/gpiochip2", .line = 12 } },
    { gpio_input::GPIO4, { .chip = "/dev/gpiochip2", .line = 13 } },
    { gpio_input::GPIO5, { .chip = "/dev/gpiochip6", .line = 1 } },
    { gpio_input::GPIO6, { .chip = "/dev/gpiochip6", .line = 2 } },
    { gpio_input::GPIO7, { .chip = "/dev/gpiochip5", .line = 26 } },
    { gpio_input::GPIO8, { .chip = "/dev/gpiochip5", .line = 28 } },
};

const std::map<gpio_output, gpio_hw_metadata> gpio_output_hw_metadata{ { gpio_output::GPIO_PROGRAM,
                                                                         { .chip = "/dev/gpiochip3", .line = 6 } } };

std::map<gpio_input, gpiod::line>  gpio_input_lines;
std::map<gpio_output, gpiod::line> gpio_output_lines;

const std::map<gpio_input, ::std::bitset<32>> gpio_flags{
    { gpio_input::GPIO1, gpiod::line_request::FLAG_BIAS_PULL_DOWN },
    { gpio_input::GPIO2, gpiod::line_request::FLAG_BIAS_PULL_DOWN },
    { gpio_input::GPIO3, gpiod::line_request::FLAG_BIAS_PULL_DOWN },
    { gpio_input::GPIO4, gpiod::line_request::FLAG_BIAS_PULL_DOWN },
    { gpio_input::GPIO5, gpiod::line_request::FLAG_BIAS_PULL_DOWN },
    { gpio_input::GPIO6, gpiod::line_request::FLAG_BIAS_PULL_DOWN },
    { gpio_input::GPIO7, gpiod::line_request::FLAG_BIAS_PULL_UP },
    { gpio_input::GPIO8, gpiod::line_request::FLAG_BIAS_PULL_DOWN }
};

const std::map<gpio_output, ::std::bitset<32>> gpio_output_flags{ { gpio_output::GPIO_PROGRAM,
                                                                    gpiod::line_request::FLAG_BIAS_PULL_DOWN } };

enum class gpiod_line_init_error
{
    INPUT_RANGE_ERROR,   // developer error
    CHIP_INIT_ERROR,     // system error
    LINE_REQUEST_ERROR,  // system error
    UNKNOWN_SYSTEM_ERROR // system error + deadly case value -> time to check logs
};

Result<gpiod::line, gpiod_line_init_error> create_gpio_input_pin(const gpio_input i)
{
    const auto [gpio_name, gpio_enum_name] = gpio_inputs_metadata.at(i);
    const std::string err_prefix           = "[" + gpio_name + " on " + gpio_enum_name + "]:";
    try
    {
        const auto [chip_loc, line_num] = gpio_input_hw_metadata.at(i);
        const gpiod::chip c(chip_loc);
        gpiod::line       l = c.get_line(line_num);
        l.request({ "dimos_gpio_service", gpiod::line_request::EVENT_BOTH_EDGES, gpio_flags.at(i) });
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

enum class gpio_output_init_error
{
    UNKNOWN_ERROR
};

Result<gpiod::line, gpio_output_init_error> create_gpio_output_pin(const gpio_output i)
{
    try
    {
        const auto [chip_loc, line_num] = gpio_output_hw_metadata.at(i);
        const gpiod::chip c(chip_loc);
        gpiod::line       l = c.get_line(line_num);
        l.request({ "dimos_gpio_service", gpiod::line_request::DIRECTION_OUTPUT, gpio_output_flags.at(i) }, false);
        return l;
    }
    catch (...)
    {
        return gpio_output_init_error::UNKNOWN_ERROR;
    }
}

// PUBLIC FUNCTIONS
std::map<gpio_input, bool> gpio_init()
{
    std::map<gpio_input, bool> has_error{};
    for (auto &i : gpio_inputs)
    {
        Result<gpiod::line, gpiod_line_init_error> r = create_gpio_input_pin(i);
        if (r.has_error())
        {
            has_error[i] = true;
            continue;
        }
        gpio_input_lines[i] = r.get_data();
        has_error[i]        = false;
    }
    for (auto &i : gpio_outputs)
    {
        Result<gpiod::line, gpio_output_init_error> r = create_gpio_output_pin(i);
        if (r.has_error())
        {
            qInfo() << "FUCKKK";
            continue;
        }
        qInfo() << "Successfully requested output line for" << gpio_outputs_metadata.at(i).enum_name.c_str();
        gpio_output_lines[i] = r.get_data();
    }

    return has_error;
}

Result<gpio_edge, line_read_error> wait_for_line_event(const gpio_input i)
{
    if (gpio_input_lines.find(i) == gpio_input_lines.end())
        return line_read_error::GPIO_NOT_INITALIZED;
    const gpiod::line &l = gpio_input_lines.at(i);
    try
    {
        //        TODO fix this code as it repeats events
        //        const bool has_event = l.event_wait(std::chrono::milliseconds(500));
        //        if (!has_event)
        //            return line_read_error::TIMEOUT;
        //        return l.get_value() == 1 ? gpio_edge::FALLING_EDGE : gpio_edge::RISING_EDGE;

        const auto l_event = l.event_read(); // todo make this react to QThread::requestInterruption
        return l_event.event_type == gpiod::line_event::RISING_EDGE ? gpio_edge::FALLING_EDGE : gpio_edge::RISING_EDGE;
    }
    catch (std::system_error &e)
    {
        std::string s = "Line Read Error ";
        s += e.what();
        qErrnoWarning(s.c_str());
        // e.code().value() returns 1
        return line_read_error::LINE_READ_SYSTEM_ERROR;
    }
}

Result<gpio_level, line_read_error> read_gpio(const gpio_input i)
{
    if (gpio_input_lines.find(i) == gpio_input_lines.end())
        return line_read_error::GPIO_NOT_INITALIZED;
    const gpiod::line &l = gpio_input_lines.at(i);
    try
    {
        return l.get_value() == 0 ? gpio_level::LOW : gpio_level::HIGH; // TODO validate this is correct
    }
    catch (std::system_error &e)
    {
        std::string s = "Line Read Error ";
        s += e.what();
        qErrnoWarning(s.c_str());
        return line_read_error::LINE_READ_SYSTEM_ERROR;
    }
}

Result<std::monostate, line_write_error> write_gpio(const gpio_output i, bool level)
{
    try
    {
        const gpiod::line &l = gpio_output_lines.at(i);
        l.set_direction_output();
        l.set_value(level);
        return std::monostate{};
    }
    catch (...)
    {
        return line_write_error::UNKNOWN_ERROR;
    }
}
