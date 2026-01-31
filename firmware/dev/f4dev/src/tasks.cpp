#include "tasks.h"

extern "C"
{
#include "main.h"
}

#include "usbd_cdc_if.h"
#include "hw_can.hpp"
// #include "hw_usb.hpp"
// #include "hw_chimeraConfig_v2.hpp"
// #include "hw_sd.hpp"
#include "util_errorCodes.hpp"

// #include "hw_bootup.h"
// #include "hw_uart.h"
// #include "shared.pb.h"
// #include "f4dev.pb.h"
// #include "hw_gpio.h"
// #include "hw_gpios.h"
// #include "hw_adcs.h"
// #include "io_log.h"

#include <cassert>

// static SdCard *sd;

// CanHandle can = { .hcan = &hcan2, .bus_num = 0, .receive_callback = [](const CanMsg *rx_msg) {}, .ready = false };
static void canRxCallback(const io::CanMsg &msg)
{
    // can_rx_queue.pushMsgToQueue(msg);
}

hw::can can1(hcan1, 0, canRxCallback);

const hw::can &hw::can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can1.getHcan());
    return can1;
}
// int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
// int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
// int lfs_erase(const struct lfs_config *c, lfs_block_t block);
// int lfs_sync(const struct lfs_config *c);

// int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
// {
//     SdCardStatus status = hw_sd_readOffset(sd, (uint8_t *)buffer, (uint32_t)block, (uint32_t)off, (uint32_t)size);

//     return (status != SD_CARD_OK) ? 0 : 1;
// }

// int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
// {
//     SdCardStatus status = hw_sd_writeOffset(sd, (uint8_t *)buffer, (uint32_t)block, (uint32_t)off, (uint32_t)size);
//     return (status != SD_CARD_OK) ? 0 : 1;
// }

// int lfs_erase(const struct lfs_config *c, lfs_block_t block)
// {
//     SdCardStatus status = hw_sd_erase(sd, (uint32_t)block, (uint32_t)block);
//     return (status != SD_CARD_OK) ? 0 : 1;
// }

// int lfs_sync(const struct lfs_config *c)
// {
//     return 0;
// }

// struct lfs_config cfg = {
//     // block device operations
//     .read  = lfs_read,
//     .prog  = lfs_prog,
//     .erase = lfs_erase,
//     .sync  = lfs_sync,

//     // block device configuration
//     .read_size        = 16,
//     .prog_size        = 16,
//     .block_size       = 4096,
//     .block_count      = 128,
//     .cache_size       = LFS_CACHE_SIZE,
//     .lookahead_size   = LFS_LOOKAHEAD_SIZE,
//     .block_cycles     = 500,
//     .read_buffer      = (void *)read_buffer,
//     .prog_buffer      = (void *)prog_buffer,
//     .lookahead_buffer = (void *)lookahead_buffer,

// };

// lfs_t      lfs;
// lfs_file_t file;

void tasks_init()
{
    // sd->hsd     = &hsd;
    // sd->timeout = 1000;

    // // config littlefs
    // cfg.block_size  = sd->hsd->SdCard.BlockSize;
    // cfg.block_count = sd->hsd->SdCard.BlockNbr;

    // // mount the filesystem
    // int err = lfs_mount(&lfs, &cfg);
    // // write the hello world
    // if (err)
    // {
    //     // reformat if we can't mount the filesystem
    //     // this should only happen on the first boot
    //     err = lfs_format(&lfs, &cfg);
    //     err = lfs_mount(&lfs, &cfg);
    // }

    // // read current count
    // uint32_t boot_count = 0;
    // // mkdir
    // err = lfs_mkdir(&lfs, "helloworld_dir");

    // err = lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    // if (!err)
    // {
    //     err = lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));
    // }

    // // update boot count
    // boot_count += 1;
    // err = lfs_file_rewind(&lfs, &file);
    // if (!err)
    // {
    //     err = lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));
    // }

    // ASSERT_EXIT_OK(hw_usb_init());
}

void tasks_default()
{
    // hw_chimera_v2_task(&chimera_v2_config);
    for (;;)
        ;
}