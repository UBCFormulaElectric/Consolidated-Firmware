#pragma once

#include <cmath>
#include <cstdint>

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) sizeof(array_pointer) / sizeof(array_pointer[0])

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

// Extra guard because HAL defines the same macro
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

#define EMPTY_STRUCT(name)     \
    typedef struct             \
    {                          \
        unsigned char _unused; \
    } name;

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) sizeof(array_pointer) / sizeof(array_pointer[0])

constexpr auto MIN(auto a, auto b) { return (a < b) ? a : b; }
constexpr auto MAX(auto a, auto b) { return (a > b) ? a : b; }
constexpr auto CLAMP(auto x, auto min, auto max) { return MAX(MIN(x, max), min); }
constexpr auto SQUARE(auto x) { return x * x; }
constexpr auto SQRT(auto x) { return static_cast<decltype(x)>(sqrt(static_cast<double>(x))); }
constexpr auto MIN3(auto x, auto y, auto z) { return MIN(MIN(x, y), z); }
constexpr auto MIN4(auto w, auto x, auto y, auto z) { return MIN(MIN(MIN(w, x), y), z); }
constexpr auto CLAMP_TO_ONE(auto x) { return (x <= 0) ? 1 : ((x > 1) ? 1 : x); }
constexpr bool IS_BIT_SET(auto input, uint8_t bit) { return (bool)((uint8_t)(input >> bit) & 1U); }
constexpr auto SET_BIT_HIGH(auto input, uint8_t bit) { return input | (1U << bit); }
constexpr auto SET_BIT_LOW(auto input, uint8_t bit) { return input & ~(1U << bit); }
constexpr bool APPROX_EQUAL(auto a, auto b, auto threshold) { return (std::fabs(a - b) < threshold); }
constexpr bool APPROX_EQUAL_FLOAT(float a, float b, float threshold) { return (std::fabsf(a - b) < threshold); }
constexpr bool IS_IN_RANGE(auto min, auto max, auto val) { return (val > min) && (val < max); }
constexpr uint16_t CHANGE_WORD_ENDIANNESS(uint16_t word) { return static_cast<uint16_t>(((word >> 8U) | (word << 8U))); }

constexpr uint32_t MAX_4_BITS_VALUE  = static_cast<uint32_t>(15);
constexpr uint32_t MAX_6_BITS_VALUE  = static_cast<uint32_t>(63);
constexpr uint32_t MAX_8_BITS_VALUE  = static_cast<uint32_t>(255);
constexpr uint32_t MAX_10_BITS_VALUE = static_cast<uint32_t>(1023);
constexpr uint32_t MAX_11_BITS_VALUE = static_cast<uint32_t>(2047);
constexpr uint32_t MAX_12_BITS_VALUE = static_cast<uint32_t>(4095);
constexpr uint32_t MAX_16_BITS_VALUE = static_cast<uint32_t>(65535);
constexpr uint32_t MAX_32_BITS_VALUE = static_cast<uint32_t>(4294967295);

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