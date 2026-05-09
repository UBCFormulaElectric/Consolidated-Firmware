#include <iostream>

#include "io_canQueues.hpp"
io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

#include "io_fileSystems.hpp"
#include <algorithm>
io::FileSystem fs{};

std::expected<uint32_t, io::FileSystem::FileSystemError> io::FileSystem::open(const char *path)
{
    // add dot before path
    if (path[0] != '/')
    {
        return std::unexpected(FileSystemError::ERROR_BAD_ARG);
    }
    std::string path_str(path);
    if (path_str.empty())
    {
        return std::unexpected(FileSystemError::ERROR_BAD_ARG);
    }
    path_str = "." + path_str;

    std::cout << path_str << std::endl;

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
                std::cout << "failure here" << std::endl;
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
    std::cout << "writing " << buf.size() << " bytes to file" << std::endl;
    file.write(reinterpret_cast<const char *>(buf.data()), static_cast<std::streamsize>(buf.size()));
    const std::string s = "penis penis penis";
    file.write(s.data(), static_cast<std::streamsize>(s.size()));
    file.flush();
    return {};
}