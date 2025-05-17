#include "bootloader.h"
#include "bootloaderConfig.h"
#include "main.h"

#include <stdint.h>
#include <string.h>

#include "cmsis_gcc.h"
#include "cmsis_os.h"

#include "app_commitInfo.h"
#include "io_canQueue.h"
#include "io_log.h"

#include "hw_hardFaultHandler.h"
#include "hw_flash.h"
#include "hw_utils.h"
#include "hw_crc.h"
#include "hw_hal.h"

#ifdef STM32H733xx
#include "hw_fdcan.h"
#elif STM32F412Rx
#include "hw_can.h"
#else
#error "Please define what MCU is used"
#endif

#include "app_utils.h"
#include <assert.h>

extern CRC_HandleTypeDef hcrc;
extern TIM_HandleTypeDef htim6;

// Need these to be created an initialized elsewhere
extern CanHandle can;

void canRxQueueOverflowCallBack(const uint32_t unused)
{
    UNUSED(unused);
    BREAK_IF_DEBUGGER_CONNECTED();
}

void canTxQueueOverflowCallBack(const uint32_t unused)
{
    UNUSED(unused);
    // BREAK_IF_DEBUGGER_CONNECTED();
}

// App code block. Start/size included from the linker script.
// Info needed by the bootloader to boot safely. Currently takes up the the first kB
// of flash allocated to the app.
typedef struct
{
    uint32_t checksum;
    uint32_t size_bytes;
    uint32_t bootloader_status;
} Metadata;

extern Metadata __app_metadata_start__; // NOLINT(*-reserved-identifier)
extern uint32_t __app_metadata_size__;  // NOLINT(*-reserved-identifier)

// App metadata block. Start/size included from the linker script.
extern uint32_t __app_code_start__; // NOLINT(*-reserved-identifier)
extern uint32_t __app_code_size__;  // NOLINT(*-reserved-identifier)

// Boot flag from RAM
__attribute__((section(".boot_flag"))) volatile uint8_t boot_flag;

typedef enum
{
    BOOT_STATUS_APP_VALID,
    BOOT_STATUS_APP_INVALID,
    BOOT_STATUS_NO_APP
} BootStatus;

_Noreturn static void modifyStackPointerAndStartApp(const uint32_t *address)
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    uint32_t app_sp    = address[0];
    uint32_t app_start = address[1];
#pragma GCC diagnostic pop
    __set_MSP(app_sp);
    void (*app_reset_handler)(void) = (void (*)(void))app_start;
    app_reset_handler(); // Call app's Reset_Handler, starting the app.

    // Should never get here!
    BREAK_IF_DEBUGGER_CONNECTED()
    for (;;)
    {
    }
}

static BootStatus verifyAppCodeChecksum(void)
{
    // ReSharper disable once CppRedundantDereferencingAndTakingAddress
    if (*&__app_code_start__ == 0xFFFFFFFF)
    {
        // If app initial stack pointer is all 0xFF, assume app is not present.
        return BOOT_STATUS_NO_APP;
    }

    const Metadata *metadata = &__app_metadata_start__;
    if (metadata->size_bytes > (uint32_t)&__app_code_size__)
    {
        // App binary size field is invalid.
        return BOOT_STATUS_APP_INVALID;
    }

    const uint32_t calculated_checksum = hw_crc_calculate(&__app_code_start__, metadata->size_bytes / 4);
    return calculated_checksum == metadata->checksum ? BOOT_STATUS_APP_VALID : BOOT_STATUS_APP_INVALID;
}

static uint32_t current_address;
static bool     update_in_progress;

void bootloader_preInit(void)
{
    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("Bootloader reset!");
}

