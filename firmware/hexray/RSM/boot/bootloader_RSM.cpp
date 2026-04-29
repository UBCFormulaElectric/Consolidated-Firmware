#include "bootloader.hpp"
#include "bootloader_RSM.hpp"
#include "bootloader.h"
#include "main.h"
#include "io_log.hpp"

void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_WARN("CAN TX queue overflow");
}

void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_WARN("CAN RX queue overflow");
}

void tx_overflow_clear_callback(){};
void rx_overflow_clear_callback(){};

io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue", tx_overflow_callback, tx_overflow_clear_callback };
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue", rx_overflow_callback, rx_overflow_clear_callback };

namespace hw::cans
{
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
}

bootloader::config RSM_boot_config(
    hw::cans::fdcan1,
    boot_can_tx_queue,
    boot_can_rx_queue,
    board_highbits,
    git_commit_hash_val,
    git_commit_clean_val);

void bootloader_preInit()
{
    bootloader::preInit();
}

void bootloader_init()
{
    bootloader::init(RSM_boot_config);
}

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    return hw::cans::fdcan1;
}