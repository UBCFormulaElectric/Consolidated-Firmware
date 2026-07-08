#include "logfs.hpp"

#include <algorithm>
#include <cassert>

namespace LogFs
{

FileSystem::FileSystem(const BlockDevice::Cfg& cfg_in)
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