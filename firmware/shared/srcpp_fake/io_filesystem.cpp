#include "io_filesystem.hpp"

#include <algorithm>
#include <fstream>
#include <string>

// Host-side fake of io::FileSystem: each file descriptor maps to a std::fstream in the working directory.

std::expected<void, io::FileSystem::FileSystemError> io::FileSystem::init()
{
    return {};
}

std::expected<uint32_t, io::FileSystem::FileSystemError> io::FileSystem::open(const char *path)
{
    if (path == nullptr || path[0] != '/')
    {
        return std::unexpected(FileSystemError::ERROR_BAD_ARG);
    }
    // add dot before path so files land in the working directory
    const std::string path_str = "." + std::string(path);

    if (open_file_names.contains(path_str))
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
            files_opened[i] = true;
            // create file if it doesn't exist
            auto [it, inserted] =
                files.emplace(i, std::fstream{ path_str, std::ios::in | std::ios::out | std::ios::app });
            if (!inserted)
            {
                return std::unexpected(FileSystemError::ERROR);
            }
            if (!it->second.is_open())
            {
                return std::unexpected(FileSystemError::ERROR);
            }
            open_file_names.emplace(path_str);
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
    file.flush();
    return {};
}

std::expected<void, io::FileSystem::FileSystemError>
    io::FileSystem::write(uint32_t fd, std::span<uint8_t> buf, std::size_t size)
{
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
    const auto n = std::min(size, buf.size());
    file.write(reinterpret_cast<const char *>(buf.data()), static_cast<std::streamsize>(n));
    return {};
}

std::expected<void, io::FileSystem::FileSystemError> io::FileSystem::sync(uint32_t fd)
{
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
    file.flush();
    return {};
}
