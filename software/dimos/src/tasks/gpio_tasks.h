#pragma once
#include "gpio.h"

namespace GPIOTask
{
enum class GPIO_setup_errors
{
    LINE_SETUP_ERROR
};
const std::map<GPIO_setup_errors, std::string> GPIO_setup_errors_str = { { GPIO_setup_errors::LINE_SETUP_ERROR,
                                                                           "Line Setup Error" } };
Result<std::monostate, GPIO_setup_errors>      setup();

enum class GPIO_teardown_errors
{
};
const std::map<GPIO_teardown_errors, std::string> GPIO_teardown_errors_str = {};
Result<std::monostate, GPIO_teardown_errors>      teardown();
} // namespace GPIOTask