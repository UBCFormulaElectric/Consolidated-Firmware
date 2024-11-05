#include "io_bootloader.h"
#include <stdint.h>
#include "cmsis_gcc.h"
#include "cmsis_os.h"
#include "hw_hal.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"

#define BOOT_CAN_START 1012

extern uint32_t __boot_code_start__;
extern uint32_t __boot_code_size__;

extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef  huart7;
extern TIM_HandleTypeDef   htim3;
extern UART_HandleTypeDef  huart2;
extern UART_HandleTypeDef  huart1;
extern UART_HandleTypeDef  huart3;
extern SD_HandleTypeDef    hsd1;

static void modifyStackPointerAndStartApp(const uint32_t *address)
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
    uint32_t boot_sp    = address[0];
    uint32_t boot_start = address[1];
#pragma GCC diagnostic pop
    __set_MSP(boot_sp);
    void (*boot_reset_handler)(void) = (void (*)(void))boot_start;
    boot_reset_handler(); // Call app's Reset_Handler, starting the app.

    // Should never get here!
    BREAK_IF_DEBUGGER_CONNECTED();
    for (;;)
    {
    }
}

void io_bootloader_checkBootMsg(CanMsg *msg)
{
    if (msg->std_id == BOOT_CAN_START)
    {
        // DeInit all peripherals and interupts
        HAL_ADC_Stop_IT(&hadc1);
        HAL_ADC_Stop_IT(&hadc3);
        HAL_ADC_DeInit(&hadc1);
        HAL_ADC_DeInit(&hadc3);
        HAL_TIM_Base_Stop_IT(&htim3);
        HAL_UART_Abort_IT(&huart1);
        HAL_UART_Abort_IT(&huart2);
        HAL_UART_Abort_IT(&huart3);
        HAL_UART_Abort_IT(&huart7);
        HAL_UART_DeInit(&huart1);
        HAL_UART_DeInit(&huart2);
        HAL_UART_DeInit(&huart3);
        HAL_UART_DeInit(&huart7);

        // ackknowledge the msg sent by boot
        CanMsg reply = { .std_id = BOOT_CAN_START, .dlc = 0 };
        io_can_pushTxMsgToQueue(&reply);

        modifyStackPointerAndStartApp(&__boot_code_start__);
    }
}
