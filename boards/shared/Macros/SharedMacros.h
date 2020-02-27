/**
 * @brief Shared helper macros
 */
#ifndef SHARED_MACROS_H
#define SHARED_MACROS_H

/******************************************************************************
 * Includes
 ******************************************************************************/

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
// clang-format off
#define NUM_ELEMENTS_IN_ARRAY(array_pointer) \
    (sizeof(array_pointer) / sizeof(array_pointer[0]))

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) \
    (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

/* @brief Set a software breakpoint if debugger is connected */
#define BREAK_IF_DEBUGGER_CONNECTED() \
if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) \
{ \
    __asm__ __volatile__ ("bkpt #0"); \
} \

#endif /* SHARED_MACROS_H */
