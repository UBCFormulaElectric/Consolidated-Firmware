#pragma once

#include <cmath>
#include <span>

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) sizeof(array_pointer) / sizeof(array_pointer[0])

template <typename T, typename... U> [[nodiscard]] inline constexpr T MIN_OF(const T x, const U... y)
{
    T result = x;
    ((result = std::min(result, y)), ...);
    return result;
}

template <typename T> [[nodiscard]] inline constexpr T SQUARE(const T x)
{
    return x * x;
}

template <typename T> [[nodiscard]] inline constexpr bool IS_IN_RANGE(const T min, const T max, const T x)
{
    return (x > min) && (x < max);
}

template <typename T> [[nodiscard]] inline constexpr bool SIGN(const T x)
{
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

/* @brief Extract the basename from a file path */
#ifdef _MSC_VER
constexpr const char *filename_only(const char *path)
{
    const char *last_slash = path;
    for (const char *it = path; *it != '\0'; ++it)
    {
        if (*it == '/' || *it == '\\')
        {
            last_slash = it + 1;
        }
    }
    return last_slash;
}
#define __BASENAME__(x) filename_only(x)
#else
#define __BASENAME__(path) (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)
#endif

// Extra guard because HAL defines the same macro
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

#define EMPTY_STRUCT(name)     \
    typedef struct             \
    {                          \
        unsigned char _unused; \
    } name;

#ifdef __cplusplus
#define CFUNC extern "C"
#define NORET [[noreturn]]
#else
#define CFUNC
#define NORET _Noreturn
#endif

#define forever for (;;)

#ifdef TARGET_EMBEDDED
/* @brief Set a software breakpoint if debugger is connected */
#define BREAK_IF_DEBUGGER_CONNECTED()                     \
    if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) \
    {                                                     \
        __asm__ __volatile__("bkpt #0");                  \
    }
#elif TARGET_TEST
#if defined(_WIN32)
#include <windows.h>
#include <intrin.h>
#define IS_DEBUGGER_PRESENT() (IsDebuggerPresent() != 0)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__APPLE__) || defined(__linux__)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
static inline int IS_DEBUGGER_PRESENT(void)
{
#if defined(__APPLE__)
    // macOS: use sysctl to detect debugger
    int               mib[4];
    struct kinfo_proc info;
    size_t            size = sizeof(info);
    info.kp_proc.p_flag    = 0;

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();
    sysctl(mib, 4, &info, &size, NULL, 0);
    return ((info.kp_proc.p_flag & P_TRACED) != 0);
#elif defined(__linux__)
    // Linux: read /proc/self/status
    FILE *f = fopen("/proc/self/status", "r");
    if (!f)
        return 0;
    char line[256];
    int  debugger_present = 0;
    while (fgets(line, sizeof(line), f))
    {
        if (strncmp(line, "TracerPid:", 10) == 0)
        {
            int tracer_pid   = atoi(line + 10);
            debugger_present = (tracer_pid != 0);
            break;
        }
    }
    fclose(f);
    return debugger_present;
#else
    return 0; // Fallback: unknown platform
#endif
}

#define DEBUG_BREAK() raise(SIGTRAP)

#else
#define IS_DEBUGGER_PRESENT() 0
#define DEBUG_BREAK()
#endif

#define BREAK_IF_DEBUGGER_CONNECTED() \
    {                                 \
        if (IS_DEBUGGER_PRESENT())    \
            DEBUG_BREAK();            \
    }
#else
#error "TARGET_EMBEDDED or TARGET_TEST must be defined"
#endif
#undef ERROR
