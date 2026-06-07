#pragma once

#include "hw_flash.hpp"
#include "hw_can.hpp"
#include "io_queue.hpp"

#include <cstddef>
#include <cstring>
#include <span>
#include <expected>
#include <optional>

// Keep CAN protocol in sync with:
// canup/bootloader.py

inline constexpr uint8_t STATUS_10HZ_ID_LOWBITS           = 0x0;
inline constexpr uint8_t START_UPDATE_ID_LOWBITS          = 0x1;
inline constexpr uint8_t UPDATE_ACK_ID_LOWBITS            = 0x2;
inline constexpr uint8_t GO_TO_APP_LOWBITS                = 0x3;
inline constexpr uint8_t ERASE_SECTOR_ID_LOWBITS          = 0x4;
inline constexpr uint8_t ERASE_SECTOR_COMPLETE_ID_LOWBITS = 0x5;
inline constexpr uint8_t PROGRAM_ID_LOWBITS               = 0x6;
inline constexpr uint8_t VERIFY_ID_LOWBITS                = 0x7;
inline constexpr uint8_t APP_VALIDITY_ID_LOWBITS          = 0x8;
inline constexpr uint8_t GO_TO_BOOT                       = 0x9;
inline constexpr uint8_t ERASE_SECTOR_FAILED_ID_LOWBITS   = 0xA;
inline constexpr uint8_t PROGRAM_ID_FAILED_LOWBITS        = 0xB;

namespace bootloader
{
class config
{
  public:
    io::queue<hw::CanMsg, 256> &can_tx_queue;
    io::queue<hw::CanMsg, 256> &can_rx_queue;

    uint32_t BOARD_HIGHBITS{ 0 };
    uint32_t _GIT_COMMIT_HASH{ 0 };
    bool     _GIT_COMMIT_CLEAN{ false };
    // if this is nullopt, then flash is clean
    std::optional<uint32_t>                                    block_addr = std::nullopt;
    std::array<std::byte, hw::flash::WORD_BYTES>               block_buffer{};
    std::array<bool, hw::flash::WORD_BYTES / sizeof(uint64_t)> block_buffer_filled{};

#if defined(STM32H733xx) || defined(STM32H562xx)
    hw::fdcan &fdcan_handle;
    config(
        hw::fdcan                  &fdcan_handle_in,
        io::queue<hw::CanMsg, 256> &tx_queue,
        io::queue<hw::CanMsg, 256> &rx_queue,
        const uint32_t              board_highbits,
        const uint32_t              git_commit_hash,
        const bool                  git_commit_clean)
      : can_tx_queue(tx_queue),
        can_rx_queue(rx_queue),
        BOARD_HIGHBITS(board_highbits),
        _GIT_COMMIT_HASH(git_commit_hash),
        _GIT_COMMIT_CLEAN(git_commit_clean),
        fdcan_handle(fdcan_handle_in){};
#elif defined(STM32F412Rx)
    hw::can &can_handle;
    config(
        hw::can                    &can_handle_in,
        io::queue<hw::CanMsg, 256> &tx_queue,
        io::queue<hw::CanMsg, 256> &rx_queue,
        uint32_t                    board_highbits,
        uint32_t                    git_commit_hash,
        bool                        git_commit_clean)
      : can_tx_queue(tx_queue),
        can_rx_queue(rx_queue),
        BOARD_HIGHBITS(board_highbits),
        GIT_COMMIT_HASH(git_commit_hash),
        GIT_COMMIT_CLEAN(git_commit_clean),
        can_handle(can_handle_in){};
#endif

    virtual ~config() = default;

    virtual void boardSpecific_init(){};

    virtual void boardSpecific_tick(){};

    /**
     * @param address memory address to write to
     * @param data data to write
     * @return
     */
    virtual result<void> boardSpecific_program(const uint32_t address, const uint64_t data)
    {
        assert(address % sizeof(uint64_t) == 0); // must write 8 bytes at a time
        const uint32_t block_offset = address % hw::flash::WORD_BYTES;
        if (not block_addr.has_value())
        {
            block_addr = address - block_offset;
        }
        else
        {
            // check that the addr is valid in this block
            if (block_addr.value() <= address && address < block_addr.value() + hw::flash::WORD_BYTES)
            {
                // TODO return an error indicating that the last block has not been written yet
                return std::unexpected(ErrorCode::INVALID_ARGS);
            }
        }
        assert(block_addr.has_value());
        assert(block_addr.value() % hw::flash::WORD_BYTES == 0);

        std::memcpy(&block_buffer[block_offset], &data, sizeof(data));
        block_buffer_filled[block_offset / sizeof(uint64_t)] = true;

        // On the STM32H733xx microcontroller, the smallest amount of memory you can
        // program at a time is 32 bytes (one "flash word" FLASH_WORD_BYTES = 32B).
        // On the STM32H562xx microcontroller, the largest amount of memory you can
        // program at a time is 16 bytes (quad "flash word" FLASH_WORD_BYTES = 16B).
        // This means we cannot program the data from a single CAN message, since the CAN message has at most 8 bytes of
        // data. So we keep track of the data to program in a buffer in RAM, until the buffer is full, and then we can
        // write the entire flash word. This implementation only works if the binary size is a multiple of 32 bytes, or
        // the buffer won't fill up for the last few bytes so won't be written into flash. This is guaranteed by canup.

        if (block_offset + sizeof(uint64_t) != hw::flash::WORD_BYTES)
        {
            // have not filled the buffer yet
            return std::unexpected(ErrorCode::ERROR_INDETERMINATE);
        }
        const auto res = hw::flash::programFlash(address / hw::flash::WORD_BYTES * hw::flash::WORD_BYTES, block_buffer);
        block_addr     = std::nullopt;
        return res;
    }
};

void              preInit();
void              init(config &boot_config);
[[noreturn]] void runInterfaceTask(config &boot_config);
[[noreturn]] void runTickTask(config &boot_config);
[[noreturn]] void runCanTxTask(config &boot_config);

} // namespace bootloader