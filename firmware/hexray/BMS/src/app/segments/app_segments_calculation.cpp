#include <cmath>

#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "app_thermistors.hpp"

namespace app::segments::calculate
{
Cells<result<bool>> cellOwc(
    const std::array<Cells<result<float>>, static_cast<size_t>(io::adbms::OpenWireSwitch::CHANNEL_COUNT)> &owc_voltages)
{
    constexpr size_t NUM_C_PINS = CELLS_PER_SEGMENT + 1U;

    Cells<result<bool>> owc_cell;

    const Cells<result<float>> &cell_odd_voltage =
        owc_voltages[static_cast<size_t>(io::adbms::OpenWireSwitch::ODD_CHANNELS)];
    const Cells<result<float>> &cell_even_voltage =
        owc_voltages[static_cast<size_t>(io::adbms::OpenWireSwitch::EVEN_CHANNELS)];

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            owc_cell[seg][cell] = true;

        std::array<float, CELLS_PER_SEGMENT> cellpu{};
        std::array<float, CELLS_PER_SEGMENT> cellpd{};
        std::array<bool, CELLS_PER_SEGMENT>  delta_valid{};

        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!cell_even_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(cell_even_voltage[seg][cell].error());
                continue;
            }
            if (!cell_odd_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(cell_odd_voltage[seg][cell].error());
                continue;
            }

            if (cell % 2 == 0)
            {
                cellpu[cell] = cell_even_voltage[seg][cell].value();
                cellpd[cell] = cell_odd_voltage[seg][cell].value();
            }
            else
            {
                cellpu[cell] = cell_odd_voltage[seg][cell].value();
                cellpd[cell] = cell_even_voltage[seg][cell].value();
            }

            delta_valid[cell] = true;
        }

        std::array<float, CELLS_PER_SEGMENT> cell_delta{};
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            cell_delta[cell] = cellpu[cell] - cellpd[cell];
        }

        std::array<bool, NUM_C_PINS> cpin_open{};
        size_t                       n = 0;
        while (n + 1U < CELLS_PER_SEGMENT)
        {
            if (delta_valid[n] && delta_valid[n + 1U] && cell_delta[n] > OW_CELL_DELTA_THRESHOLD)
            {
                cpin_open[n] = true;
                while (n + 1U < CELLS_PER_SEGMENT && delta_valid[n] && delta_valid[n + 1U])
                {
                    if (const float diff = cell_delta[n] - cell_delta[n + 1U]; diff <= -OW_CELL_DELTA_THRESHOLD)
                    {
                        cpin_open[n + 1U] = true;
                        n++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            n++;
        }

        // Endpoint check: a raw ADC of 0 (= 1.5V, the floor of convertRegToVoltage) flags the outermost C-pin as open.
        if (cell_even_voltage[seg][0] && cell_even_voltage[seg][0].value() <= convertRegToVoltage(0U))
        {
            cpin_open[0] = true;
        }
        if (cell_odd_voltage[seg][CELLS_PER_SEGMENT - 1U] &&
            cell_odd_voltage[seg][CELLS_PER_SEGMENT - 1U].value() <= convertRegToVoltage(0U))
        {
            cpin_open[CELLS_PER_SEGMENT] = true;
        }

        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (owc_cell[seg][cell] && (cpin_open[cell] || cpin_open[cell + 1U]))
            {
                owc_cell[seg][cell] = false;
            }
        }
    }
    return owc_cell;
}

Therms<result<float>>
    thermTemps(const std::array<ThermGpios<result<float>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
                   &therm_voltages)
{
    Therms<result<float>> out;

    const ThermGpios<result<float>> &therm_0_7 = therm_voltages[static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_0_7)];
    const ThermGpios<result<float>> &therm_8_13 =
        therm_voltages[static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_8_13)];

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            const size_t                     gpio    = therm % THERM_GPIOS_PER_SEGMENT;
            const ThermGpios<result<float>> &source  = (therm < THERM_GPIOS_PER_SEGMENT) ? therm_0_7 : therm_8_13;
            const result<float>             &reading = source[seg][gpio];

            if (!reading)
            {
                out[seg][therm] = std::unexpected(reading.error());
                continue;
            }

            const float voltage    = reading.value();
            const float resistance = R_SERIES * (voltage / (V_REF2 - voltage));
            out[seg][therm]        = app::therm::adbms_ntc10k_lut.resistanceToTemp(resistance);
        }
    }
    return out;
}

Therms<result<bool>>
    thermOwc(const std::array<ThermGpios<result<float>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
                 &therm_voltages)
{
    Therms<result<bool>> out;

    const ThermGpios<result<float>> &therm_0_7 = therm_voltages[static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_0_7)];
    const ThermGpios<result<float>> &therm_8_13 =
        therm_voltages[static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_8_13)];

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            const size_t                     gpio    = therm % THERM_GPIOS_PER_SEGMENT;
            const ThermGpios<result<float>> &source  = (therm < THERM_GPIOS_PER_SEGMENT) ? therm_0_7 : therm_8_13;
            const result<float>             &reading = source[seg][gpio];

            if (!reading)
            {
                out[seg][therm] = std::unexpected(reading.error());
                continue;
            }

            // TODO: calibrate OW_THERM_THRESHOLD (currently 2.8V).
            out[seg][therm] = reading.value() > OW_THERM_THRESHOLD;
        }
    }
    return out;
}
} // namespace app::segments::calculate
