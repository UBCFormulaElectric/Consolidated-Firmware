#pragma once

namespace io::therm
{
    bool muxSelect(const uint8_t channel);
    float readSelectedTemp();
} // namespace io::therm