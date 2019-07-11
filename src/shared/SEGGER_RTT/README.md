### SEGGER Real-Time Transfer (RTT)
To use this library:
1. Download **J-Link Software and Documentation pack** from [here](https://www.segger.com/downloads/jlink/)
1. Configure `JLinkRTTViewer` works and can connect to J-Link debugger
    - Open `JLinkRTTViewer`
    - Select `USB` under `Connection to J-Link`
    - Select the appropriate ST microcontroller (e.g. `STM32F302C8) under `Specify Target Device`
    - Select `SWD` and `4000kHz` under `Target Interface & Speed`
    - Select `Auto Detection` under `RTT Control Block`
1. `#include "SEGGER_RTT.h"` to use `SEGGER_RTT_printf()`:
    ```  
    static uint32_t i = 0;
    for (;;)
    {
        SEGGER_RTT_printf(0, "Test message from SEGGER RTT:%u\r\n", i);
        osDelay(1);
    }
    ```  
1. In `JLinkRTTViewer`, you should see your debug print messages:
![image](https://user-images.githubusercontent.com/16970019/61187626-b6f62c80-a628-11e9-9f7f-6d02f3082de0.png)
1. Happy RTT-ing!
