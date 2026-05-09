#include "io_canQueues.hpp"

io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

#include "io_filesystem.hpp"
std::expected<uint32_t, io::FileSystem::FileSystemError> io::FileSystem::open(const char *path)
{
    return {};
}
std::expected<void, io::FileSystem::FileSystemError>
    io::FileSystem::readMetadata(uint32_t fd, std::span<uint8_t> buf, uint32_t &num_read)
{
    return {};
}
std::expected<void, io::FileSystem::FileSystemError>
    io::FileSystem::writeMetadata(uint32_t fd, std::span<const uint8_t> buf)
{
    return {};
}