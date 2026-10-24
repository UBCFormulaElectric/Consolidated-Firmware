#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "app_pack.hpp"
#include "io_time.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <expected>
#include <limits>

namespace
{

constexpr float   ADC_LSB_V     = 150e-6f;
constexpr float   ADC_OFFSET_V  = 1.5f;
constexpr float   VPV_LSB_V     = 3.75e-3f;
constexpr float   VPV_OFFSET_V  = 37.5f;
constexpr float   ITMP_LSB_C    = 0.02f;
constexpr float   ITMP_OFFSET_C = -73.0f;
constexpr int16_t ADC_DEFAULT   = INT16_MIN;

constexpr std::array<uint16_t, 5> RDCV{ { io::adbms::RDCVA, io::adbms::RDCVB, io::adbms::RDCVC, io::adbms::RDCVD,
                                          io::adbms::RDCVE } };
constexpr std::array<uint16_t, 5> RDSV{ { io::adbms::RDSVA, io::adbms::RDSVB, io::adbms::RDSVC, io::adbms::RDSVD,
                                          io::adbms::RDSVE } };
constexpr std::array<uint16_t, 3> RDAUX{ { io::adbms::RDAUXA, io::adbms::RDAUXB, io::adbms::RDAUXC } };

constexpr size_t CELLS_PER_GROUP = io::adbms::REG_GROUP_SIZE / sizeof(int16_t);
constexpr size_t GPIOS_PER_GROUP = io::adbms::REG_GROUP_SIZE / sizeof(int16_t);

constexpr float  THERM_LUT_STEP_V = 0.01f;
constexpr size_t THERM_LUT_SIZE   = 299;
constexpr float  THERM_VREF2_V    = 3.0f;
constexpr float  THERM_R_SERIES   = 10e3f;
// Thermistor datasheet (NTCS0603E3103JMT); worst case error of +- 2degC
constexpr float  THERM_R_NOMINAL  = 10e3f;
constexpr float  THERM_T_NOMINAL  = 298.15f;
constexpr float  THERM_BETA       = 3610.0f;
constexpr float  KELVIN_OFFSET    = 273.15f;


constexpr std::array<float, THERM_LUT_SIZE> THERM_LUT = []
{
    std::array<float, THERM_LUT_SIZE> lut{};
    for (size_t i = 0; i < THERM_LUT_SIZE; i++)
    {
        const float voltage    = static_cast<float>(i + 1) * THERM_LUT_STEP_V;
        // Voltage divider equation: 10k resistor (highside) and 10k thermistor (lowside)
        const float resistance = THERM_R_SERIES * voltage / (THERM_VREF2_V - voltage);
        // Beta equation: 1/T = 1/T0 + ln(R/R0)/B in Kelvin
        lut[i] = 1.0f / (1.0f / THERM_T_NOMINAL + std::log(resistance / THERM_R_NOMINAL) / THERM_BETA) - KELVIN_OFFSET;
    }
    return lut;
}();
} // namespace

namespace io::adbms::read
{
result<void> configReg(Segments<result<CFGA>> &config_a, Segments<result<CFGB>> &config_b)
{
    Segments<result<RegGroup>> config_regs;

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDCFGA, config_regs));
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        if (!config_regs[seg]) {
            config_a[seg] = std::unexpected(config_regs[seg].error());
        } else {
            config_a[seg] = std::bit_cast<CFGA>(*config_regs[seg]);
        }
    }

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDCFGB, config_regs));
     for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        if (!config_regs[seg]) {
            config_b[seg] = std::unexpected(config_regs[seg].error());
        } else {
            config_b[seg] = std::bit_cast<CFGB>(*config_regs[seg]);
        }
    }

    return {};
}

result<void> pwmReg(Segments<result<PWMA>> &pwm_a, Segments<result<PWMB>> &pwm_b)
{
    Segments<result<RegGroup>> pwm_regs;

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDPWMA, pwm_regs));
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        if (!pwm_regs[seg]) {
            pwm_a[seg] = std::unexpected(pwm_regs[seg].error());
        } else {
            pwm_a[seg] = std::bit_cast<PWMA>(*pwm_regs[seg]);
        }
    }

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDPWMB, pwm_regs));
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        if (!pwm_regs[seg]) {
            pwm_b[seg] = std::unexpected(pwm_regs[seg].error());
        } else {
            pwm_b[seg] = std::bit_cast<PWMB>(*pwm_regs[seg]);
        }
    }

    return {};
}

