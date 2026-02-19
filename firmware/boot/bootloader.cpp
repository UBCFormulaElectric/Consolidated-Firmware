#include "hw_bootup.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_flash.hpp"
#include "hw_utils.hpp"
#include "hw_hal.hpp"
#include "io_log.hpp"
#include "app_crc32.hpp"
#include "bootloader.hpp"
#include "hw_flash.hpp"

#include "cmsis_gcc.h"
#include "cmsis_os.h"
#include <io_canMsg.hpp>
#include <io_queue.hpp>
#include <expected>

#if defined(STM32H733xx) || defined(STM32H562xx)
#include "hw_fdcan.h"
#elif defined(STM32F412Rx)
#include "hw_can.h"
#else
#error "Please define what MCU is used"
#endif

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

extern uint32_t __boot_request_start__; // NOLINT(*-reserved-identifier)

enum class BootStatus : uint8_t
{
    BOOT_STATUS_APP_VALID = 0,
    BOOT_STATUS_APP_INVALID,
    BOOT_STATUS_NO_APP
};

static bool       update_in_progress;
static BootStatus boot_status;
static uint32_t   current_address;

[[noreturn]] static void modifyStackPointerAndStartApp(const uint32_t *address)
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
    SCB->VTOR = reinterpret_cast<uint32_t>(address);

    // Flush processor pipeline.
    __ISB();
    __DSB();

    // Tell MCU to use the main stack pointer rather than process stack pointer (PSP is used with RTOS)
    __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);

    // Modify stack pointer and jump to app code.
    // In a binary built with our linker settings, the interrupt vector table is the first thing
    // placed in flash. The first word of the vector table contains the initial stack pointer
    // and the second word containers the address of the reset handler. Update stack pointer and
    // program counter accordingly.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    const uint32_t app_sp    = address[0];
    const uint32_t app_start = address[1];
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

static void verifyAppCodeChecksum(void)
{
    // ReShaper disable once CppRedundantDereferencingAndTakingAddress
    if (*&__app_code_start__ == 0xFFFFFFFF)
    {
        // If app initial stack pointer is all 0xFF, assume app is not present
        boot_status = BootStatus::BOOT_STATUS_NO_APP;
        return;
    }

    const Metadata *metadata = &__app_metadata_start__;
    if (metadata->size_bytes > reinterpret_cast<uint32_t>(&__app_code_size__))
    {
        // App binary size field is invalid
        boot_status = BootStatus::BOOT_STATUS_APP_INVALID;
        return;
    }

    const uint32_t calculated_checksum =
        app::crc32::finalize(app::crc32::update(app::crc32::init(), &__app_code_start__, metadata->size_bytes));
    boot_status = calculated_checksum == metadata->checksum ? BootStatus::BOOT_STATUS_APP_VALID
                                                            : BootStatus::BOOT_STATUS_APP_INVALID;
}

void bootloader::preInit(void)
{
    hw_hardFaultHandler_init();

    // verify checksum place holder
    if (boot_status == BootStatus::BOOT_STATUS_APP_VALID &&
        hw::bootup::getBootRequest().target == hw::bootup::BootTarget::BOOT_TARGET_APP)
    {
        // Jump to app
        modifyStackPointerAndStartApp(&__app_code_start__);
    }

    // Boot request targetting bootloader. Overwrite it to target app next so we don't get stuck here
    const hw::bootup::BootRequest app_request = { .target        = hw::bootup::BootTarget::BOOT_TARGET_APP,
                                                  .context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE,
                                                  ._unused       = 0xFFFF,
                                                  .context_value = 0 };
    hw::bootup::setBootRequest(app_request);
}

void bootloader::init(void)
{
    LOG_INFO("Bootloader reset!");
    // Initialize CAN
    // Board Specific
}

