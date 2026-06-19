#include "io_sevenSeg.hpp"
#include "hw_pwmOutputs.hpp"
#include "hw_spis.hpp"
#include <algorithm>
#include "hw_gpios.hpp"

namespace io::seven_seg
{
result<void> write(const std::span<char, DIGITS> data)
{
    std::ranges::reverse(data);

    std::array<digit, DIGITS> ssdata{};
    for (size_t ii = 0; ii < DIGITS; ii++)
    {
        ssdata[ii] = char_to_segment(data[ii]);
    }
    RETURN_IF_ERR_SILENT(
        seven_seg_device.transmit(std::span{ reinterpret_cast<uint8_t *>(ssdata.data()), ssdata.size() }));
    seven_seg_rck.writePin(true);
    seven_seg_rck.writePin(false);
    return {};
}

result<void> setBrightness(const float brightness)
{
    RETURN_IF_ERR_SILENT(seven_seg_dimming.start());
    RETURN_IF_ERR_SILENT(seven_seg_dimming.setDutyCycle(100 - brightness));
    return {};
}
} // namespace io::seven_seg