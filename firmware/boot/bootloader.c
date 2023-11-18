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
#include "stm32f4xx_hal_gpio.h"

extern CRC_HandleTypeDef hcrc;
extern CAN_HandleTypeDef hcan1;
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
    BREAK_IF_DEBUGGER_CONNECTED();
}

static void canTxOverflow(uint32_t unused)
{
    BREAK_IF_DEBUGGER_CONNECTED();
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
static bool     loading_binary;

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

    hw_hardFaultHandler_init();
    hw_crc_init(&hcrc);
    hw_can_init(&hcan1);
    io_can_init(&can_config);
    bootloader_boardSpecific_init();
}

void bootloader_runInterfaceTask()
{
    for (;;)
    {
        CanMsg command;
        io_can_popRxMsgFromQueue(&command);

        if (command.std_id == START_UPDATE_ID)
        {
            // Reset current address to program and update state.
            current_address = (uint32_t)&__app_metadata_start__;
            loading_binary  = true;

            // Send ACK message that programming has started.
            CanMsg reply = { .std_id = UPDATE_ACK_ID, .dlc = 0 };
            io_can_pushTxMsgToQueue(&reply);
        }
        if (command.std_id == ERASE_SECTOR_ID && loading_binary)
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
        else if (command.std_id == PROGRAM_ID && loading_binary)
        {
            // Program 64 bits at the current address.
            // No reply for program command to reduce latency.
            uint32_t lsb_word = *(uint32_t *)command.data;
            uint32_t msb_word = *(uint32_t *)&command.data[sizeof(uint32_t)];
            hw_flash_programWord(current_address, lsb_word);
            hw_flash_programWord(current_address + sizeof(uint32_t), msb_word);
            current_address += 2 * sizeof(uint32_t);
        }
        else if (command.std_id == VERIFY_ID && loading_binary)
        {
            // Verify received checksum matches the one saved in flash.
            CanMsg reply = {
                .std_id = APP_VALIDITY_ID,
                .dlc    = 1,
            };
            reply.data[0] = verifyAppCodeChecksum();
            io_can_pushTxMsgToQueue(&reply);

            // Verify command doubles as exit programming state command.
            loading_binary = false;
        }
    }
}

void bootloader_runTickTask()
{
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        // Some boards don't have a "boot mode" GPIO and just jump directly to app.
        if (verifyAppCodeChecksum() == BOOT_STATUS_APP_VALID && !loading_binary
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
            bootloader_boardSpecific_deinit();

            // Jump to app.
            modifyStackPointerAndStartApp(&__app_code_start__);
        }

        // Broadcast a message at 1Hz so we can check status over CAN.
        CanMsg status_msg  = { .std_id = STATUS_10HZ_ID, .dlc = 1 };
        status_msg.data[0] = verifyAppCodeChecksum();
        io_can_pushTxMsgToQueue(&status_msg);

        bootloader_boardSpecific_tick();

        start_ticks += 100; // 10Hz tick
        osDelayUntil(start_ticks);
    }
}

void bootloader_runCanTxTask()
{
    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

__WEAK void bootloader_boardSpecific_init() {}

__WEAK void bootloader_boardSpecific_deinit() {}

__WEAK void bootloader_boardSpecific_tick() {}
