#include "app_socStorage.hpp"
#include "io_log.hpp"
#include "hw_sds.hpp"

extern "C"
{
#include "io_fileSystem.h"
}

#include <cstdint>
#include <expected>

namespace
{
constexpr const char *SOC_STORAGE_PATH       = "/soc.txt";
constexpr uint32_t    SOC_TENTHS_UNAVAILABLE = UINT32_MAX;

FileSystemError soc_storage_status    = FILE_MOUNT_FAILED;
bool            soc_storage_ready     = false;
uint32_t        soc_fd                = 0U;
uint32_t        soc_last_saved_tenths = SOC_TENTHS_UNAVAILABLE;

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

std::expected<void, FileSystemError> requireStorageReady()
{
    if (!soc_storage_ready)
    {
        return std::unexpected(soc_storage_status);
    }

    return {};
}

std::expected<void, FileSystemError> read(float &saved_soc_percent)
{
    saved_soc_percent = -1.0f;

    if (auto result = requireStorageReady(); !result)
    {
        return result;
    }

    return recordStatus(io_fileSystem_read(soc_fd, &saved_soc_percent, sizeof(saved_soc_percent)));
}

std::expected<void, FileSystemError> write(const float soc_percent)
{
    if (auto result = requireStorageReady(); !result)
    {
        return result;
    }

    if (auto result = recordStatus(io_fileSystem_write(soc_fd, &soc_percent, sizeof(soc_percent))); !result)
    {
        return result;
    }

    return recordStatus(io_fileSystem_sync(soc_fd));
}
} // namespace

namespace app::socStorage
{

void init()
{
    soc_storage_ready = false;

    // TODO: Replace once cpp version of filesystem is implemented
    if (!sd1.sdPresent())
    {
        (void)recordStatus(FILE_MOUNT_FAILED);
        LOG_ERROR("SoC storage unavailable: SD card not present");
        return;
    }

    if (auto result = recordStatus(io_fileSystem_init()); !result)
    {
        LOG_ERROR("SoC storage unavailable: failed to initialize file system");
        return;
    }
    if (auto result = recordStatus(io_fileSystem_open(SOC_STORAGE_PATH, &soc_fd)); !result)
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

FileSystemError getStatus()
{
    return soc_storage_status;
}

bool readSocFromSd(float &saved_soc_percent)
{
    soc_last_saved_tenths = SOC_TENTHS_UNAVAILABLE;

    const auto read_result = read(saved_soc_percent);
    if (!read_result)
    {
        return false;
    }

    if (isValidSocPercent(saved_soc_percent))
    {
        soc_last_saved_tenths = socPercentToTenths(saved_soc_percent);
    }

    return true;
}

bool writeSocToSd(const float soc_percent)
{
    const auto write_result = write(soc_percent);
    if (!write_result)
    {
        return false;
    }

    if (isValidSocPercent(soc_percent))
    {
        soc_last_saved_tenths = socPercentToTenths(soc_percent);
    }

    return true;
}

uint32_t getLastWrittenSocTenths()
{
    if (isValidSocTenths(soc_last_saved_tenths))
    {
        return soc_last_saved_tenths;
    }

    return SOC_TENTHS_UNAVAILABLE;
}

bool convertSocToTenths(const float soc_percent, uint32_t &soc_tenths)
{
    if (!isValidSocPercent(soc_percent))
    {
        return false;
    }

    soc_tenths = socPercentToTenths(soc_percent);
    return true;
}

} // namespace app::socStorage
