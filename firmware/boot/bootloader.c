#include "bootloader.h"
#include "app_crc32.h"
#include "bootloaderConfig.h"
#include "hw_bootup.h"
#include "main.h"

#include <stdbool.h>
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
#include "hw_hal.h"

#if defined(STM32H733xx) || defined(STM32H563xx)
#include "hw_fdcan.h"
#elif defined(STM32F412Rx)
#include "hw_can.h"
#else
#error "Please define what MCU is used"
#endif

#include "app_utils.h"

// SYS tick timer
extern TIM_HandleTypeDef htim6;

// Need these to be created an initialized elsewhere (main.c)
extern CanHandle can;

void canRxQueueOverflowCallBack(const uint32_t unused)
{
    UNUSED(unused);
    BREAK_IF_DEBUGGER_CONNECTED();
}

void canTxQueueOverflowCallBack(const uint32_t unused)
{
    UNUSED(unused);
    BREAK_IF_DEBUGGER_CONNECTED();
}

// App code block. Start/size included from the linker script.
// Info needed by the bootloader to boot safely. Currently takes up the the first kB
// of flash allocated to the app.
typedef struct
{
    uint32_t checksum;
    uint32_t size_bytes;
} Metadata;

extern Metadata __app_metadata_start__; // NOLINT(*-reserved-identifier)
extern uint32_t __app_metadata_size__;  // NOLINT(*-reserved-identifier)

// App metadata block. Start/size included from the linker script.
extern uint32_t __app_code_start__; // NOLINT(*-reserved-identifier)
extern uint32_t __app_code_size__;  // NOLINT(*-reserved-identifier)

typedef enum
{
    BOOT_STATUS_APP_VALID,
    BOOT_STATUS_APP_INVALID,
    BOOT_STATUS_NO_APP
} BootStatus;

static CanTxQueue can_tx_queue;
static uint32_t   current_address;
static bool       update_in_progress;
static BootStatus boot_status;

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

void verifyAppCodeChecksum(void)
{
    // ReSharper disable once CppRedundantDereferencingAndTakingAddress
    if (*&__app_code_start__ == 0xFFFFFFFF)
    {
        // If app initial stack pointer is all 0xFF, assume app is not present.
        boot_status = BOOT_STATUS_NO_APP;
        return;
    }

    const Metadata *metadata = &__app_metadata_start__;
    if (metadata->size_bytes > (uint32_t)&__app_code_size__)
    {
        // App binary size field is invalid.
        boot_status = BOOT_STATUS_APP_INVALID;
        return;
    }

    const uint32_t calculated_checksum =
        app_crc32_finalize(app_crc32_update(app_crc32_init(), &__app_code_start__, metadata->size_bytes));
    boot_status = calculated_checksum == metadata->checksum ? BOOT_STATUS_APP_VALID : BOOT_STATUS_APP_INVALID;
}

void bootloader_preInit(void)
{
    hw_hardFaultHandler_init();

    verifyAppCodeChecksum();
    if (boot_status == BOOT_STATUS_APP_VALID && hw_bootup_getBootRequest().target == BOOT_TARGET_APP)
    {
        // Jump to app.
        modifyStackPointerAndStartApp(&__app_code_start__);
    }

    // Boot request targetting bootloader. Overwrite it to target app next so we don't get stuck here.
    const BootRequest app_request = { .target = BOOT_TARGET_APP, .context = BOOT_CONTEXT_NONE, .context_value = 0 };
    hw_bootup_setBootRequest(app_request);
}

void bootloader_init(void)
{
    // Configure and initialize SEGGER SystemView. Must be done after clocks are configured.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("Bootloader reset!");

    hw_can_init(&can);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    bootloader_boardSpecific_init();
}

