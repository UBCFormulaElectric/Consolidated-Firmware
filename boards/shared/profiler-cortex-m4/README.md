## Simple code profiler for ARM Cortex-M4 MCUs

Based on [Catethysis profiler](https://catethysis.ru/profiler/).

### Features

- can be used with any Cortex-M4 MCU (Cortex-M3 will work too)
- based on Data Watchpoint and Trace (DWT) cycle counter (DWT->CYCCNT)

Output example:
```
Profiling "math.h" sequence:
--Event-----------------------|-timestamp_us-|----delta_us---|-timestamp_ticks-|----delta_ticks---|
sinf() start                  :         0 us | +        0 us |        46 ticks | +       46 ticks |
sinf() end                    :         1 us | +        1 us |       334 ticks | +      288 ticks |
tanf() start                  :         2 us | +        1 us |       375 ticks | +       41 ticks |
tanf() end                    :         4 us | +        2 us |       743 ticks | +      368 ticks |
```

### Examples

Profiler tested on STM32F4-Disco board.
printf() via USART2 at PA2 (115200, 8 data bit, 1 start bit, 1 stop bit, no parity).

```
int main(void)
{
    profiler_start("math.h");

    profiler_event("sinf() start");
    sinf(1.45f);
    profiler_event("sinf() end");

    profiler_event("tanf() start");
    tanf(1.0f);
    profiler_event("tanf() end");

    profiler_stop();

    while (1) {
    }
}
```

### Notes

- counter overflow is not handled
- counter use core frequency
- printf() should be inited before profiler used

### Porting

Simple replace MCU define in ```profiler.h```.

```
//-- Includes ------------------------------------------------------------------
#include "stm32f4xx.h"  // replace it
#include <stdint.h>
#include <stdio.h>
```
