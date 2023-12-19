#include "bootloader.h"
#include "config.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "cmsis_gcc.h"
#include "cmsis_os.h"
#include "hw_hal.h"
#include "io_can.h"
#include "hw_flash.h"
#include "hw_hardFaultHandler.h"
#include "Io_SharedMacros.h"
#include "hw_crc.h"
#include "main.h"
#include "hw_gpio.h"

extern CRC_HandleTypeDef hcrc;
extern TIM_HandleTypeDef htim6;

// App code block. Start/size included from the linker script.
extern uint32_t __app_metadata_start__;
extern uint32_t __app_metadata_size__;

// App metadata block. Start/size included from the linker script.
extern uint32_t __app_code_start__;
extern uint32_t __app_code_size__;

// Info needed by the bootloader to boot safely. Currently takes up the the first kB
// of flash allocated to the app.
typedef struct
{
    uint32_t checksum;
    uint32_t size_bytes;
} Metadata;

typedef enum
{
    BOOT_STATUS_APP_VALID,
    BOOT_STATUS_APP_INVALID,
    BOOT_STATUS_NO_APP
} BootStatus;

static void canRxOverflow(uint32_t unused)
{
    // BREAK_IF_DEBUGGER_CONNECTED();
}

static void canTxOverflow(uint32_t unused)
{
    // BREAK_IF_DEBUGGER_CONNECTED();
}

static void modifyStackPointerAndStartApp(uint32_t *address)
{
    // Disable interrupts before jumping.
    __disable_irq();

    // Disable system tick to stop FreeRTOS timebase.
    SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;

    // Clear all pending interrupts by setting all ICPRs (Interrupt Clear Pending Register)
    // to 0xFFFFFFFF. This is done so no interrupts are queued up when we jump to the app.
    // (There are 8 registers on the Cortex-M4)
    for (uint32_t i = 0; i < 8; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    // Update the vector table offset register. When an interrupt is fired,
    // the microcontroller looks for the corresponding interrupt service handler
    // at the memory address in the VTOR. We need to update it so the app ISRs
    // are used.
    SCB->VTOR = (uint32_t)address;

    // Flush processor pipeline.
    __ISB();

    // Tell MCU to use the main stack pointer rather than process stack pointer (PSP is used with RTOS)
    __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);

    // Modify stack pointer and jump to app code.
    // In a binary built with our linker settings, the interrupt vector table is the first thing
    // placed in flash. The first word of the vector table contains the initial stack pointer
    // and the second word containers the address of the reset handler. Update stack pointer and
    // program counter accordingly.
    uint32_t app_sp    = address[0];
    uint32_t app_start = address[1];
    __set_MSP(app_sp);
    void (*app_reset_handler)(void) = (void (*)(void))app_start;
    app_reset_handler(); // Call app's Reset_Handler, starting the app.

    // Should never get here!
    BREAK_IF_DEBUGGER_CONNECTED();
    for (;;)
    {
    }
}

static BootStatus verifyAppCodeChecksum(void)
{
    if (*&__app_code_start__ == 0xFFFFFFFF)
    {
        // If app initial stack pointer is all 0xFF, assume app is not present.
        return BOOT_STATUS_NO_APP;
    }

    Metadata *metadata = (Metadata *)&__app_metadata_start__;
    if (metadata->size_bytes > (uint32_t)&__app_code_size__)
    {
        // App binary size field is invalid.
        return BOOT_STATUS_APP_INVALID;
    }

    uint32_t calculated_checksum = hw_crc_calculate(&__app_code_start__, metadata->size_bytes / 4);
    return calculated_checksum == metadata->checksum ? BOOT_STATUS_APP_VALID : BOOT_STATUS_APP_INVALID;
}

static const CanConfig can_config = {
    .rx_msg_filter        = NULL,
    .rx_overflow_callback = canRxOverflow,
    .tx_overflow_callback = canTxOverflow,
};
static const Gpio bootloader_pin = {
    .port = BOOT_GPIO_PORT,
    .pin  = BOOT_GPIO_PIN,
};

static uint32_t current_address;
static bool     update_in_progress;
static uint32_t prog_count;

