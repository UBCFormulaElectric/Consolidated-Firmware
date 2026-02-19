#include "io_powerGauge.hpp"
#include "hw_spis.hpp"
#include <algorithm>

namespace io::power_gauge
{
std::expected<void, ErrorCode> update(std::array<led_frame, LEDS> data)
{
    std::ranges::reverse(data);
    return pwr_chg_device.transmit(std::span{ reinterpret_cast<uint8_t *>(data.data()), data.size() });
}
} // namespace io::power_gauge
