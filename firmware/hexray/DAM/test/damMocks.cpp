#include <unordered_map>
#include <fstream>

#include "io_canQueues.hpp"
io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

#include "io_fileSystems.hpp"
io::FileSystem fs{};

std::expected<uint32_t, io::FileSystem::FileSystemError> io::FileSystem::open(const char *path)
{
    if (open_file_names.contains(path))
    {
        return std::unexpected(FileSystemError::ERROR_BAD_ARG);
    }

    for (size_t i = 0;; i++)
    {
        if (i >= MAX_FILE_NUMBER)
        {
            return std::unexpected(FileSystemError::NO_SPACE);
        }
        if (!files_opened[i])
        {
            // found a file descriptor that is not opened, open file and return fd
            files_opened[i]     = true;
            auto [it, inserted] = files.emplace(i, std::fstream{ path });
            if (!inserted)
            {
                return std::unexpected(FileSystemError::ERROR);
            }
            if (!it->second.is_open())
            {
                return std::unexpected(FileSystemError::ERROR);
            }
            open_file_names.emplace(path);
            return i;
        }
    }
}
std::expected<void, io::FileSystem::FileSystemError>
    io::FileSystem::readMetadata(uint32_t fd, std::span<uint8_t> buf, uint32_t &num_read)
{
    // get file with fd
    const auto it = std::ranges::find_if(files, [fd](const auto &pair) { return pair.first == fd; });
    if (it == files.end())
    {
        return std::unexpected(FileSystemError::NOT_FOUND);
    }
    auto &file = it->second;
    if (!file.is_open())
    {
        return std::unexpected(FileSystemError::ERROR);
    }
    // read file into buf as much as possible
    file.read(reinterpret_cast<char *>(buf.data()), static_cast<std::streamsize>(buf.size()));
    num_read = static_cast<uint32_t>(file.gcount());
    return {};
}
std::expected<void, io::FileSystem::FileSystemError>
    io::FileSystem::writeMetadata(uint32_t fd, const std::span<const uint8_t> buf)
{
    // get file with fd
    const auto it = std::ranges::find_if(files, [fd](const auto &pair) { return pair.first == fd; });
    if (it == files.end())
    {
        return std::unexpected(FileSystemError::NOT_FOUND);
    }
    auto &file = it->second;
    if (!file.is_open())
    {
        return std::unexpected(FileSystemError::ERROR);
    }
    // write buf into file
    file.write(reinterpret_cast<const char *>(buf.data()), static_cast<std::streamsize>(buf.size()));
    return {};
}