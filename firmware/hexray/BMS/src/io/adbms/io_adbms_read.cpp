#include "io_adbms_internal.hpp"
#include "app_pack.hpp"
#include "io_time.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

namespace
{

constexpr float   ADC_LSB_V    = 150e-6f;
constexpr float   ADC_OFFSET_V = 1.5f;
constexpr int16_t ADC_DEFAULT  = INT16_MIN;

constexpr std::array<uint16_t, 5> RDCV{ io::adbms::RDCVA, io::adbms::RDCVB, io::adbms::RDCVC, io::adbms::RDCVD,
                                        io::adbms::RDCVE };
constexpr std::array<uint16_t, 5> RDSV{ io::adbms::RDSVA, io::adbms::RDSVB, io::adbms::RDSVC, io::adbms::RDSVD,
                                        io::adbms::RDSVE };
constexpr std::array<uint16_t, 3> RDAUX{ io::adbms::RDAUXA, io::adbms::RDAUXB, io::adbms::RDAUXC };

constexpr size_t CELLS_PER_GROUP = io::adbms::REG_GROUP_SIZE / sizeof(int16_t);
constexpr size_t GPIOS_PER_GROUP = io::adbms::REG_GROUP_SIZE / sizeof(int16_t);

constexpr float  THERM_VREF2_V    = 3.0f;
constexpr float  THERM_R_SERIES   = 10e3f;
constexpr float  THERM_R_NOMINAL  = 10e3f;
constexpr float  THERM_T_NOMINAL  = 298.15f;
constexpr float  THERM_BETA       = 3610.0f;
constexpr float  KELVIN_OFFSET    = 273.15f;
constexpr float  THERM_LUT_STEP_V = 0.01f;
constexpr size_t THERM_LUT_SIZE   = 299;

constexpr std::array<float, THERM_LUT_SIZE> THERM_LUT = []
{
    std::array<float, THERM_LUT_SIZE> lut{};
    for (size_t i = 0; i < THERM_LUT_SIZE; i++)
    {
        const float voltage    = static_cast<float>(i + 1) * THERM_LUT_STEP_V;
        const float resistance = THERM_R_SERIES * voltage / (THERM_VREF2_V - voltage);
        lut[i] = 1.0f / (1.0f / THERM_T_NOMINAL + std::log(resistance / THERM_R_NOMINAL) / THERM_BETA) - KELVIN_OFFSET;
    }
    return lut;
}();
} // namespace

namespace io::adbms::read
{
result<void> configReg(Segments<CFGA> &config_a, Segments<CFGB> &config_b)
{
    Segments<RegGroup> config_regs;

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDCFGA, config_regs));
    config_a = std::bit_cast<Segments<CFGA>>(config_regs);

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDCFGB, config_regs));
    config_b = std::bit_cast<Segments<CFGB>>(config_regs);

    return {};
}

result<void> pwmReg(Segments<PWMA> &pwm_a, Segments<PWMB> &pwm_b)
{
    Segments<RegGroup> pwm_regs;

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDPWMA, pwm_regs));
    pwm_a = std::bit_cast<Segments<PWMA>>(pwm_regs);

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDPWMB, pwm_regs));
    pwm_b = std::bit_cast<Segments<PWMB>>(pwm_regs);

    return {};
}

result<void> cAdc(const bool redundant, app::pack::VoltStats &stats)
{
    std::array<Segments<RegGroup>, RDCV.size()> cell_regs;
    Segments<RegGroup>                          stat_c{};

    for (size_t group = 0; group < RDCV.size(); group++)
    {
        RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDCV[group], cell_regs[group]));
    }

    if (redundant)
    {
        RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATC, stat_c));
    }

    app::pack::LocatedValue max{ .value = std::numeric_limits<float>::lowest() };
    app::pack::LocatedValue min{ .value = std::numeric_limits<float>::max() };

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const RegGroup &reg = cell_regs[cell / CELLS_PER_GROUP][seg];

            if (!reg)
            {
                voltages[seg][cell] = std::unexpected(reg.error());
                continue;
            }
            
            const size_t    lsb = (cell % CELLS_PER_GROUP) * sizeof(int16_t);
            const int16_t   raw = static_cast<int16_t>(reg[lsb] | reg[lsb + 1] << 8);

            if (raw == ADC_DEFAULT)
            {
                stats.voltages[seg][cell] = ErrorCode::INVALID_READING;
                continue;
            }

            if (redundant)
            {
                if ((stat_c[seg][cell / 8] >> (cell % 8)) & 1)
                {
                    stats.voltages[seg][cell] = ErrorCode::MISMATCH;
                    continue;
                }
            }

            const float voltage       = static_cast<float>(raw) * ADC_LSB_V + ADC_OFFSET_V;
            stats.voltages[seg][cell] = voltage;
            if (voltage > max.value)
                max = { voltage, static_cast<uint8_t>(seg), static_cast<uint8_t>(cell) };
            if (voltage < min.value)
                min = { voltage, static_cast<uint8_t>(seg), static_cast<uint8_t>(cell) };
        }
    }

    if (max.value >= min.value)
    {
        stats.max = max;
        stats.min = min;
    }

    stats.updated_ms = io::time::getCurrentMs();
    return {};
}

