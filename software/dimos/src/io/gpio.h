#pragma once
#include "types.h"

#include <map>
#include <string>

enum gpio_input
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

constexpr int GPIO_COUNT = 8; // MAKE SURE THIS IS UPDATED WITH ABOVE!!!

typedef struct
{
    std::string name;
} gpio_info;

// const std::map<gpio_input, gpio_info> GPIO_inputs_info{
//     { GPIO1, { .name = "ROT_S_SOM" } }, { GPIO2, { .name = "ROT_A_SOM" } }, { GPIO3, { .name = "ROT_B_SOM" } },
//     { GPIO4, { .name = "SW_UP" } },     { GPIO5, { .name = "SW_RIGHT" } },  { GPIO6, { .name = "SW_LEFT" } },
//     { GPIO7, { .name = "SW_DOWN" } },   { GPIO8, { .name = "SW_1" } },
// };

std::array<bool, GPIO_COUNT> gpio_init();

enum gpio_edge
{
    RISING_EDGE,
    FALLING_EDGE
};
enum line_read_error
{
    LINE_READ_SYSTEM_ERROR // caused by not requesting?
};
Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i);