void bootloader_init()
{
#ifndef BOOT_AUTO
    // This file is shared between multiple boards, meaning we cannot rely on STM32CubeMX
    // to initialize the peripherals. So we do it manually here.
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef bootloader_gpio_init = {
        .Pin  = BOOT_GPIO_PIN,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLUP,
    };
    HAL_GPIO_Init(BOOT_GPIO_PORT, &bootloader_gpio_init);
#endif

    // HW-level CAN should be initialized in main.c, since it is MCU-specific.
    hw_hardFaultHandler_init();
    hw_crc_init(&hcrc);
    io_can_init(&can_config);

    // Some boards don't have a "boot mode" GPIO and just jump directly to app.
    if (verifyAppCodeChecksum() == BOOT_STATUS_APP_VALID && false
#ifndef BOOT_AUTO
        && hw_gpio_readPin(&bootloader_pin)
#endif
    )
    {
        // Deinit peripherals.
#ifndef BOOT_AUTO
        HAL_GPIO_DeInit(BOOT_GPIO_PORT, BOOT_GPIO_PIN);
#endif
        HAL_TIM_Base_Stop_IT(&htim6);
        HAL_CRC_DeInit(&hcrc);
        hw_can_deinit();

        // Jump to app.
        modifyStackPointerAndStartApp(&__app_code_start__);
    }

    bootloader_boardSpecific_init();
}

uint32_t prog_buf[8];

void bootloader_runInterfaceTask()
{
    for (;;)
    {
        CanMsg command;
        io_can_popRxMsgFromQueue(&command);

        if (command.std_id == START_UPDATE_ID)
        {
            // Reset current address to program and update state.
            current_address    = (uint32_t)&__app_metadata_start__;
            update_in_progress = true;

            // Send ACK message that programming has started.
            CanMsg reply = { .std_id = UPDATE_ACK_ID, .dlc = 0 };
            io_can_pushTxMsgToQueue(&reply);
            prog_count = 0;
        }
        else if (command.std_id == ERASE_SECTOR_ID && update_in_progress)
        {
            // Erase a flash sector.
            uint8_t sector = command.data[0];
            hw_flash_eraseSector(sector);

            // Erasing sectors takes a while, so reply when finished.
            CanMsg reply = {
                .std_id = ERASE_SECTOR_COMPLETE_ID,
                .dlc    = 0,
            };
            io_can_pushTxMsgToQueue(&reply);
        }
        else if (command.std_id == PROGRAM_ID && update_in_progress)
        {
            // Program 64 bits at the current address.
            // No reply for program command to reduce latency.
            uint32_t lsb_word = *(uint32_t *)command.data;
            uint32_t msb_word = *(uint32_t *)&command.data[sizeof(uint32_t)];
            // hw_flash_programWord(current_address, lsb_word);
            // hw_flash_programWord(current_address + sizeof(uint32_t), msb_word);

            uint32_t lsb_adr = (current_address / 4) % 8;
            uint32_t msb_adr = lsb_adr + 1;
            prog_buf[lsb_adr] = lsb_word;
            prog_buf[msb_adr] = msb_word;

            current_address += 2 * 4;
            prog_count += 2;

            if(current_address % 32 == 0)
            {
                // can only program 32 bytes at a time on h7
                // doing something nice with readback and then write didn't work for some reason ??
                // so here's a shitty buffered approach that works but is shitty
                hw_flash_programFlashWord(current_address - 32, prog_buf);
            }
        }
        else if (command.std_id == VERIFY_ID && update_in_progress)
        {
            // flush prog buffer
            while(true)
            {
                uint32_t lsb_adr = (current_address / 4) % 8;
                uint32_t msb_adr = lsb_adr + 1;
                prog_buf[lsb_adr] = 0xFFFFFFFF;
                prog_buf[msb_adr] = 0xFFFFFFFF;

                current_address += 2 * 4;
                prog_count += 2;

                if(current_address % 32 == 0)
                {
                    hw_flash_programFlashWord(current_address - 32, prog_buf);
                    break;
                }
            }

            // Verify received checksum matches the one saved in flash.
            CanMsg reply = {
                .std_id = APP_VALIDITY_ID,
                .dlc    = 1,
            };
            reply.data[0] = verifyAppCodeChecksum();
            io_can_pushTxMsgToQueue(&reply);

            // Verify command doubles as exit programming state command.
            update_in_progress = false;
        }
    }
}

void bootloader_runTickTask()
{
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        osDelay(1000);
    }

    // for (;;)
    // {
    //     // Broadcast a message at 1Hz so we can check status over CAN.
    //     CanMsg status_msg  = { .std_id = STATUS_10HZ_ID, .dlc = 1 };
    //     status_msg.data[0] = verifyAppCodeChecksum();
    //     io_can_pushTxMsgToQueue(&status_msg);

    //     bootloader_boardSpecific_tick();

    //     start_ticks += 100; // 10Hz tick
    //     osDelayUntil(start_ticks);
    // }
}

void bootloader_runCanTxTask()
{
    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

__WEAK void bootloader_boardSpecific_init() {}

__WEAK void bootloader_boardSpecific_tick() {}
