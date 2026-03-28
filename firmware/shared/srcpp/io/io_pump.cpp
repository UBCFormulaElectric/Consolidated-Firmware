#include "io_pump.hpp"

namespace io
{
std::expected<void, ErrorCode> Pump::setPercentage(uint8_t percent) const
{
    // if (auto status = requirePot(); !status)
    // {
    //     return std::unexpected(status.error());
    // }
    const uint8_t hw_percent = logicalToHw(invert_, percent);
    return pot_.writePercentage(hw_percent);
}

std::expected<uint8_t, ErrorCode> Pump::getPercentage() const
{
    // if (auto status = requirePot(); !status)
    // {
    //     return std::unexpected(status.error());
    // }

    uint8_t hw_percent = 0u;
    if (auto status = pot_.readPercentage(hw_percent); !status)
    {
        return std::unexpected(status.error());
    }

    return hwToLogical(invert_, hw_percent);
}

std::expected<void, ErrorCode> Pump::enable(bool enable) const
{
    // if (auto status = requireEfuse(); !status)
    // {
    //     return std::unexpected(status.error());
    // }

    efuse_.setChannel(enable);
    return {};
}

std::expected<bool, ErrorCode> Pump::isEnabled() const
{
    // if (auto status = requireEfuse(); !status)
    // {
    //     return std::unexpected(status.error());
    // }

    return efuse_.isChannelEnabled();
}

std::expected<bool, ErrorCode> Pump::ok() const
{
    return efuse_.ok();
}

// std::expected<bool, ErrorCode> Pump::pgood() const
// {
//     // if (auto status = requireEfuse(); !status)
//     // {
//     //     return std::unexpected(status.error());
//     // }
//     return efuse_.pgood();
// }

std::expected<bool, ErrorCode> Pump::isReady() const
{
    auto healthy = ok();
    if (!healthy)
    {
        return std::unexpected(healthy.error());
    }
    auto enabled = isEnabled();
    if (!enabled)
    {
        return std::unexpected(enabled.error());
    }
    return *healthy && *enabled;
}

// std::expected<void, ErrorCode> Pump::requirePot() const
// {
//     if (!pot_)
//     {
//         return exitcodestd::unexpected(ErrorCode::UNIMPLEMENTED);
//     }
//    // return {};
// }

// std::expected<void, ErrorCode> Pump::requireEfuse() const
// {
//     if (!efuse_)
//     {
//         return std::unexpected(ErrorCode::UNIMPLEMENTED);
//     }
//     //return {};
// }
} // namespace io