[[noreturn]] void bootloader::runInterfaceTask(config &boot_config)
{
    for (;;)
    {
        auto can_msg = boot_config.can_rx_queue.pop();
        if (not can_msg.has_value())
        {
            LOG_INFO("No CAN command received");
            continue;
        }

        const io::CanMsg command = can_msg.value();

        if (command.std_id == (boot_config.BOARD_HIGHBITS | START_UPDATE_ID_LOWBITS))
        {
            // Reset current address to program and update state.
            current_address    = reinterpret_cast<uint32_t>(&__app_metadata_start__);
            update_in_progress = true;

            // Send ACK message that programming has started.
            io::CanMsg reply{};
            reply.std_id = boot_config.BOARD_HIGHBITS | UPDATE_ACK_ID_LOWBITS;
            reply.dlc    = 0;
            boot_config.can_tx_queue.push(reply);
        }
        else if (command.std_id == (boot_config.BOARD_HIGHBITS | ERASE_SECTOR_ID_LOWBITS) && update_in_progress)
        {
            // Erase a flash sector.
            const uint8_t sector = command.data.data8[0];
            (void)hw::flash::eraseSector(sector);

            // Erasing sectors takes a while, so reply when finished.
            io::CanMsg reply{};
            reply.std_id = (boot_config.BOARD_HIGHBITS | ERASE_SECTOR_COMPLETE_ID_LOWBITS);
            reply.dlc    = 0;
            boot_config.can_tx_queue.push(reply);
        }
        else if (command.std_id == (boot_config.BOARD_HIGHBITS | PROGRAM_ID_LOWBITS) && update_in_progress)
        {
            // Program 64 bits at the current address.
            // No reply for program command to reduce latency.
            boot_config.boardSpecific_program(current_address, command.data.data64[0]);
            current_address += sizeof(uint64_t);
        }
        else if (command.std_id == (boot_config.BOARD_HIGHBITS | VERIFY_ID_LOWBITS) && update_in_progress)
        {
            // Verify received checksum matches the one saved in flash.
            io::CanMsg reply{};
            reply.std_id = (boot_config.BOARD_HIGHBITS | VERIFY_ID_LOWBITS);
            reply.dlc    = 1;
            verifyAppCodeChecksum();
            reply.data.data8[0] = static_cast<uint8_t>(boot_status);
            boot_config.can_tx_queue.push(reply);

            // Verify command doubles as exit programming state command.
            update_in_progress = false;
        }
        else if (command.std_id == (boot_config.BOARD_HIGHBITS | GO_TO_APP_LOWBITS) && !update_in_progress)
        {
            const hw::bootup::BootRequest app_request = { .target        = hw::bootup::BootTarget::BOOT_TARGET_APP,
                                                          .context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE,
                                                          ._unused       = 0xFFFF,
                                                          .context_value = 0 };
            hw::bootup::setBootRequest(app_request);
            NVIC_SystemReset();
        }
        else if (command.std_id == (boot_config.BOARD_HIGHBITS | GO_TO_BOOT))
        {
            // Restart bootloader update state when receiving a GO_TO_BOOT command.
            update_in_progress = false;
            current_address    = reinterpret_cast<uint32_t>(&__app_metadata_start__);
        }
        else
        {
            LOG_ERROR("got stdid %X", command.std_id);
        }
    }
}

[[noreturn]] void bootloader::runTickTask(config &boot_config)
{
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!update_in_progress)
        {
            // Broadcast a message at 1Hz so we can check status over CAN.
            io::CanMsg status_msg{};
            status_msg.std_id         = boot_config.BOARD_HIGHBITS | STATUS_10HZ_ID_LOWBITS;
            status_msg.dlc            = 5;
            status_msg.data.data32[0] = boot_config.GIT_COMMIT_HASH;
            status_msg.data.data8[4] = (uint8_t)(static_cast<uint8_t>(boot_status) << 1) | boot_config.GIT_COMMIT_CLEAN;
            boot_config.can_tx_queue.push(status_msg);
        }

        boot_config.boardSpecific_tick();

        start_ticks += 100; // 10Hz tick
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] void bootloader::runCanTxTask(config &boot_config)
{
    for (;;)
    {
        const io::CanMsg tx_msg = boot_config.can_tx_queue.pop().value();
#if defined(STM32H733xx) || defined(STM32H562xx)
        (void)boot_config.fdcan_handle.fdcan_transmit(tx_msg);
#elif defined(STM32F412Rx)
        (void)boot_config.can_handle.can_transmit(tx_msg);
#endif
    }
}
