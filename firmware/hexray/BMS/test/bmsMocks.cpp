#include "io_canQueues.hpp"

io::queue<io::CanMsg, 250> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

#include "io_adbms.hpp"
#include "test_fakes.hpp"

#include <cmath>

namespace
{
// Mirror of the register encodings in app_segments_internal.hpp so the mocked
// reads round-trip through the same conversions the app uses.
constexpr float REG_VOLTAGE_SCALE  = 150e-6f;
constexpr float REG_VOLTAGE_OFFSET = 1.5f;

constexpr float V_REF2    = 3.0f;
constexpr float R_SERIES  = 10e3f;
constexpr float R_NOMINAL = 10e3f;
constexpr float T_NOMINAL = 298.15f;
constexpr float BETA_COEFF = 3610.0f;
constexpr float KELVIN_OFFSET = 273.15f;

int16_t voltageToReg(const float voltage)
{
    return static_cast<int16_t>((voltage - REG_VOLTAGE_OFFSET) / REG_VOLTAGE_SCALE);
}

// Inverse of convertRegToTemp(): produces the GPIO register a thermistor at the
// given Celsius temperature would read.
int16_t tempToReg(const float temp_c)
{
    const float temp_k     = temp_c + KELVIN_OFFSET;
    const float resistance = R_NOMINAL * std::exp(BETA_COEFF * (1.0f / temp_k - 1.0f / T_NOMINAL));
    const float voltage    = V_REF2 * resistance / (resistance + R_SERIES);
    return voltageToReg(voltage);
}
} // namespace

namespace io::adbms
{
// Backing storage for the read mocks, configured through the fakes::segments::* setters.
namespace
{
    Cells<int16_t>      cell_voltage_storage{};
    Cells<int16_t>      secondary_cell_voltage_storage{};
    ThermGpios<int16_t> therm_gpio_storage{};
    Segments<int16_t>   seg_voltage_storage{};
} // namespace
} // namespace io::adbms

namespace io::adbms::write
{
[[nodiscard]] result<void> pwmReg(const Segments<PWMConfig> &pwm_config)
{
    return result<void>{};
}

[[nodiscard]] result<void> configReg(const Segments<SegmentConfig> &config)
{
    return result<void>{};
}
} // namespace io::adbms::write

namespace io::adbms::read
{
[[nodiscard]] Segments<result<SegmentConfig>> configReg()
{
    return Segments<result<SegmentConfig>>{};
}

[[nodiscard]] Segments<result<PWMConfig>> pwmReg()
{
    return Segments<result<PWMConfig>>{};
}

[[nodiscard]] Cells<result<int16_t>> cellVoltage()
{
    Cells<result<int16_t>> out{};
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            out[seg][cell] = cell_voltage_storage[seg][cell];
        }
    }
    return out;
}

[[nodiscard]] Cells<result<int16_t>> secondaryCellVoltage()
{
    Cells<result<int16_t>> out{};
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            out[seg][cell] = secondary_cell_voltage_storage[seg][cell];
        }
    }
    return out;
}

[[nodiscard]] Segments<result<int16_t>> segVoltage()
{
    Segments<result<int16_t>> out{};
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        out[seg] = seg_voltage_storage[seg];
    }
    return out;
}

[[nodiscard]] ThermGpios<result<int16_t>> thermGpioVoltage()
{
    ThermGpios<result<int16_t>> out{};
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t gpio = 0; gpio < THERM_GPIOS_PER_SEGMENT; gpio++)
        {
            out[seg][gpio] = therm_gpio_storage[seg][gpio];
        }
    }
    return out;
}

[[nodiscard]] Segments<StatusGroupsRes> status()
{
    return Segments<StatusGroupsRes>{};
}
} // namespace io::adbms::read

namespace io::adbms::command
{
[[nodiscard]] result<void> startCellsAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> startAuxAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> pollSecondaryCellsAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> pollCellsAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> pollAuxAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> startBalance()
{
    return result<void>{};
}

[[nodiscard]] result<void> stopBalance()
{
    return result<void>{};
}

[[nodiscard]] result<void> owcCells(OpenWireSwitch owcSwitch)
{
    return result<void>{};
}
} // namespace io::adbms::command

namespace io::adbms::clear
{
[[nodiscard]] result<void> aux()
{
    return result<void>{};
}

[[nodiscard]] result<void> flags()
{
    return result<void>{};
}

[[nodiscard]] result<void> cell()
{
    return result<void>{};
}

[[nodiscard]] result<void> secondaryCell()
{
    return result<void>{};
}

[[nodiscard]] result<void> filteredCell()
{
    return result<void>{};
}

[[nodiscard]] result<void> stat()
{
    return result<void>{};
}
} // namespace io::adbms::clear

namespace fakes::segments
{
void setCellVoltages(const std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &voltages)
{
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const int16_t reg                                  = voltageToReg(voltages[seg][cell]);
            io::adbms::cell_voltage_storage[seg][cell]           = reg;
            io::adbms::secondary_cell_voltage_storage[seg][cell] = reg;
        }
    }
}

void setCellVoltage(const size_t segment, const size_t cell, const float voltage)
{
    const int16_t reg                                        = voltageToReg(voltage);
    io::adbms::cell_voltage_storage[segment][cell]           = reg;
    io::adbms::secondary_cell_voltage_storage[segment][cell] = reg;
}

void setPackVoltageEvenly(const float pack_voltage)
{
    const float   cell_voltage = pack_voltage / (NUM_SEGMENTS * CELLS_PER_SEGMENT);
    const int16_t reg          = voltageToReg(cell_voltage);
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            io::adbms::cell_voltage_storage[seg][cell]           = reg;
            io::adbms::secondary_cell_voltage_storage[seg][cell] = reg;
        }
    }
}

void setCellTemperatures(const std::array<std::array<float, THERM_GPIOS_PER_SEGMENT>, NUM_SEGMENTS> &temperatures)
{
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t gpio = 0; gpio < THERM_GPIOS_PER_SEGMENT; gpio++)
        {
            io::adbms::therm_gpio_storage[seg][gpio] = tempToReg(temperatures[seg][gpio]);
        }
    }
}

void SetAuxRegs(const float temperature_c)
{
    const int16_t reg = tempToReg(temperature_c);
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t gpio = 0; gpio < THERM_GPIOS_PER_SEGMENT; gpio++)
        {
            io::adbms::therm_gpio_storage[seg][gpio] = reg;
        }
    }
}

void SetAuxReg(const uint8_t segment, const uint8_t gpio, const float temperature_c)
{
    io::adbms::therm_gpio_storage[segment][gpio] = tempToReg(temperature_c);
}
} // namespace fakes::segments
