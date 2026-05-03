#include "hw_fmac.hpp"

namespace hw::fmac
{

std::expected<void, ErrorCode>
    FmacIir::init(std::span<const int16_t> coefB, std::span<const int16_t> coefA, uint8_t gainExponent)
{
    (void)coefB;
    (void)coefA;
    (void)gainExponent;
    return {};
}

// Pass-through: writes the input sample directly to *output. Tests that need a specific
// filtered value can swap in a mock or extend this fake later.
std::expected<void, ErrorCode> FmacIir::process(const float sample, float *output)
{
    if (output != nullptr)
        *output = sample;
    return {};
}

std::expected<void, ErrorCode> FmacIir::stop()
{
    return {};
}

} // namespace hw::fmac
