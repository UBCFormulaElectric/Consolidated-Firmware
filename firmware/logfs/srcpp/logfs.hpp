#pragma once

#include <cassert>
#include <cstdint>
#include <span>
#include <string_view>

namespace LogFs
{

inline constexpr uint16_t ORIGIN = 0;
inline constexpr uint32_t INVALID_BLOCK = 0xFFFFFFFFu;
inline constexpr uint16_t PATH_BYTES = 128;
inline constexpr uint16_t PAIR_SIZE = 2;

enum class ErrorCode : int16_t
{
    OK            = 0,
    IO            = -2,
    CORRUPT       = -3,
    INVALID_ARG   = -4,
    INVALID_PATH  = -5,
    UNMOUNTED     = -6,
    NOMEM         = -7,
    NOT_OPEN      = -8,
    RD_ONLY       = -9,
    WR_ONLY       = -10,
    DNE           = -11,
    NO_MORE_FILES = -12,
};

enum class OpenFlags : uint8_t
{
    RD_ONLY = 0x01,
    WR_ONLY = 0x02,
    RD_WR   = 0x03,
    CREATE  = 0x10,
};

enum class ReadFlags : uint8_t
{
    END,
    ITER,
};

struct PairHeader
{
    uint32_t crc = 0;
    uint8_t seq_num = 0;
    uint32_t write_cycles = 0;
    uint32_t replacement_addr = INVALID_BLOCK;
};

struct BlockFile
{
    PairHeader pair_hdr{};
    uint32_t next_file_addr = INVALID_BLOCK;
    uint32_t metadata_addr = INVALID_BLOCK;
    uint32_t head_data_addr = INVALID_BLOCK;
    uint32_t prev_head_addr = INVALID_BLOCK;
    uint32_t num_data_blocks = 0;
    char path[1] = {};
};

struct BlockMetadata
{
    PairHeader pair_hdr{};
    uint32_t num_bytes = 0;
    uint8_t data = 0;
};

struct BlockData
{
    uint32_t crc = 0;
    uint32_t prev_data_addr = INVALID_BLOCK;
    uint32_t num_bytes = 0;
    uint8_t data = 0;
};

struct Pair
{
    uint32_t addrs[PAIR_SIZE];
    uint8_t seq_num = 0;
    bool seq_num_on_disk = false;
};

struct Cache
{
    uint32_t cached_addr = INVALID_BLOCK;
    std::span<std::byte> buf{};
};

struct FileCfg
{
    std::string_view path{};
    std::span<std::byte> cache{};
};

class BlockDevice
{
public:
    struct Cfg
    {
        uint32_t block_size = 0;
        uint32_t block_count = 0;
        BlockDevice* device = nullptr;
        std::span<std::byte> cache{};
        uint32_t write_cycles = 0;
        bool rd_only = false;
    };

    virtual ~BlockDevice() = default;
    virtual ErrorCode read(uint32_t block, std::span<std::byte> buf) = 0;
    virtual ErrorCode write(uint32_t block, std::span<const std::byte> buf) = 0;
};

class File
{
public:
    File() = default;

    Cache cache{};
    BlockData* cache_data = nullptr;
    char path[PATH_BYTES] = {};
    uint32_t flags = 0;

    bool is_open = false;
    Pair file_pair{};
    Pair metadata_pair{};
    uint32_t head_data_addr = INVALID_BLOCK;

    uint32_t read_iter_init = 0;
    uint32_t read_iter_data_byte = 0;
    uint32_t read_iter_data_addr = INVALID_BLOCK;
};

class FileSystem
{
public:
    explicit FileSystem(const BlockDevice::Cfg& cfg);

    ErrorCode mount();
    ErrorCode format();
    ErrorCode open(File& file, const FileCfg& cfg, uint32_t flags);
    ErrorCode close(File& file);

private:
    const BlockDevice::Cfg* cfg = nullptr;
    File root_file{};
    uint32_t head_file_addr = INVALID_BLOCK;
    uint32_t head_addr = 0;
    uint32_t eff_block_size_bytes = 0;
    int max_path_len_bytes = 0;
    bool mounted = false;
    bool out_of_memory = false;
    PairHeader* cache_pair_hdr = nullptr;
    BlockFile* cache_file = nullptr;
    BlockMetadata* cache_metadata = nullptr;
    BlockData* cache_data = nullptr;
};

struct Path
{
    char path[PATH_BYTES] = {};
    uint32_t file_addr = INVALID_BLOCK;
    uint32_t next_file_addr = INVALID_BLOCK;
};

} // namespace LogFs
