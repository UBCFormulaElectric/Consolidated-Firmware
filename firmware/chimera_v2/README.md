# Chimera V2
Control UBC Formula Electric STM boards over USB with Python.

![Chimera Architecture](../../images/chimera/chimera_v2_high_res.drawio.png)

*For Python API docs, see [`./docs/chimera_v2.md`](./docs/chimera_v2.md)*

## Quick Start
To install the `chimera_v2` Python package, run
```sh
pip install 'git+https://github.com/UBCFormulaElectric/Consolidated-Firmware.git@master#egg=chimera-v2&subdirectory=firmware/chimera_v2'
```

To output a quick 10Hz square wave over GPIO 6 of the F4Dev board, 

```python
import time
import chimera_v2

# Config.
RATE_HZ = 10.0
DUTY_CYCLE = 0.5
LED_NET_NAME = "GPIO_6"

# Init device.
chimera_device = chimera_v2.F4Dev()

# Main loop.
while True:
    print("On")
    chimera_device.gpio_write(LED_NET_NAME, True)
    time.sleep(DUTY_CYCLE / RATE_HZ)

    print("Off")
    chimera_device.gpio_write(LED_NET_NAME, False)
    time.sleep((1.0 - DUTY_CYCLE) / RATE_HZ)
```
*From [`./examples/f4dev_blinky.py`](./examples/f4dev_blinky.py)*

