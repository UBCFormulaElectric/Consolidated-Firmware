#pragma once
#include "types.h"

#include <array>
#include <map>
#include <string>

enum class gpio_input
{
    GPIO1,
    GPIO2,
    GPIO3,
    GPIO4,
    GPIO5,
    GPIO6,
    GPIO7,
    GPIO8,
};

enum class gpio_output
{
    GPIO_PROGRAM,
};

// MAKE SURE THIS IS UPDATED WITH ABOVE!!!

constexpr int                               GPIO_IN_COUNT = 8;
const std::array<gpio_input, GPIO_IN_COUNT> gpio_inputs   = { gpio_input::GPIO1, gpio_input::GPIO2, gpio_input::GPIO3,
                                                              gpio_input::GPIO4, gpio_input::GPIO5, gpio_input::GPIO6,
                                                              gpio_input::GPIO7, gpio_input::GPIO8 };

constexpr int                                 GPIO_OUT_COUNT = 1;
const std::array<gpio_output, GPIO_OUT_COUNT> gpio_outputs   = { gpio_output::GPIO_PROGRAM };

typedef struct
{
    std::string name, enum_name;
} gpio_info;

const std::map<gpio_input, gpio_info> gpio_inputs_metadata{
    { gpio_input::GPIO1, { .name = "F1", .enum_name = "GPIO1" } },
    { gpio_input::GPIO2, { .name = "ROT_A", .enum_name = "GPIO2" } },
    { gpio_input::GPIO3, { .name = "OUT", .enum_name = "GPIO3" } },
    { gpio_input::GPIO4, { .name = "F2", .enum_name = "GPIO4" } },
    { gpio_input::GPIO5, { .name = "STG", .enum_name = "GPIO5" } },
    { gpio_input::GPIO6, { .name = "ERR", .enum_name = "GPIO6" } },
    { gpio_input::GPIO7, { .name = "ROT_S", .enum_name = "GPIO7" } },
    { gpio_input::GPIO8, { .name = "ROT_B", .enum_name = "GPIO8" } },
};

const std::map<gpio_output, gpio_info> gpio_outputs_metadata{
    { gpio_output::GPIO_PROGRAM, { .name = "PROGRAM", .enum_name = "GPIO_PROGRAM" } },
};

std::map<gpio_input, bool> gpio_init();

enum class gpio_edge
{
    RISING_EDGE,
    FALLING_EDGE
};
enum class line_read_error
{
    LINE_READ_SYSTEM_ERROR, // caused by not requesting?
    DEV_DUMMY_DATA,         // returned on dev gpio
    TIMEOUT,
    GPIO_NOT_INITALIZED
};
const std::map<line_read_error, std::string> line_read_error_str{ { line_read_error::LINE_READ_SYSTEM_ERROR,
                                                                    "LINE_READ_SYSTEM_ERROR" },
                                                                  { line_read_error::DEV_DUMMY_DATA, "DEV_DUMMY_DATA" },
                                                                  { line_read_error::TIMEOUT, "TIMEOUT" } };
/**
 * Waits for a line event on the given gpio input
 * @param i the gpio input to wait for an event on
 * @return The line event that occurred (either rising or falling)
 */
Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i);

enum class gpio_level
{
    LOW,
    HIGH
};
/**
 * Reads the instantaneous value of the gpio input
 * @param i the gpio input to read
 * @return the value of the gpio input
 */
Result<gpio_level, line_read_error> read_gpio(const gpio_input i);

enum class line_write_error
{
    UNKNOWN_ERROR
};
Result<std::monostate, line_write_error> write_gpio(const gpio_output i, bool level);