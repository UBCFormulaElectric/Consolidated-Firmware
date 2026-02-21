#include "io_powerGauge.hpp"
#include "hw_spis.hpp"
#include <algorithm>

namespace io::power_gauge
{
std::expected<void, ErrorCode> update(std::array<led_frame, LEDS> data)
{
    std::ranges::reverse(data);
    static constexpr std::array<uint8_t, 4> sof{ { 0, 0, 0, 0 } };
    static constexpr std::array<uint8_t, 4> eof{ { 0xff, 0xff, 0xff, 0xff } };
    RETURN_IF_ERR_SILENT(pwr_chg_device.transmit(sof))
    RETURN_IF_ERR_SILENT(pwr_chg_device.transmit(std::span{ reinterpret_cast<uint8_t *>(data.data()), data.size() }));
    RETURN_IF_ERR_SILENT(pwr_chg_device.transmit(eof))
    return {};
}
} // namespace io::power_gauge