result<void> cAdc(const bool redundant, app::pack::VoltageBuffer &voltage_buffer)
{
    std::array<Segments<result<RegGroup>>, RDCV.size()> cell_regs;
    Segments<result<RegGroup>>                          stat_c_regs{};

    for (size_t group = 0; group < RDCV.size(); group++)
    {
        RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDCV[group], cell_regs[group]));
    }

    if (redundant)
    {
        RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATC, stat_c_regs));
    }

    app::pack::Reading max{ .value = std::numeric_limits<float>::lowest() };
    app::pack::Reading min{ .value = std::numeric_limits<float>::max() };

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            // Finds cell reg group (ie. RDCVA, RDCVB, etc.)
            const result<RegGroup> &reg = cell_regs[cell / CELLS_PER_GROUP][seg];

            if (!reg)
            {
                voltage_buffer.voltages[seg][cell] = std::unexpected(reg.error());
                continue;
            }

            // ADBMS stores data in little endian
            const int16_t raw = std::bit_cast<std::array<int16_t, CELLS_PER_GROUP>>(*reg)[cell % CELLS_PER_GROUP];

            if (raw == ADC_DEFAULT)
            {
                voltage_buffer.voltages[seg][cell] = std::unexpected(ErrorCode::INVALID_READING);
                continue;
            }

            // When enabled, compares CADCs with SADCs and stores flag when mismatch
            if (redundant)
            {
                const result<RegGroup> &stat_c = stat_c_regs[seg];
                if (!stat_c)
                {
                    voltage_buffer.voltages[seg][cell] = std::unexpected(stat_c.error());
                    continue;
                }
                if ((std::bit_cast<STATC>(*stat_c).csflt >> cell) & 1U)
                {
                    voltage_buffer.voltages[seg][cell] = std::unexpected(ErrorCode::MISMATCH);
                    continue;
                }
            }

            const float voltage = static_cast<float>(raw) * ADC_LSB_V + ADC_OFFSET_V;
            voltage_buffer.voltages[seg][cell] = voltage;

            if (voltage > max.value) max = { voltage, static_cast<uint8_t>(seg), static_cast<uint8_t>(cell) };
            if (voltage < min.value) min = { voltage, static_cast<uint8_t>(seg), static_cast<uint8_t>(cell) };
        }
    }

    if (max.value >= min.value)
    {
        voltage_buffer.max = max;
        voltage_buffer.min = min;
    }

    voltage_buffer.updated_ms = io::time::getCurrentMs();
    return {};
}

result<void> sAdc(PackCells<result<float>> &owc_voltages)
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
            // Finds cell reg group (ie. RDSVA, RDSVB, etc.)
            const result<RegGroup> &reg = regs[cell / CELLS_PER_GROUP][seg];

            if (!reg)
            {
                owc_voltages[seg][cell] = std::unexpected(reg.error());
                continue;
            }

            // ADBMS stores data in little endian
            const int16_t raw = std::bit_cast<std::array<int16_t, CELLS_PER_GROUP>>(*reg)[cell % CELLS_PER_GROUP];

            if (raw == ADC_DEFAULT)
            {
                owc_voltages[seg][cell] = std::unexpected(ErrorCode::INVALID_READING);
                continue;
            }

            owc_voltages[seg][cell] = static_cast<float>(raw) * ADC_LSB_V + ADC_OFFSET_V;
        }
    }
    return {};
}

result<void> xAdcTherm(const ThermistorMux mux, app::pack::TemperatureBuffer &temp_buffer, PackTherms<result<float>> &voltages)
{
    std::array<Segments<result<RegGroup>>, RDAUX.size()> gpio_regs;

    for (size_t group = 0; group < RDAUX.size(); group++)
    {
        RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDAUX[group], gpio_regs[group]));
    }
    
    const size_t first_therm = mux == ThermistorMux::THERM_0_7 ? 0 : THERM_GPIOS_PER_SEGMENT;

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t gpio = 0; gpio < THERM_GPIOS_PER_SEGMENT && first_therm + gpio < THERMISTORS_PER_SEGMENT; gpio++)
        {
            const size_t            therm = first_therm + gpio;
            const result<RegGroup> &reg   = gpio_regs[gpio / GPIOS_PER_GROUP][seg];

            if (!reg)
            {
                voltages[seg][therm]                 = std::unexpected(reg.error());
                temp_buffer.temperatures[seg][therm] = std::unexpected(reg.error());
                continue;
            }

            // ADBMS stores data in little endian
            const int16_t raw = std::bit_cast<std::array<int16_t, GPIOS_PER_GROUP>>(*reg)[gpio % GPIOS_PER_GROUP];

            if (raw == ADC_DEFAULT)
            {
                voltages[seg][therm]                 = std::unexpected(ErrorCode::INVALID_READING);
                temp_buffer.temperatures[seg][therm] = std::unexpected(ErrorCode::INVALID_READING);
                continue;
            }

            const float voltage  = static_cast<float>(raw) * ADC_LSB_V + ADC_OFFSET_V;
            voltages[seg][therm] = voltage;

            
            const long lut_index = std::lround(voltage / THERM_LUT_STEP_V) - 1;
            // LUT is used if voltage is between 0.01 to 2.99V (inclusive)
            if (lut_index < 0 || lut_index >= static_cast<long>(THERM_LUT_SIZE))
                temp_buffer.temperatures[seg][therm] = std::unexpected(ErrorCode::INVALID_READING);
            else
                temp_buffer.temperatures[seg][therm] = THERM_LUT[static_cast<size_t>(lut_index)];
        }
    }

    app::pack::Reading max{ .value = std::numeric_limits<float>::lowest() };
    app::pack::Reading min{ .value = std::numeric_limits<float>::max() };

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            if (!temp_buffer.temperatures[seg][therm])
                continue;

            const float temp = temp_buffer.temperatures[seg][therm].value();
            if (temp > max.value)
                max = { temp, static_cast<uint8_t>(seg), static_cast<uint8_t>(therm) };
            if (temp < min.value)
                min = { temp, static_cast<uint8_t>(seg), static_cast<uint8_t>(therm) };
        }
    }

    if (max.value >= min.value)
    {
        temp_buffer.max = max;
        temp_buffer.min = min;
    }

    temp_buffer.updated_ms = io::time::getCurrentMs();
    return {};
}

