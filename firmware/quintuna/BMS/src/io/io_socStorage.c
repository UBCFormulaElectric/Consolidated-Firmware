#include "io_socStorage.h"
#include "io_fileSystem.h"
#include "io_log.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include <stdint.h>

#define SOC_STORAGE_PATH "/soc.txt"

#define CHECK_SOC_STORAGE_ERR_CRITICAL(f)                                \
    if (!(f))                                                            \
    {                                                                    \
        soc_storage_error_remaining = 0;                                 \
        LOG_ERROR("Critical SoC storage failure, storage unavailable"); \
        return;                                                          \
    }

#define CHECK_SOC_STORAGE_ENABLED_RET_FALSE()                \
    if (soc_storage_error_remaining == 0)                    \
    {                                                        \
        LOG_ERROR("SoC storage disabled, not continuing");  \
        return false;                                        \
    }

#define CHECK_SOC_STORAGE_ERR_RET_FALSE(f)                \
    if (!(f))                                             \
    {                                                     \
        soc_storage_error_remaining--;                    \
        LOG_ERROR("SoC storage operation failed");       \
        return false;                                     \
    }

static uint32_t soc_fd;
static uint8_t  soc_storage_error_remaining = 10;

void io_socStorage_init(void)
{
    if (!io_fileSystem_present())
    {
        soc_storage_error_remaining = 0;
        LOG_ERROR("SoC storage unavailable: SD card not present");
        return;
    }

    CHECK_SOC_STORAGE_ERR_CRITICAL(io_fileSystem_init() == FILE_OK);
    CHECK_SOC_STORAGE_ERR_CRITICAL(io_fileSystem_open(SOC_STORAGE_PATH, &soc_fd) == FILE_OK);

    float last_soc;
    if (io_fileSystem_read(soc_fd, &last_soc, sizeof(last_soc)) != FILE_OK)
    {
        const float invalid_soc = -1.0f;

        // Seed only a brand-new/empty file with an invalid sentinel.
        CHECK_SOC_STORAGE_ERR_CRITICAL(
            io_fileSystem_write(soc_fd, (void *)&invalid_soc, sizeof(invalid_soc)) == FILE_OK);
        CHECK_SOC_STORAGE_ERR_CRITICAL(io_fileSystem_sync(soc_fd) == FILE_OK);
    }
}

bool io_socStorage_isAvailable(void)
{
    return io_fileSystem_present() && soc_storage_error_remaining > 0;
}

bool io_socStorage_read(float *saved_soc_c)
{
    if (saved_soc_c == NULL)
    {
        return false;
    }

    *saved_soc_c = -1.0f;
    CHECK_SOC_STORAGE_ENABLED_RET_FALSE();

    CHECK_SOC_STORAGE_ERR_RET_FALSE(io_fileSystem_read(soc_fd, saved_soc_c, sizeof(*saved_soc_c)) == FILE_OK);
    return true;
}

bool io_socStorage_write(float soc)
{
    CHECK_SOC_STORAGE_ENABLED_RET_FALSE();

    CHECK_SOC_STORAGE_ERR_RET_FALSE(io_fileSystem_write(soc_fd, &soc, sizeof(soc)) == FILE_OK);
    CHECK_SOC_STORAGE_ERR_RET_FALSE(io_fileSystem_sync(soc_fd) == FILE_OK);
    return true;
}
