#include "app_socStorage.hpp"
#include "io_log.hpp"
#include "hw_sds.hpp"
#include "io_filesystem.hpp"

#include <cstdint>
#include <expected>

namespace
{
constexpr const char *SOC_STORAGE_PATH       = "/soc.txt";
constexpr uint32_t    SOC_TENTHS_UNAVAILABLE = UINT32_MAX;

bool soc_storage_ready          = false;
std::optional<uint32_t> soc_fd = std::nullopt;
uint32_t soc_last_saved_tenths = SOC_TENTHS_UNAVAILABLE;

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

std::expected<void, FileSystemError> recordStatus(const FileSystemError status)
{
    soc_storage_status = status;
    if (status != FILE_OK)
    {
        LOG_ERROR("File system error: %d", status);
        return std::unexpected(status);
    }

    return {};
}

bool requireStorageReady()
{
    return soc_storage_ready;
}

std::expected<void, FileSystemError> read(float &saved_soc_percent)
{
    saved_soc_percent = -1.0f;

    return RETURN_IF_ERR(io::FileSystem::read(soc_fd, &saved_soc_percent, sizeof(saved_soc_percent)));
}

std::expected<void, FileSystemError> write(const float soc_percent)
{

    RETURN_IF_ERR(io::FileSystem::write(soc_fd, &soc_percent, sizeof(soc_percent)))

    return RETURN_IF_ERR(io::FileSystem::sync(soc_fd));
}
} // namespace

namespace app::socStorage
{

void init()
{
    soc_storage_ready = false;

    if (!io::FileSystem::checkMount())
    {
        LOG_ERROR("SoC storage unavailable: failed to initialize filesystem");
        return;
    }
    if (const auto result = io::FileSystem::open(SOC_STORAGE_PATH, &soc_fd); !result)
    {
        LOG_ERROR("SoC storage unavailable: failed to open file");
        return;
    }

    float last_soc_percent = 0.0f;
    if (auto result = recordStatus(io_fileSystem_read(soc_fd, &last_soc_percent, sizeof(last_soc_percent))); !result)
    {
        constexpr float invalid_soc_percent = -1.0f;
        if (auto write_result =
                recordStatus(io_fileSystem_write(soc_fd, &invalid_soc_percent, sizeof(invalid_soc_percent)));
            !write_result)
        {
            LOG_ERROR("SoC storage unavailable: failed to write default value");
            return;
        }
        if (auto sync_result = recordStatus(io_fileSystem_sync(soc_fd)); !sync_result)
        {
            LOG_ERROR("SoC storage unavailable: failed to sync file");
            return;
        }
    }

    soc_storage_ready = true;
}

bool isAvailable()
{
    // TODO replace once cpp version of filesystem is implemented
    return sd1.sdPresent() && soc_storage_ready;
}

std::expected<float, ErrorCode> readSocFromSd()
{
    float saved_soc_percent = -1.0f;

    const auto read_result = read(saved_soc_percent);
    if (!read_result)
    {
        return std::unexpected(ErrorCode::ERROR_SOC_STORAGE_READ_FAILED);
    }

    if (isValidSocPercent(saved_soc_percent))
    {
        soc_last_saved_tenths = socPercentToTenths(saved_soc_percent);
    }

    return saved_soc_percent;
}

std::expected<void, ErrorCode> writeSocToSd(const float soc_percent)
{
    const auto write_result = write(soc_percent);
    if (!write_result)
    {
        return std::unexpected(ErrorCode::ERROR_SOC_STORAGE_WRITE_FAILED);
    }

    if (isValidSocPercent(soc_percent))
    {
        soc_last_saved_tenths = socPercentToTenths(soc_percent);
    }

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

    soc_tenths = socPercentToTenths(soc_percent);
    return soc_tenths;
}

} // namespace app::socStorage
