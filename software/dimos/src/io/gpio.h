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
    GPIO8
};

// MAKE SURE THIS IS UPDATED WITH ABOVE!!!

constexpr int               GPIO_COUNT  = 8;
inline constexpr std::array gpio_inputs = {
    gpio_input::GPIO1, gpio_input::GPIO2, gpio_input::GPIO3, gpio_input::GPIO4,
    gpio_input::GPIO5, gpio_input::GPIO6, gpio_input::GPIO7, gpio_input::GPIO8
};

typedef struct
{
    std::string name, enum_name;
} gpio_info;

const std::map<gpio_input, gpio_info> GPIO_inputs_info {
    { gpio_input::GPIO1, { .name = "F1", .enum_name = "GPIO1" } },
    { gpio_input::GPIO2, { .name = "ROT_A", .enum_name = "GPIO2" } },
    { gpio_input::GPIO3, { .name = "OUT", .enum_name = "GPIO3" } },
    { gpio_input::GPIO4, { .name = "F2", .enum_name = "GPIO4" } },
    { gpio_input::GPIO5, { .name = "STG", .enum_name = "GPIO5" } },
    { gpio_input::GPIO6, { .name = "ERR", .enum_name = "GPIO6" } },
    { gpio_input::GPIO7, { .name = "ROT_S", .enum_name = "GPIO7" } },
    { gpio_input::GPIO8, { .name = "ROT_B", .enum_name = "GPIO8" } },
};

std::map<gpio_input, bool> gpio_init();

/**
 * \brief Waits for a line event on the given gpio input
 * \return The line event that occurred (either rising or falling)
 */
enum class gpio_edge
{
    RISING_EDGE,
    FALLING_EDGE
};
enum class line_read_error
{
    LINE_READ_SYSTEM_ERROR, // caused by not requesting?
    DEV_DUMMY_DATA,         // returned on dev gpio
    TIMEOUT
};
Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i);

enum class gpio_level {
    LOW,
    HIGH
};
Result<gpio_level, line_read_error> read_gpio(gpio_input i);