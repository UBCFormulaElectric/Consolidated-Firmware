#include "app_bootcount.hpp"

namespace app::bootcount
{
std::expected<uint32_t, io::FileSystemError> BootCounter::init(io::FileSystem &fs)
{
    if (bootcount_inited == true)
        return std::unexpected(io::FileSystemError::ERROR_BAD_ARG);

    auto result = fs.open("/bootcount.txt");
    if (!result)
        return std::unexpected(result.error());

    bootcount_fd     = *result;
    bootcount_inited = true;

    return bootcount_fd;
}

std::expected<uint32_t, io::FileSystemError> BootCounter::read(io::FileSystem &fs) const
{
    if (!bootcount_inited)
        return std::unexpected(io::FileSystemError::ERROR_BAD_ARG);

    uint32_t value    = 0;
    uint32_t num_read = 0;

    auto err = fs.readMetadata(
        bootcount_fd, std::span(reinterpret_cast<uint8_t *>(&value), sizeof(value)), sizeof(value), num_read);
    if (!err)
        return std::unexpected(err.error());

    if (num_read != sizeof(value))
        value = 0;

    return value;
}

std::expected<uint32_t, io::FileSystemError> BootCounter::increment(io::FileSystem &fs) const
{
    if (!bootcount_inited)
        return std::unexpected(io::FileSystemError::ERROR_BAD_ARG);

    auto current = read(fs);
    if (!current)
        return std::unexpected(current.error());

    uint32_t next = *current + 1;

    auto err =
        fs.writeMetadata(bootcount_fd, std::span(reinterpret_cast<uint8_t *>(&next), sizeof(next)), sizeof(next));
    if (!err)
        return std::unexpected(err.error());

    return next;
}

} // namespace app::bootcount