#pragma once
#include <array>

extern "C"
{
#include "logfs.h"
}

namespace app::filesystem
{
struct FileSystemContext
{
    static constexpr std::size_t MAX_FILE_NUMBER = 3;

    std::array<LogFsFileCfg, MAX_FILE_NUMBER>                            files_cfg{};
    std::array<LogFsFile, MAX_FILE_NUMBER>                               files{};
    std::array<std::array<char, HW_DEVICE_SECTOR_SIZE>, MAX_FILE_NUMBER> files_cache{};
    std::array<bool, MAX_FILE_NUMBER>                                    files_opened{};

    LogFsFile                                  bootcount_file{};
    std::array<uint8_t, HW_DEVICE_SECTOR_SIZE> bootcount_cache{};
    LogFsFileCfg                               bootcount_cfg = {
                                      .path  = "/bootcount.txt",
                                      .cache = bootcount_cache.data(),
    };
};

} // namespace app::filesystem