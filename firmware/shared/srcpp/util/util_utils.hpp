#pragma once

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
#include <signal.h>
#include <unistd.h>

static inline int IS_DEBUGGER_PRESENT(void)
{
#if defined(__APPLE__)
    // macOS: use sysctl to detect debugger
#include <sys/types.h>
#include <sys/sysctl.h>
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