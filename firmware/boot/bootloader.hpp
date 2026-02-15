#pragma once
#include "util_utils.hpp"
#include <cstdint>
#include "hw_flash.hpp"
#include "hw_can.hpp"
#include <cstring>
#include <io_canMsg.hpp>
#include <span>
#include "io_queue.hpp"
#include "cmsis_gcc.h"

// Keep CAN protocol in sync with:
// canup/bootloader.py

#define STATUS_10HZ_ID_LOWBITS (0x0)
#define START_UPDATE_ID_LOWBITS (0x1)
#define UPDATE_ACK_ID_LOWBITS (0x2)
#define GO_TO_APP_LOWBITS (0x3)
#define ERASE_SECTOR_ID_LOWBITS (0x4)
#define ERASE_SECTOR_COMPLETE_ID_LOWBITS (0x5)
#define PROGRAM_ID_LOWBITS (0x6)
#define VERIFY_ID_LOWBITS (0x7)
#define APP_VALIDITY_ID_LOWBITS (0x8)
#define GO_TO_BOOT (0x9)

namespace bootloader
{

void tx_overflow_callback(const uint32_t overflow_count);

void rx_overflow_callback(const uint32_t overflow_count);

void tx_overflow_clear_callback();
void rx_overflow_clear_callback();

class config
{
  public:

#if defined(STM32H733xx) || defined(STM32H562xx)
    hw::fdcan fdcan_handle;
#elif defined(STM32F412Rx)
    hw::can can_handle;
#endif

    io::queue<io::CanMsg, 128> can_tx_queue{
        "CanTxQueue",
        tx_overflow_callback,
        tx_overflow_clear_callback
    };

    io::queue<io::CanMsg, 128> can_rx_queue{
        "CanRxQueue",
        rx_overflow_callback,
        rx_overflow_clear_callback
    };

    uint32_t BOARD_HIGHBITS{0};
    uint32_t GIT_COMMIT_HASH{0};
    bool GIT_COMMIT_CLEAN;
    

    virtual ~config() = default;

    virtual void boardSpecific_init(void){};

    virtual void boardSpecific_tick(void){};

    virtual void boardSpecific_program(uint32_t address, uint64_t data)
    {
        std::span<std::byte, hw::flash::WORD_BYTES> program_buffer;
        uint32_t buffer_idx = address % hw::flash::WORD_BYTES;
        std::memcpy(&program_buffer[buffer_idx], &data, sizeof(data));

        // On the STM32H733xx microcontroller, the smallest amount of memory you can
        // program at a time is 32 bytes (one "flash word" FLASH_WORD_BYTES = 32B).
        // On the STM32H562xx microcontroller, the largest amount of memory you can
        // program at a time is 16 bytes (quad "flash word" FLASH_WORD_BYTES = 16B).
        // This means we cannot program the data from a single CAN message, since the CAN message has at most 8 bytes of
        // data. So we keep track of the data to program in a buffer in RAM, until the buffer is full, and then we can
        // write the entire flash word. This implementation only works if the binary size is a multiple of 32 bytes, or
        // the buffer won't fill up for the last few bytes so won't be written into flash. This is guaranteed by canup.
        if (buffer_idx + sizeof(uint64_t) == hw::flash::WORD_BYTES)
        {  
            std::span<const std::byte, hw::flash::WORD_BYTES> spano(program_buffer);
            hw::flash::programFlash(address / hw::flash::WORD_BYTES * hw::flash::WORD_BYTES, program_buffer);
        }
    };
};

void preInit(void);
void init(void);
[[noreturn]] void runInterfaceTask(config &boot_config);
[[noreturn]] void runTickTask(config &boot_config);
[[noreturn]] void runCanTxTask(config &boot_config);

} // namespace bootloader