#include "app_socStorage.hpp"
#include "io_log.hpp"
#include "io_filesystem.hpp"
#include "io_filesystems.hpp"

#include <cstdint>
#include <expected>
#include <optional>

namespace
{
constexpr const char *SOC_STORAGE_PATH       = "/soc.txt";
constexpr uint32_t    SOC_TENTHS_UNAVAILABLE = UINT32_MAX;

bool                    soc_storage_ready     = false;
std::optional<uint32_t> soc_fd                = std::nullopt;
uint32_t                soc_last_saved_tenths = SOC_TENTHS_UNAVAILABLE;

constexpr uint32_t socPercentToTenths(const float soc_percent)
{
    return static_cast<uint32_t>(soc_percent * 10.0f + 0.5f);
}

constexpr bool isValidSocPercent(const float soc_percent)
{
    return soc_percent > 0.0f && soc_percent < 100.0f;
}

constexpr bool isValidSocTenths(const uint32_t soc_tenths)
{
    return soc_tenths > 0U && soc_tenths < 1000U;
}

std::expected<void, ErrorCode> read(float &saved_soc_percent)
{
    saved_soc_percent = -1.0f;

    auto buf = std::span<uint8_t>(reinterpret_cast<uint8_t *>(&saved_soc_percent), sizeof(saved_soc_percent));
    if (auto err = fs.read(soc_fd.value(), buf, sizeof(saved_soc_percent)); !err)
    {
        return std::unexpected(ErrorCode::READ_FAILED);
    }
    return {};
}

std::expected<void, ErrorCode> write(const float soc_percent)
{
    auto buf = std::span<uint8_t>(reinterpret_cast<uint8_t *>(const_cast<float *>(&soc_percent)), sizeof(soc_percent));
    if (auto err = fs.write(soc_fd.value(), buf, sizeof(soc_percent)); !err)
    {
        return std::unexpected(ErrorCode::WRITE_FAILED);
    }

    if (auto err = fs.sync(soc_fd.value()); !err)
    {
        return std::unexpected(ErrorCode::WRITE_FAILED);
    }
    return {};
}
} // namespace

namespace app::socStorage
{

void init()
{
    soc_storage_ready = false;

    if (auto open_result = fs.open(SOC_STORAGE_PATH))
    {
        soc_fd = *open_result;
    }
    else
    {
        soc_fd = std::nullopt;
        LOG_ERROR("SoC storage unavailable: failed to open file");
        return;
    }

    if (const auto result = readSocFromSd(); !result || !isValidSocPercent(result.value()))
    {
        LOG_WARN("SoC storage warning: failed to read valid SoC, attempting to reset.");
        constexpr float invalid_soc_percent = -1.0f;
        if (const auto write_result = writeSocToSd(invalid_soc_percent); !write_result)
        {
            LOG_ERROR("SoC storage error: failed to reset invalid SoC");
            return;
        }
    }

    soc_storage_ready = true;
}

bool isAvailable()
{
    return soc_storage_ready;
}

std::expected<float, ErrorCode> readSocFromSd()
{
    float saved_soc_percent = -1.0f;

    RETURN_IF_ERR(read(saved_soc_percent));

    if (isValidSocPercent(saved_soc_percent))
    {
        soc_last_saved_tenths = socPercentToTenths(saved_soc_percent);
    }

    return saved_soc_percent;
}

std::expected<void, ErrorCode> writeSocToSd(const float soc_percent)
{
    if (!isValidSocPercent(soc_percent))
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }

    RETURN_IF_ERR(write(soc_percent));
    soc_last_saved_tenths = socPercentToTenths(soc_percent);

    return {};
}

uint32_t getLastWrittenSocTenths()
{
    if (isValidSocTenths(soc_last_saved_tenths))
    {
        return soc_last_saved_tenths;
    }

    return SOC_TENTHS_UNAVAILABLE;
}

std::expected<uint32_t, ErrorCode> convertSocToTenths(const float soc_percent)
{
    if (!isValidSocPercent(soc_percent))
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }

    const uint32_t soc_tenths = socPercentToTenths(soc_percent);
    return soc_tenths;
}

} // namespace app::socStorage
