#pragma once

#include <cstdint>

#include "io_semaphore.hpp"
#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

namespace app::pack
{

struct OpenWireCheckBuffer
{
    uint32_t                 updated_ms = 0U;
    io::adbms::PackCellFlags cells_owc_ok{};
};

struct ThermOpenWireCheckBuffer
{
    uint32_t                  updated_ms = 0U;
    io::adbms::PackThermFlags therms_owc_ok{};
};

struct BalancingBuffer
{
    bool                          balancing_enabled = false;
    io::adbms::PackCells<uint8_t> pwm{};

    uint32_t                            updated_ms = 0U;
    io::adbms::PackCells<result<float>> voltages{};
};

inline io::adbms::VoltageBuffer       voltage_buffer{};    // Cell voltages
inline io::adbms::TemperatureBuffer   temp_buffer{};       // Cell temperatures
inline OpenWireCheckBuffer            cadc_owc_buffer{};   // C-ADC cell open wire results
inline OpenWireCheckBuffer            sadc_owc_buffer{};   // S-ADC cell open wire results
inline ThermOpenWireCheckBuffer       therm_owc_buffer{};  // Thermistor open wire results
inline io::adbms::SupplyBuffer        supply_buffer{};     // ADBMS supply voltages and die temp
inline io::adbms::DiagnosticBuffer    diag_buffer{};       // ADBMS OV/UV, thermal shdn, self test and supply flags
inline BalancingBuffer                balance_buffer{};    // Balancing enable, PWM duty and settled voltages
inline const io::semaphore::Semaphore buffer_lock{ true }; // Guards all buffers above across tasks

namespace alerts
{
    void init(); // Stop all alert debounce timers
    bool tick(); // Checks alerts, returns true if any fault is active
} // namespace alerts

namespace balancing
{
    void init(); // Clear balancing state
    void tick(); // Enable balancing, recompute PWM duty when voltages are new
    void stop(); // Disable balancing and clear duty
} // namespace balancing

namespace sequence
{
    void init(); // Reset readings and start in MEASURE state
    void tick(); // Run ADBMS state machine, updates buffers above
} // namespace sequence

} // namespace app::pack
