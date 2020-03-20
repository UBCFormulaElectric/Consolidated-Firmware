/**
 * @brief Shared library with callback function for HAL library
 * @note  Include this header file in main.h, above the auto-generated function
 *        prototype `void ErrorHandler(void)`. Then, fill in the function
 *        definition in main.c as if the function prototype is
 *        `void ErrorHandler(char *file, uint32_t line)`
 */
#ifndef SHARED_ASSERT_H
#define SHARED_ASSERT_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <assert.h>

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
#if defined(__arm__)
#define shared_assert(expr) \
    ((expr) ? (void)0U      \
            : Io_SharedAssert_AssertFailed((char *)__FILE__, __LINE__, #expr))
#elif defined(__x86_64__) || defined(__i386__)
#define shared_assert(expr) assert(expr)
#endif

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Callback function for when shared_assert() fails to assert as true
 * @param file_path: File where the assert failed.
 * @param line: Line number where the assert failed.
 * @param expr: Expression that failed to be asserted as true.
 */
void Io_SharedAssert_AssertFailed(char *file_path, uint32_t line, char *expr);

#endif // SHARED_ASSERT_H
