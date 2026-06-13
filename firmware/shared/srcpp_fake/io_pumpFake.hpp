#pragma once

#include <cstdint>

namespace fakes::io::rPump
{
extern uint8_t percentage;

void    set_readPercentage(const uint8_t value);
uint8_t get_readPercentage();
} // namespace fakes::io::rPump