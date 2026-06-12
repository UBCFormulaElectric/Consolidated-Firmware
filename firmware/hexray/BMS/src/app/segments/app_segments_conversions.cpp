#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using io::adbms::ThermGpios;

namespace app::segments
{
using io::adbms::Cells, io::adbms::Segments;

Cells<result<float>> conversion::cellVoltage()
{
    Cells<result<float>>         out_voltage;
    const Cells<result<int16_t>> cell_voltage = io::adbms::read::cellVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!cell_voltage[seg][cell])
            {
                out_voltage[seg][cell] = std::unexpected(cell_voltage[seg][cell].error());
            }
            else
            {
                out_voltage[seg][cell] = convertRegToVoltage(cell_voltage[seg][cell].value());
            }
        }
    }
    return out_voltage;
}

ThermGpios<result<float>> conversion::thermVoltage()
{
    ThermGpios<result<float>>         out_therm;
    const ThermGpios<result<int16_t>> therm_voltage = io::adbms::read::thermGpioVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t gpio = 0; gpio < THERM_GPIOS_PER_SEGMENT; gpio++)
        {
            if (!therm_voltage[seg][gpio])
            {
                out_therm[seg][gpio] = std::unexpected(therm_voltage[seg][gpio].error());
            }
            else
            {
                out_therm[seg][gpio] = convertRegToVoltage(therm_voltage[seg][gpio].value());
            }
        }
    }
    return out_therm;
}

Segments<result<float>> conversion::segVoltage()
{
    const Segments<result<int16_t>> seg_voltage = io::adbms::read::segVoltage();

    Segments<result<float>> out_voltage;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        if (!seg_voltage[seg])
        {
            out_voltage[seg] = std::unexpected(seg_voltage[seg].error());
        }
        else
        {
            out_voltage[seg] = 25 * convertRegToVoltage(seg_voltage[seg].value());
        }
    }
    return out_voltage;
}

Cells<result<float>> conversion::cellOwcVoltages()
{
    Cells<result<float>>         out_cell_owc;
    const Cells<result<int16_t>> cell_owc_voltage = io::adbms::read::secondaryCellVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!cell_owc_voltage[seg][cell])
            {
                out_cell_owc[seg][cell] = std::unexpected(cell_owc_voltage[seg][cell].error());
            }
            else
            {
                out_cell_owc[seg][cell] = convertRegToVoltage(cell_owc_voltage[seg][cell].value());
            }
        }
    }
    return out_cell_owc;
}
} // namespace app::segments
