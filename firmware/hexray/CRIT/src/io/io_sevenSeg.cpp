#include "io_sevenSeg.hpp"
#include "hw_pwmOutputs.hpp"
#include "hw_spis.hpp"
#include <algorithm>

namespace io::seven_seg
{
std::expected<void, ErrorCode> write(std::array<digit, DIGITS> &data)
{
    std::ranges::reverse(data);
    return seven_seg_device.transmit(std::span{ reinterpret_cast<uint8_t *>(data.data()), data.size() });
}

std::expected<void, ErrorCode> setBrightness(const float brightness)
{
    RETURN_IF_ERR_SILENT(seven_seg_dimming.start());
    RETURN_IF_ERR_SILENT(seven_seg_dimming.setDutyCycle(brightness));
    return {};
}
} // namespace io::seven_seg