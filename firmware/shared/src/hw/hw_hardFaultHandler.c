#include "hw_hardFaultHandler.h"
#include "hw_hal.h"
#include "hw_utils.h"
#include "main.h"

#if !(defined(LED_GPIO_Port) && defined(LED_Pin))
#warning "LED_GPIO_Port and LED_Pin are not defined. Please define them in main.h"
#endif

void hw_hardFaultHandler_init(void)
{
    // Div-by-zero exception is disabled by default and must be enabled manually
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
}

// TODO (Issue: #400): Remove #pragma when we are using the variables to
// transmit the data over CAN
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void                   hw_hardFaultHandler_logInfo(uint32_t *fault_stack)
{
    // Registers pushed onto the stack frame before entering hard fault handler
    volatile uint32_t stacked_r0;
    volatile uint32_t stacked_r1;
    volatile uint32_t stacked_r2;
    volatile uint32_t stacked_r3;
    volatile uint32_t stacked_r12;
    volatile uint32_t stacked_lr;  // Link register
    volatile uint32_t stacked_pc;  // Program counter
    volatile uint32_t stacked_psr; // Program status register

    // Fault exception registers with information about the exception
    volatile uint32_t cfsr;
    volatile uint32_t hfsr;
    volatile uint32_t afsr;
    volatile uint32_t bfar;  // Only valid if BFARVALID is set
    volatile uint32_t mmfar; // Only valid if MMARVALID is set

    // The register values are read from the stack based on the order in which
    // registers are pushed onto the stack frame upon entering an exception
    stacked_r0  = fault_stack[0];
    stacked_r1  = fault_stack[1];
    stacked_r2  = fault_stack[2];
    stacked_r3  = fault_stack[3];
    stacked_r12 = fault_stack[4];
    stacked_lr  = fault_stack[5];
    stacked_pc  = fault_stack[6];
    stacked_psr = fault_stack[7];

    cfsr  = SCB->CFSR;
    hfsr  = SCB->HFSR;
    afsr  = SCB->AFSR;
    bfar  = SCB->BFAR;
    mmfar = SCB->MMFAR;

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
#if defined(LED_GPIO_Port) && defined(LED_Pin)
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        for (int i = 0; i < 3000000; i++)
        {
            __ASM("nop");
        }
#endif
    };
}
#pragma GCC diagnostic pop