result<void> xAdcSupply(app::pack::SupplyBuffer &supply_buffer) {
    Segments<result<RegGroup>> stat_a_regs;
    Segments<result<RegGroup>> stat_b_regs;
    Segments<result<RegGroup>> aux_d_regs;

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATA, stat_a_regs));
    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATB, stat_b_regs));
    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDAUXD, aux_d_regs));

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        if (!stat_a_regs[seg])
        {
            supply_buffer.vref[seg] = std::unexpected(stat_a_regs[seg].error());
            supply_buffer.itmp[seg] = std::unexpected(stat_a_regs[seg].error());
        }
        else
        {
            const STATA stat_a      = std::bit_cast<STATA>(*stat_a_regs[seg]);
            supply_buffer.vref[seg] = static_cast<float>(stat_a.vref2) * ADC_LSB_V + ADC_OFFSET_V;
            supply_buffer.itmp[seg] = static_cast<float>(stat_a.itmp) * ITMP_LSB_C + ITMP_OFFSET_C;
        }

        if (!stat_b_regs[seg])
        {
            supply_buffer.vd[seg]   = std::unexpected(stat_b_regs[seg].error());
            supply_buffer.va[seg]   = std::unexpected(stat_b_regs[seg].error());
            supply_buffer.vres[seg] = std::unexpected(stat_b_regs[seg].error());
        }
        else
        {
            const STATB stat_b      = std::bit_cast<STATB>(*stat_b_regs[seg]);
            supply_buffer.vd[seg]   = static_cast<float>(stat_b.vd) * ADC_LSB_V + ADC_OFFSET_V;
            supply_buffer.va[seg]   = static_cast<float>(stat_b.va) * ADC_LSB_V + ADC_OFFSET_V;
            supply_buffer.vres[seg] = static_cast<float>(stat_b.vres) * ADC_LSB_V + ADC_OFFSET_V;
        }

        if (!aux_d_regs[seg])
        {
            supply_buffer.vmv[seg] = std::unexpected(aux_d_regs[seg].error());
            supply_buffer.vpv[seg] = std::unexpected(aux_d_regs[seg].error());
        }
        else
        {
            const AUXD aux_d       = std::bit_cast<AUXD>(*aux_d_regs[seg]);
            supply_buffer.vmv[seg] = static_cast<float>(aux_d.vmv) * ADC_LSB_V + ADC_OFFSET_V;
            supply_buffer.vpv[seg] = static_cast<float>(aux_d.vpv) * VPV_LSB_V + VPV_OFFSET_V;
        }
    }

    supply_buffer.updated_ms = io::time::getCurrentMs();
    return {};
}

result<void> flags(app::pack::DiagnosticBuffer &diag_buffer )
{
    Segments<result<RegGroup>> stat_c_regs;
    Segments<result<RegGroup>> stat_d_regs;

    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATC, stat_c_regs));
    RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATD, stat_d_regs));

    // Assume everything is not ok
    diag_buffer = {};

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        if (stat_d_regs[seg])
        {
            const STATD stat_d = std::bit_cast<STATD>(*stat_d_regs[seg]);
            for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            {
                diag_buffer.uv_ok[seg][cell] = !((stat_d.covuv >> (2 * cell)) & 1U);
                diag_buffer.ov_ok[seg][cell] = !((stat_d.covuv >> (2 * cell + 1)) & 1U);
            }
        }

        if (stat_c_regs[seg])
        {
            const STATC stat_c = std::bit_cast<STATC>(*stat_c_regs[seg]);
            diag_buffer.therm_shdn_ok[seg] = !stat_c.thsd;
            diag_buffer.self_test_ok[seg] =
                !(stat_c.ced || stat_c.cmed || stat_c.sed || stat_c.smed || stat_c.tmodchk || stat_c.oscchk);
            diag_buffer.supply_ok[seg] =
                !(stat_c.va_ov || stat_c.va_uv || stat_c.vd_ov || stat_c.vd_uv || stat_c.vde || stat_c.vdel);
        }
    }

    diag_buffer.updated_ms = io::time::getCurrentMs();
    return {};
}
} // namespace io::adbms::read