## For Windows Users
On Windows, you will need to install Zadig. Go to [https://zadig.akeo.ie/](https://zadig.akeo.ie/), and download the latest version.

When you open Zadig, you should see the following,

![Zadig 1](../../images/chimera/zadig/1.png)

With the board plugged in over USB, click *List All Devices*. This will populate the central dropdown.

![Zadig 2](../../images/chimera/zadig/2.png)

Select the STM32 device on the dropdown.

![Zadig 3](../../images/chimera/zadig/3.png)

Then, in the menu pointed to by the green arrow, select *libusb-win32*. You will hae to use the up and down buttons near the box to cycle through your options.

![Zadig 4](../../images/chimera/zadig/4.png)

Then click Replace Driver.

![Zadig 5](../../images/chimera/zadig/5.png)

## User Docs
For users of the `chimera_v2` Python package, checkout [`./docs/chimera_v2.md`](./docs/chimera_v2.md).

## Development
Chimera V2 has two ends - board side and Python side. Board side code can be found at [`io_chimera_v2.h`](../shared/src/io/io_chimera_v2.h)/[`io_chimera_v2.c`](../shared/src/io/io_chimera_v2.c). Python side code is in the [same directory as this README](.).

### Configuring a New Device
Open up the `.ioc` file of your board in STM32CubeMX, and search fot the `USB_DEVICE` setting. Set the device class to `Communication Device Class (Virtual Port Com)`.

![USB_DEVICE configuration](../../images/chimera/stm/usb_device.png)

Then search for the `USB_OTG_FS` (or `USB_OTG_HS` on H7s) setting. Set the mode to `Device_Only`, and Activate_VBUS to `Disable`.

![USB_OTG configuration](../../images/chimera/stm/usb_otg.png)

> *Note for H7s*, make sure Activate_SOF is unchecked, and Internal FS Phy is set to `DEVICE_ONLY`.
> ![USB_OTG H7 configuration](../../images/chimera/stm/usb_h7_otg.png)

You should see two pins configured on the chip for USB,

![USB pins](../../images/chimera/stm/usb_pins.png)

Next, we need to make sure we build our binaries with all the USB dependencies. First, make sure that the call to `stm32f412rx_cube_library`/`stm32h733xx_cube_library` has `TRUE` as it's last argument (this flag enables USB).

Eg.
```cmake
stm32h733xx_cube_library(
    "h7dev_stm32cube"
    "${STM32_HAL_SRCS}"
    "${SYSCALLS}"
    "${MD5_LOCATION}"
    TRUE
)
```

We also need to add to the passed HAL sources files required by USB. 

For the H7, make sure you have
```cmake
"stm32h7xx_hal_pcd.c"
"stm32h7xx_hal_pcd_ex.c"
"stm32h7xx_ll_usb.c"
```

For the F4, make sure you have
```cmake
"stm32f4xx_hal_pcd.c"
"stm32f4xx_hal_pcd_ex.c"
"stm32f4xx_ll_usb.c"
```

Eg. From the F4Dev,

```cmake
...

set(STM32_HAL_SRCS
        ...
        "stm32f4xx_hal_pcd.c"
        "stm32f4xx_hal_pcd_ex.c"
        "stm32f4xx_ll_usb.c"
)

stm32f412rx_cube_library(
        "f4dev_stm32cube"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${MD5_LOCATION}"
        TRUE
)

...
```

Try to build. This will regenerate all the STM32 code, with USB files setup.

At the top of the file, you will need to add the following `#pragma` statements to kill diagnostics on this file.
```
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
```

It should look something like this:
```c
/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_cdc_if.c
 * @version        : v1.0_Cube
 * @brief          : Usb device for Virtual Com Port.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

// Ignore sign conversion errors.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
/* USER CODE END Header */
```

Now find the generated `usbd_cdc_if.c` file. At the top, include [`hw_usb.h`](../shared/src/hw/hw_usb.h).
```c
/* USER CODE BEGIN INCLUDE */
#include "hw_usb.h"
/* USER CODE END INCLUDE */
```

Next, find the defenition of `CDC_Receive_FS`/`CDC_Receive_HS`. Add a call to `hw_usb_pushRxMsgToQueue(Buf, *Len);` in the body of the function.

Eg. from the F4Dev,
```c
static int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len)
{
    /* USER CODE BEGIN 6 */
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);

    // Hook to hw_usb.
    hw_usb_pushRxMsgToQueue(Buf, *Len);

    return (USBD_OK);
    /* USER CODE END 6 */
}
```

In the `main.c` of your board, you will need to initialize the `hw_usb` RX queue. To do this, go to where you init all your peripherals, and call `hw_usb_init()` (you will need to include [`hw_usb.h`](../shared/src/hw/hw_usb.h) in `main.c` well).

We can finally run chimera. Include the shared `io_chimera_v2.h` library, and run `io_chimera_v2_main` in your desired task.

> **TODO:** Curently, Chimera V2 is only setup for the dev boards. When Quintuna boards arrive, we will decide how we handle multi-task boards with watchdog.

### Generated Files
To generate Proto libraries for Chimera, you can run [`./scripts/generate_proto.sh`](./scripts/generate_proto.sh). To generate user docs, you can run [`./scripts/generate_docs.sh`](./scripts/generate_docs.sh). Look inside both scripts for details on how they operate.

### USB Drivers
Chimera V2 (unlike the original Chimera) is based on USB. The drivers can be found in [`hw_usb.h`](../shared/src/hw/hw_usb.h)/[`hw_usb.c`](../shared/src/hw/hw_usb.c).

We use the Communications Device Class (CDC) for our devices. This class takes 64 byte bulk input and output, which we convert to a continuous stream using a CMSIS `osMessageQueue` on the device side, and a buffer string on the Python side.

`hw_usb` exposes a `hw_usb_pushRxMsgToQueue` method, that must be called from the `CDC_Receive_FS` method in the STM32 autogenerated `usb_cdc_if.c` file.

eg. From the F4Dev board,
```c
static int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len)
{
    /* USER CODE BEGIN 6 */
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);

    // hook to hw_usb
    hw_usb_pushRxMsgToQueue(Buf, *Len);

    return (USBD_OK);
    /* USER CODE END 6 */
}
```

> Note: F4s and H7s have different exposed USB APIs, since H7s support HS (high speed) and F4s support FS (full speed). If the receive callback on the F4 is called `CDC_Receive_FS`, it will be called `CDC_Receive_HS` on the H7.

