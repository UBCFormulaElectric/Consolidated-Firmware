#include "logfs.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>

namespace LogFs
{

[[maybe_unused]] static void initFile(File& file, const FileCfg& cfg, const OpenFlags flags)
{
    file.cache.cached_addr = INVALID_BLOCK;
    file.cache.buf         = cfg.cache;
    file.cache_data        = reinterpret_cast<BlockData*>(cfg.cache.data());
    file.is_open           = false;
    file.flags             = static_cast<uint32_t>(flags);

    const auto path_len = std::min(cfg.path.size(), static_cast<size_t>(PATH_BYTES - 1));
    std::memcpy(file.path, cfg.path.data(), path_len);
    file.path[path_len] = '\0';
}

// File System 
LogFs::FileSystem::FileSystem(const BlockDevice::Cfg& cfg_in)
    : cfg(&cfg_in)
{
    assert(cfg != nullptr);
    assert(cfg->block_size > sizeof(BlockFile));
    assert(cfg->block_size > sizeof(BlockMetadata));
    assert(cfg->block_size > sizeof(BlockData));

    eff_block_size_bytes = cfg->block_size - (sizeof(BlockData) - 1);
    max_path_len_bytes = std::min(
        static_cast<int>(cfg->block_size - sizeof(BlockFile)),
        static_cast<int>(PATH_BYTES)
    );
}

ErrorCode FileSystem::mount()
{
    return ErrorCode::NOT_OPEN; //bs for now 
}

ErrorCode FileSystem::format()
{
    return ErrorCode::NOT_OPEN;
}

ErrorCode FileSystem::open(File&, const FileCfg&, uint32_t)
{
    return ErrorCode::NOT_OPEN;
}

ErrorCode FileSystem::close(File&)
{
    return ErrorCode::NOT_OPEN;
}

} // namespace LogFs
