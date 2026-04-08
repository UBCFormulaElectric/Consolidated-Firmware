#include "bootloader.hpp"
#include "bootloader_RSM.hpp"
#include "io_canQueues.hpp"
#include "main.h"

void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}

void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}

void tx_overflow_clear_callback(){};
void rx_overflow_clear_callback(){};

io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue", tx_overflow_callback, tx_overflow_clear_callback };
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue", rx_overflow_callback, rx_overflow_clear_callback };

namespace hw::cans
{
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
}

bootloader::config RSM_boot_config(hw::cans::fdcan1, boot_can_tx_queue, boot_can_rx_queue, board_highbits, git_commit_hash_val, git_commit_clean_val);

void bootloader_preInit()
{
    bootloader::preInit();
}

void bootloader_init()
{
    bootloader::init(RSM_boot_config);
}