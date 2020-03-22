#pragma once

#include <stdint.h>
#include <assert.h>

#if defined(__arm__)
#define shared_assert(expr) \
    ((expr) ? (void)0U      \
            : Io_SharedAssert_AssertFailed((char *)__FILE__, __LINE__, #expr))
#elif defined(__x86_64__) || defined(__i386__)
#define shared_assert(expr) assert(expr)
#endif

/**
 * @brief Callback function for when shared_assert() fails to assert as true
 * @param file_path: File where the assert failed.
 * @param line: Line number where the assert failed.
 * @param expr: Expression that failed to be asserted as true.
 */
void Io_SharedAssert_AssertFailed(char *file_path, uint32_t line, char *expr);
