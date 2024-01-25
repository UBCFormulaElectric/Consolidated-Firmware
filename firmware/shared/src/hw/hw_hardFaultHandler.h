/**
 * @brief Shared library with callback functions for hard-fault exceptions
 * @note  This library is inspired by the example code here:
 *        https://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html
 */
#pragma once

#include <stdint.h>

// Right before the the processor enters HardFault_Handler(), the following
// register values are pushed onto the stack: R0-R4, R12, LR, PC, PSR. These
// register values make up of what is called a "stack frame". The stack frame
// can be stored in either MSP (main stack pointer) or PSP (process stack
// pointer). The following assembly function determines which stack pointer was
// used was then hard fault occured, and passes it as an argument to
// hw_hardFaultHandler_LogInformation().
//
// Note: The MSP/PSP architecture is specific to the Cortex-M microcontroller
//       family.
#define hw_hardFaultHandler_handleFault()                                                                              \
    __asm volatile(" tst lr, #4                                \n"                                                     \
                   " ite eq                                    \n"                                                     \
                   " mrseq r0, msp                             \n"                                                     \
                   " mrsne r0, psp                             \n"                                                     \
                   " ldr r1, [r0, #24]                         \n"                                                     \
                   " b hw_hardFaultHandler_logInfo   \n");

/**
 * @brief Add the naked attributes to HardFault_Handler function declaration.
 * @note  The compiler will not generate prologue and epilogue sequences for
 *        naked functions because they are embedded assembly functions.
 */
__attribute__((naked)) void HardFault_Handler(void);

/**
 * @brief Enable the desired exceptions.
 * @note  Try to call this as early as possible to catch as many exceptions as
 *        possible.
 */
void hw_hardFaultHandler_init(void);

/**
 * @brief Log information that can help us identify what caused the hard fault.
 * @param fault_stack Pointer to the stack that was used when the hard fault
 *        occurred.
 */
void hw_hardFaultHandler_logInfo(uint32_t* fault_stack);