void bootloader_init(void)
{
    // HW-level CAN should be initialized in main.c, since it is MCU-specific.
    hw_hardFaultHandler_init();
    hw_crc_init(&hcrc);
    // This order is important! The bootloader starts the app when the bootloader
    // enable pin is high, which is caused by pullup resistors internal to each
    // MCU. However, at this point only the PDM is powered up. Empirically, the PDM's
    // resistor alone isn't strong enough to pull up the enable line, and so the
    // PDM will fail to boot. A bad fix for this is to turn on the rest of the
    // boards here first, so the PDM will get help pulling up the line from the
    // other MCUs.
    bootloader_boardSpecific_init();

    if (verifyAppCodeChecksum() == BOOT_STATUS_APP_VALID && boot_flag != 0x1)
    {
        // Deinit peripherals.
        HAL_TIM_Base_Stop_IT(&htim6);
        // HAL_TIM_Base_DeInit(&htim6);
        HAL_CRC_DeInit(&hcrc);
        // HAL_RCC_DeInit();

        // Clear RCC register flag and RAM boot flag.
        boot_flag = 0x0;

        // Jump to app.
        modifyStackPointerAndStartApp(&__app_code_start__);
    }

    hw_can_init(&can);
    io_canQueue_init();
}

_Noreturn void bootloader_runInterfaceTask(void)
{
    for (;;)
    {
        const CanMsg command = io_canQueue_popRx();

        if (command.std_id == START_UPDATE_ID)
        {
            // Reset current address to program and update state.
            current_address    = (uint32_t)&__app_metadata_start__;
            update_in_progress = true;

            // Send ACK message that programming has started.
            const CanMsg reply = { .std_id = UPDATE_ACK_ID, .dlc = 0 };
            io_canQueue_pushTx(&reply);
        }
        else if (command.std_id == ERASE_SECTOR_ID && update_in_progress)
        {
            // Erase a flash sector.
            const uint8_t sector = command.data[0];
            hw_flash_eraseSector(sector);

            // Erasing sectors takes a while, so reply when finished.
            CanMsg reply = {
                .std_id = ERASE_SECTOR_COMPLETE_ID,
                .dlc    = 0,
            };
            io_canQueue_pushTx(&reply);
        }
        else if (command.std_id == PROGRAM_ID && update_in_progress)
        {
            // Program 64 bits at the current address.
            // No reply for program command to reduce latency.
            bootloader_boardSpecific_program(current_address, *(uint64_t *)command.data);
            current_address += sizeof(uint64_t);
        }
        else if (command.std_id == VERIFY_ID && update_in_progress)
        {
            // Verify received checksum matches the one saved in flash.
            CanMsg reply = {
                .std_id = APP_VALIDITY_ID,
                .dlc    = 1,
            };
            reply.data[0] = (uint8_t)verifyAppCodeChecksum();
            io_canQueue_pushTx(&reply);

            // Verify command doubles as exit programming state command.
            update_in_progress = false;
        }
        else if (command.std_id == GO_TO_APP && !update_in_progress)
        {
            boot_flag = 0x0;
            HAL_TIM_Base_Stop_IT(&htim6);
            HAL_CRC_DeInit(&hcrc);
            modifyStackPointerAndStartApp(&__app_code_start__);
        }
    }
}

_Noreturn void bootloader_runTickTask(void)
{
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        // Broadcast a message at 1Hz so we can check status over CAN.
        CanMsg status_msg  = { .std_id = STATUS_10HZ_ID, .dlc = 5 };
        status_msg.data[0] = (uint8_t)((0x000000ff & GIT_COMMIT_HASH) >> 0);
        status_msg.data[1] = (uint8_t)((0x0000ff00 & GIT_COMMIT_HASH) >> 8);
        status_msg.data[2] = (uint8_t)((0x00ff0000 & GIT_COMMIT_HASH) >> 16);
        status_msg.data[3] = (uint8_t)((0xff000000 & GIT_COMMIT_HASH) >> 24);
        status_msg.data[4] = (uint8_t)(verifyAppCodeChecksum() << 1) | GIT_COMMIT_CLEAN;
        io_canQueue_pushTx(&status_msg);

        bootloader_boardSpecific_tick();

        start_ticks += 100; // 10Hz tick
        osDelayUntil(start_ticks);
    }
}

_Noreturn void bootloader_runCanTxTask(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx();
        LOG_IF_ERR(hw_can_transmit(&can, &tx_msg));
    }
}

__WEAK void bootloader_boardSpecific_init(void) {}

__WEAK void bootloader_boardSpecific_tick(void) {}