_Noreturn void bootloader_runInterfaceTask(void)
{
    for (;;)
    {
        const CanMsg command = io_canQueue_popRx();

        if (command.std_id == (BOARD_HIGHBITS | START_UPDATE_ID_LOWBITS))
        {
            // Reset current address to program and update state.
            current_address    = (uint32_t)&__app_metadata_start__;
            update_in_progress = true;

            // Send ACK message that programming has started.
            const CanMsg reply = { .std_id = BOARD_HIGHBITS | UPDATE_ACK_ID_LOWBITS, .dlc = 0 };
            io_canQueue_pushTx(&can_tx_queue, &reply);
        }
        else if (command.std_id == (BOARD_HIGHBITS | ERASE_SECTOR_ID_LOWBITS) && update_in_progress)
        {
            // Erase a flash sector.
            const uint8_t sector = command.data.data8[0];
            hw_flash_eraseSector(sector);

            // Erasing sectors takes a while, so reply when finished.
            CanMsg reply = {
                .std_id = (BOARD_HIGHBITS | ERASE_SECTOR_COMPLETE_ID_LOWBITS),
                .dlc    = 0,
            };
            io_canQueue_pushTx(&can_tx_queue, &reply);
        }
        else if (command.std_id == (BOARD_HIGHBITS | PROGRAM_ID_LOWBITS) && update_in_progress)
        {
            // Program 64 bits at the current address.
            // No reply for program command to reduce latency.
            bootloader_boardSpecific_program(current_address, command.data.data64[0]);
            current_address += sizeof(uint64_t);
        }
        else if (command.std_id == (BOARD_HIGHBITS | VERIFY_ID_LOWBITS) && update_in_progress)
        {
            // Verify received checksum matches the one saved in flash.
            CanMsg reply = {
                .std_id = (BOARD_HIGHBITS | APP_VALIDITY_ID_LOWBITS),
                .dlc    = 1,
            };
            verifyAppCodeChecksum();
            reply.data.data8[0] = (uint8_t)boot_status;
            io_canQueue_pushTx(&can_tx_queue, &reply);

            // Verify command doubles as exit programming state command.
            update_in_progress = false;
        }
        else if (command.std_id == (BOARD_HIGHBITS | GO_TO_APP_LOWBITS) && !update_in_progress)
        {
            const BootRequest app_request = { .target        = BOOT_TARGET_APP,
                                              .context       = BOOT_CONTEXT_NONE,
                                              .context_value = 0 };
            hw_bootup_setBootRequest(app_request);
            NVIC_SystemReset();
        }
        else if (command.std_id == (BOARD_HIGHBITS | GO_TO_BOOT))
        {
            // Restart bootloader update state when receiving a GO_TO_BOOT command.
            update_in_progress = false;
            current_address    = (uint32_t)&__app_metadata_start__;
        }
        else
        {
            LOG_ERROR("got stdid %X", command.std_id);
        }
    }
}

_Noreturn void bootloader_runTickTask(void)
{
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!update_in_progress)
        {
            // Broadcast a message at 1Hz so we can check status over CAN.
            CanMsg status_msg         = { .std_id = BOARD_HIGHBITS | STATUS_10HZ_ID_LOWBITS, .dlc = 5 };
            status_msg.data.data32[0] = GIT_COMMIT_HASH;
            status_msg.data.data8[4]  = (uint8_t)(boot_status << 1) | GIT_COMMIT_CLEAN;
            io_canQueue_pushTx(&can_tx_queue, &status_msg);
        }

        bootloader_boardSpecific_tick();

        start_ticks += 100; // 10Hz tick
        osDelayUntil(start_ticks);
    }
}

_Noreturn void bootloader_runCanTxTask(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can_tx_queue);
#if defined(STM32H733xx) || defined(STM32H563xx)
        LOG_IF_ERR(hw_fdcan_transmit(&can, &tx_msg));
#elif defined(STM32F412Rx)
        LOG_IF_ERR(hw_can_transmit(&can, &tx_msg));
#endif
    }
}

__WEAK void bootloader_boardSpecific_init(void) {}

__WEAK void bootloader_boardSpecific_tick(void) {}
