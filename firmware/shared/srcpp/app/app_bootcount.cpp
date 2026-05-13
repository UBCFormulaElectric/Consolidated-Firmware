#include "app_bootcount.hpp"

namespace app::bootcount
{
std::expected<void, io::FileSystem::FileSystemError> update(io::FileSystem &fs)
{
    auto result = fs.open("/bootcount.txt");
    if (!result)
        return std::unexpected(result.error());
    const uint32_t bootcount_fd = result.value();

    uint32_t               num_read;
    std::array<uint8_t, 4> bootcount_buf{};

    if (const auto err = fs.readMetadata(bootcount_fd, bootcount_buf, num_read); !err)
        return std::unexpected(err.error());

    // in particular read 0 bytes, which means the file is empty and we should initialize it to 1
    const uint32_t current_bootcount =
        num_read == sizeof(uint32_t) ? *reinterpret_cast<uint32_t *>(bootcount_buf.data()) : 0;

    // populate the next_bootcount_buf
    *reinterpret_cast<uint32_t *>(bootcount_buf.data()) = current_bootcount + 1;

    if (const auto err = fs.writeMetadata(bootcount_fd, bootcount_buf); !err)
        return std::unexpected(err.error());

    return {};
}
} // namespace app::bootcount