result<void> sAdc(PackCells<result<float>> &voltages)
{
    std::array<Segments<result<RegGroup>>, RDSV.size()> regs;

    for (size_t group = 0; group < RDSV.size(); group++)
    {
        RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSV[group], regs[group]));
    }

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const result<RegGroup> &reg = regs[cell / CELLS_PER_GROUP][seg];

            if (!reg)
            {
                voltages[seg][cell] = std::unexpected(reg.error());
                continue;
            }

            const size_t  lsb = (cell % CELLS_PER_GROUP) * sizeof(int16_t);
            const int16_t raw = static_cast<int16_t>(reg[lsb] | reg[lsb + 1] << 8);

            if (raw == ADC_DEFAULT)
            {
                voltages[seg][cell] = std::unexpected(ErrorCode::INVALID_READING);
                continue;
            }

            voltages[seg][cell] = static_cast<float>(raw) * ADC_LSB_V + ADC_OFFSET_V;
        }
    }

    return {};
}

result<void> xAdc(const ThermistorMux mux, app::pack::TempStats &stats, PackTherms<result<float>> &voltages)
{
    std::array<Segments<result<RegGroup>>, RDAUX.size()> gpio_regs;

    for (size_t group = 0; group < RDAUX.size(); group++)
    {
        RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDAUX[group], gpio_regs[group]));
    }

    const size_t first_therm = mux == ThermistorMux::EVEN ? 0 : THERM_GPIOS_PER_SEGMENT;

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t gpio = 0; gpio < THERM_GPIOS_PER_SEGMENT && first_therm + gpio < THERMISTORS_PER_SEGMENT; gpio++)
        {
            const size_t            therm = first_therm + gpio;
            const result<RegGroup> &reg   = gpio_regs[gpio / GPIOS_PER_GROUP][seg];

            if (!reg)
            {
                voltages[seg][therm]           = std::unexpected(reg.error());
                stats.temperatures[seg][therm] = std::unexpected(reg.error());
                continue;
            }

            const size_t  lsb = (gpio % GPIOS_PER_GROUP) * sizeof(int16_t);
            const int16_t raw = static_cast<int16_t>((*reg)[lsb] | (*reg)[lsb + 1] << 8);

            if (raw == ADC_DEFAULT)
            {
                voltages[seg][therm]           = std::unexpected(ErrorCode::INVALID_READING);
                stats.temperatures[seg][therm] = std::unexpected(ErrorCode::INVALID_READING);
                continue;
            }

            const float voltage  = static_cast<float>(raw) * ADC_LSB_V + ADC_OFFSET_V;
            voltages[seg][therm] = voltage;

            const long lut_index = std::lround(voltage / THERM_LUT_STEP_V) - 1;
            if (lut_index < 0 || lut_index >= static_cast<long>(THERM_LUT_SIZE))
                stats.temperatures[seg][therm] = std::unexpected(ErrorCode::INVALID_READING);
            else
                stats.temperatures[seg][therm] = THERM_LUT[static_cast<size_t>(lut_index)];
        }
    }

    app::pack::Reading max{ .value = std::numeric_limits<float>::lowest() };
    app::pack::Reading min{ .value = std::numeric_limits<float>::max() };

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            if (!stats.temperatures[seg][therm])
                continue;

            const float temp = stats.temperatures[seg][therm].value();
            if (temp > max.value)
                max = { temp, static_cast<uint8_t>(seg), static_cast<uint8_t>(therm) };
            if (temp < min.value)
                min = { temp, static_cast<uint8_t>(seg), static_cast<uint8_t>(therm) };
        }
    }

    if (max.value >= min.value)
    {
        stats.max = max;
        stats.min = min;
    }

    stats.updated_ms = io::time::getCurrentMs();
    return {};
}

result<void> flags(app::pack::ADBMS6830Diag &diag)
{
    Segments<RegGroup> stat_regs;

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATC, stat_regs));
    const Segments<STATC> stat_c = std::bit_cast<Segments<STATC>>(stat_regs);

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATD, stat_regs));
    const Segments<STATD> stat_d = std::bit_cast<Segments<STATD>>(stat_regs);

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            diag.uv_ok[seg][cell] = !((stat_d[seg].covuv >> (2 * cell)) & 1U);
            diag.ov_ok[seg][cell] = !((stat_d[seg].covuv >> (2 * cell + 1)) & 1U);
        }

        diag.therm_shdn_ok[seg] = !stat_c[seg].thsd;
        diag.self_test_ok[seg] =
            !(stat_c[seg].ced || stat_c[seg].cmed || stat_c[seg].sed || stat_c[seg].smed || stat_c[seg].tmodchk ||
              stat_c[seg].oscchk);
        diag.supply_ok[seg] =
            !(stat_c[seg].va_ov || stat_c[seg].va_uv || stat_c[seg].vd_ov || stat_c[seg].vd_uv || stat_c[seg].vde ||
              stat_c[seg].vdel);
    }

    diag.updated_ms = io::time::getCurrentMs();
    return {};
}
} // namespace io::adbms::read