//-- Includes ------------------------------------------------------------------
#include "profiler.h"

//-- Private functions prototypes ----------------------------------------------
static __INLINE void dwt_init(void);
static __INLINE void dwt_deinit(void);
static __INLINE uint32_t dwt_get_counter(void);

//-- Private variables ---------------------------------------------------------
extern uint32_t SystemCoreClock;

static struct
{
    const char* name;
    uint32_t start_time;
    uint32_t event_time[PROFILER_EVENTS_MAX];
    const char* event_name[PROFILER_EVENTS_MAX];
    uint32_t event_count;
} profiler;

//-- Private functions ---------------------------------------------------------
static __INLINE void dwt_init()
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static __INLINE void dwt_deinit()
{
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
}

static __INLINE uint32_t dwt_get_counter()
{
    return DWT->CYCCNT;
}

//-- Exported functions --------------------------------------------------------
void profiler_start(const char* name)
{
    profiler.name = name;
    profiler.event_count = 0;

    dwt_init();

    profiler.start_time = dwt_get_counter();
}

void profiler_event(const char* event)
{
    if (profiler.event_count < PROFILER_EVENTS_MAX) {
        profiler.event_time[profiler.event_count] = dwt_get_counter();
        profiler.event_name[profiler.event_count] = event;
        profiler.event_count++;
    }
}

void profiler_stop(void)
{
    uint32_t ticks_per_1us;
    uint32_t prev_time_us;
    uint32_t prev_time_ticks;
    uint32_t timestamp_us;
    uint32_t timestamp_ticks;
    uint32_t delta_us;
    uint32_t delta_ticks;

    ticks_per_1us = SystemCoreClock / 1000000;

    SharedLogging_Printf("Profiling \"%s\" sequence:\n"
           "--Event-----------------------|-timestamp_us-|----delta_us---|-timestamp_ticks-|----delta_ticks---|" PROFILER_ENDL,
           profiler.name);
    prev_time_us = 0;
    prev_time_ticks = 0;

    for (uint32_t i = 0; i < profiler.event_count; i++) {
        timestamp_ticks = profiler.event_time[i] - profiler.start_time;
        timestamp_us = timestamp_ticks / ticks_per_1us;
        delta_us = timestamp_us - prev_time_us;
        delta_ticks = timestamp_ticks - prev_time_ticks;
        prev_time_us = timestamp_us;
        prev_time_ticks = timestamp_ticks;
        SharedLogging_Printf("%-30s: %9d us | +%9d us | %9d ticks | +%9d ticks |" PROFILER_ENDL,
               profiler.event_name[i], (int)timestamp_us, (int)delta_us, (int)timestamp_ticks, (int)delta_ticks);
    }
    SharedLogging_Printf(PROFILER_ENDL);
    profiler.event_count = 0;
    dwt_deinit();
}
