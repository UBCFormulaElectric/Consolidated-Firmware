#include "io_socStorage.hpp"
#include "io_log.hpp"

extern "C"
{
#include "io_fileSystem.h"
}

#include <cstdint>

namespace
{
constexpr const char *SOC_STORAGE_PATH = "/soc.txt";
constexpr uint8_t MAX_STORAGE_ERRORS   = 10U;

uint32_t soc_fd                      = 0U;
uint8_t  soc_storage_error_remaining = MAX_STORAGE_ERRORS;

bool storageEnabled()
{
	if (soc_storage_error_remaining == 0U)
	{
		LOG_ERROR("SoC storage disabled, not continuing");
		return false;
	}
	return true;
}

bool criticalCheck(const bool ok)
{
	if (!ok)
	{
		soc_storage_error_remaining = 0U;
		LOG_ERROR("Critical SoC storage failure, storage unavailable");
		return false;
	}
	return true;
}

bool operationCheck(const bool ok)
{
	if (!ok)
	{
		if (soc_storage_error_remaining > 0U)
		{
			--soc_storage_error_remaining;
		}
		LOG_ERROR("SoC storage operation failed");
		return false;
	}
	return true;
}
} // namespace

namespace io::socStorage
{

void init()
{
	if (!io_fileSystem_present())
	{
		soc_storage_error_remaining = 0U;
		LOG_ERROR("SoC storage unavailable: SD card not present");
		return;
	}

	if (!criticalCheck(io_fileSystem_init() == FILE_OK))
	{
		return;
	}
	if (!criticalCheck(io_fileSystem_open(SOC_STORAGE_PATH, &soc_fd) == FILE_OK))
	{
		return;
	}

	float last_soc_percent = 0.0f;
	if (io_fileSystem_read(soc_fd, &last_soc_percent, sizeof(last_soc_percent)) != FILE_OK)
	{
		constexpr float invalid_soc_percent = -1.0f;
		if (!criticalCheck(io_fileSystem_write(soc_fd, (void *)&invalid_soc_percent, sizeof(invalid_soc_percent)) == FILE_OK))
		{
			return;
		}
		(void)criticalCheck(io_fileSystem_sync(soc_fd) == FILE_OK);
	}
}

bool isAvailable()
{
	return io_fileSystem_present() && soc_storage_error_remaining > 0U;
}

bool read(float &saved_soc_percent)
{
	saved_soc_percent = -1.0f;
	if (!storageEnabled())
	{
		return false;
	}

	return operationCheck(io_fileSystem_read(soc_fd, &saved_soc_percent, sizeof(saved_soc_percent)) == FILE_OK);
}

bool write(const float soc_percent)
{
	if (!storageEnabled())
	{
		return false;
	}

	if (!operationCheck(io_fileSystem_write(soc_fd, (void *)&soc_percent, sizeof(soc_percent)) == FILE_OK))
	{
		return false;
	}

	return operationCheck(io_fileSystem_sync(soc_fd) == FILE_OK);
}

} // namespace io::socStorage
