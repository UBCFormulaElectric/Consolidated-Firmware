#pragma once

#include <cstdint>
#include <span>

namespace io::powerMonitoring
{
void read_register(std::uint16_t reg, std::span<uint8_t> data);
void write_register(std::uint16_t reg, std::span<uint8_t> data);
void io_power_monitoring_refresh(void);
void read_voltage(uint8_t ch, float* voltage);
void read_current(uint8_t ch, float* current);
void read_power(uint8_t ch, float *power);
} //namespace io::powerMonitoring 

