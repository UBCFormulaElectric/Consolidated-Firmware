#include "bootloader.h"
#include "hw_bootup.hpp"
#include "main.h"
#include "hw_hardFaultHandler.hpp"
#include "hw_flash.hpp"
#include "hw_utils.hpp"
#include "hw_hal.hpp"
#include "io_log.hpp"
#include "app_crc32.hpp"

#include "cmsis_gcc.h"
#include "cmsis_os.h"
#include <io_canMsg.hpp>
#include <io_queue.hpp>

#if defined(STM32H733xx) || defined(STM32H562xx)
#include "hw_fdcan.h"
#elif defined(STM32F412Rx)
#include "hw_can.h"
#else
#error "Please define what MCU is used"
#endif

void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_INFO("CAN TX OVERFLOW");
}
void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_INFO("CAN RX OVERFLOW");
}
void tx_overflow_clear_callback() {}
void rx_overflow_clear_callback() {}

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

enum class BootStatus : uint8_t
{
    BOOT_STATUS_APP_VALID = 0,
    BOOT_STATUS_APP_INVALID,
    BOOT_STATUS_NO_APP
};

static bool       update_in_progress;
static BootStatus boot_status;
static uint32_t   current_address;

io::queue<io::CanMsg, 128> can_tx_queue("CanTXQueue", tx_overflow_callback, tx_overflow_clear_callback);

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
    if (metadata->size_bytes > (uint32_t)&__app_code_start__)
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

void bootloader_preInit(void)
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
                                                  .context_value = 0 };
    hw::bootup::setBootRequest(app_request);
}

void bootloader_init(void)
{
    LOG_INFO("Bootloader reset!");
    // Initialize CAN
    // Board Specific
}

[[noreturn]] void bootloader_runInterfaceTask(void)
{
    forever
    {
        const io::CanMsg command = can_tx
    }
}
