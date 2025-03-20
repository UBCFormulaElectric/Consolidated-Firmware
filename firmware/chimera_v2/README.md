# Chimera V2
Control UBC Formula Electric STM boards over USB with Python, and talk to them over CAN.

![Chimera Architecture](../../images/chimera/chimera_v2_high_res.drawio.png)

*For Python API docs, see [`./docs`](./docs)*

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

If you would like to use the `load_bank` or `power_supply` modules, you will also need to install the [VISA drivers](https://www.ni.com/en/support/downloads/drivers/download.ni-visa.html).

```python
import power_supply
import time

# Config.
CHANNEL = 2
CURRENT = 1.51

# Setup supply.
supply = power_supply.DP821A()
print(supply.get_id())

# Output current for 5s.
supply.set_current(CURRENT, CHANNEL)
supply.enable_output(CHANNEL)
time.sleep(5)

# Measure voltage and current over channel 2 before disabling output.
current = supply.measure_current(CHANNEL)
voltage = supply.measure_voltage(CHANNEL)
print(f"Voltage: {voltage} V, Current: {current} A")

supply.disable_output()
```

*From [`./examples/power_supply.py`](./examples/power_supply.py)*

## Platform Instructions
### For Mac Users
Make sure you run your Chimera V2 scripts with `sudo`, ie. `sudo python ./examples/f4dev_blinky.py`.

### For Windows Users
On Windows, you will need to install Zadig. Go to [https://zadig.akeo.ie/](https://zadig.akeo.ie/), and download the latest version.

When you open Zadig, you should see the following,

![Zadig 1](../../images/chimera/zadig/1.png)

With the board plugged in over USB, click *List All Devices*. This will populate the central dropdown.

![Zadig 2](../../images/chimera/zadig/2.png)

Select the STM32 device on the dropdown (will be named after the board you are connected to).

![Zadig 3](../../images/chimera/zadig/3.png)

Then, in the menu pointed to by the green arrow, select *libusb-win32*. You will have to use the up and down buttons near the box to cycle through your options.

![Zadig 4](../../images/chimera/zadig/4.png)

Then click Replace Driver.

![Zadig 5](../../images/chimera/zadig/5.png)

## User Docs
For users of this package, checkout [`./docs`](./docs).

## Development
Chimera V2 has two ends - board side and Python side. Board side code can be found at [`io_chimera_v2.h`](../shared/src/io/io_chimera_v2.h)/[`io_chimera_v2.c`](../shared/src/io/io_chimera_v2.c). Python side code is in the [same directory as this README](.).

### Configuring a New Device
Open up the `.ioc` file of your board in STM32CubeMX, and search fot the `USB_DEVICE` setting. Set the device class to `Communication Device Class (Virtual Port Com)`.

![USB_DEVICE configuration](../../images/chimera/stm/usb_device.png).

Then, under the *Device Descriptor* Panel, set `MANUFACTURER_STRING` to `ubc_formula_electric`, and `PRODUCT_STRING` to the name of your board (we are enforcing a no-caps style here).

![USB ID configuration](../../images/chimera/stm/usb_id.png).

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

Also make sure to explicitly enable I2C/SPI/ADC, even if your board does not have them (Chimera V2 relies on them).

Eg. For the SSM
```cmake
target_compile_definitions("ssm.elf" PRIVATE HAL_ADC_MODULE_ENABLED HAL_I2C_MODULE_ENABLED HAL_SPI_MODULE_ENABLED)
```

Try to build. This will regenerate all the STM32 code, with USB files setup.

Now find the generated `usbd_cdc_if.c` file. At the top, include [`hw_usb.h`](../shared/src/hw/hw_usb.h) and [`io_log.h`](../shared/src/io/io_log.h).
```c
/* USER CODE BEGIN INCLUDE */
#include "hw_usb.h"
#include "io_log.h"
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
    if (!hw_usb_pushRxMsgToQueue(Buf, *Len))
        LOG_ERROR("Error encountered pushing USB RX Message to queue.");

    return (USBD_OK);
    /* USER CODE END 6 */
}
```

In the `main.c` of your board, you will need to initialize the `hw_usb` RX queue. To do this, go to where you init all your peripherals, and call `hw_usb_init()` (you will need to include [`hw_usb.h`](../shared/src/hw/hw_usb.h) in `main.c` well).

> Warning! `hw_usb_init()` creates a CMSIS OS Message Queue under the hood. 
> Make sure to initialize after `osKernelInitialize` is called.

Next, you need to setup the protobuf messages for your board. Go to [`./proto`](./proto), and create a new `.proto` file named after your board. It should look like this:

```proto
syntax = "proto3";

package name_of_board;

enum GpioNetName {
  GPIO_NET_NAME_UNSPECIFIED = 0;
}

enum AdcNetName {
  ADC_NET_NAME_UNSPECIFIED  = 0;
}

enum I2cNetName {
  I2C_NET_NAME_UNSPECIFIED = 0;
}

enum SpiNetName {
  SPI_NET_NAME_UNSPECIFIED = 0;
}
```

Now go into [`./proto/shared.proto`](./proto/shared.proto), and import your board's proto at the top of the file. In `GpioNetName`, `AdcNetName`, `SpiNetName`, and `I2cNetName`, add your new board-specific enums.

Eg. With the F4Dev configued:

```proto
...

import "f4dev.proto";

...

message GpioNetName {
  oneof name {
    ...
    f4dev.GpioNetName f4dev_net_name = 1;
  }
}

...

message AdcNetName {
  oneof name {
    ...
    f4dev.AdcNetName f4dev_net_name = 1;
  }
}

...

message I2cNetName {
  oneof name {
    ...
    f4dev.I2cNetName f4dev_net_name = 1;
  }
}

...

message I2cNetName {
  oneof name {
    ...
    f4dev.SpiNetName f4dev_net_name = 1;
  }
}

```

To generate the Protobuf Python libraries, run [`./scripts/generate_proto.sh`](./scripts/generate_proto.sh).

The Protobuf C libraries will be exposed statically as `chimera_v2_proto_cm7` (H7s) and `chimera_v2_proto_cm4` (F4s). Make sure to link the appropriate library into your binary.

Next, we need to add the board to [`./chimera_v2.py`](./chimera_v2.py). To do this, import the appropriate autogenerated Python library, `proto_autogen.name_of_board_pb2`. Then, at the bottom, add a class for your board.

Eg. for the F4 Dev board,
```python
class F4Dev(_Board):
    def __init__(self) -> None:
        """Create an interface to an F4Dev board."""

        super().__init__(
            usb_device=_UsbDevice(product="f4dev"),
            gpio_net_name="f4dev_net_name",
            adc_net_name="f4dev_net_name",
            i2c_net_name="f4dev_net_name",
            spi_net_name="f4dev_net_name",
            board_module=proto_autogen.f4dev_pb2,
        )
```

Note: the `product` field in the `_UsbDevice` initializer is the same as you configured in STM32CubeMX.

The next step is to configure Chimera on the board. Open [`io_chimera_v2.c`](../shared/src/io/io_chimera_v2.c). Modify `io_chimera_v2_getGpio`, `io_chimera_v2_getAdc`, `io_chimera_v2_getSpi`, and `io_chimera_v2_getI2c` with a branch corresponding to your board.

eg.
```c
...

// Convert a given GpioNetName to a GPIO pin.
static const Gpio *io_chimera_v2_getGpio(const GpioNetName *net_name)
{
    ...

    if (net_name->which_name == GpioNetName_f4dev_net_name_tag)
        return id_to_gpio[net_name->name.f4dev_net_name];

    ...
}

// Convert a given AdcNetName to an ADC channel pin.
static const AdcChannel *io_chimera_v2_getAdc(const AdcNetName *net_name)
{
    ...

    if (net_name->which_name == AdcNetName_f4dev_net_name_tag)
        return id_to_adc[net_name->name.f4dev_net_name];

    ...
}

// Convert a given I2C enum to an I2C device.
static const I2cDevice *io_chimera_v2_getI2c(const I2cNetName *net_name)
{
    ...

    if (net_name->which_name == AdcNetName_f4dev_net_name_tag)
        return id_to_i2c[net_name->name.f4dev_net_name];

    ...
}

// Convert a given SPI enum to a SPI device.
static const SpiDevice *io_chimera_v2_getSpi(const SpiNetName *net_name)
{
    ...

    if (net_name->which_name == SpiNetName_f4dev_net_name_tag)
        return id_to_spi[net_name->name.f4dev_net_name];

    ...
}


...
```

Next, we need to add Chimera-controllable peripherals to the board. Open up the `.proto` file you made named after your board (ie. [`./proto/f4dev.proto`](./proto/f4dev.proto)). Add an item to the enum for whatever type of peripheral you want to set.

Eg. For a GPIO on the f4dev,
```proto
enum GpioNetName {
  GPIO_NET_NAME_UNSPECIFIED = 0;
  GPIO_6 = 1; // < Add a line like this.
}
```

Now run [`./scripts/generate_proto.sh`](./scripts/generate_proto.sh) again to generate the python proto libraries. Also try to build binaries for your board.

To capture these peripherals on the board side, we need to create a mapping from protobuf names to the actual GPIO pins/ADC Channels. In the `io` level of your board, create files called `io_chimeraConfig_v2.h`/`io_chimeraConfig_v2.c`.

`io_chimeraConfig_v2.h` should look like this,

```c
#pragma once
#include "hw_gpio.h"
#include "hw_adc.h"
#include "hw_i2c.h"

extern const Gpio       *id_to_gpio[];
extern const AdcChannel *id_to_adc[];
extern const I2cDevice  *id_to_i2c[];
extern const SpiDevice  *id_to_spi[];
```

`io_chimeraConfig_v2.c` should extern declare tables mapping from the protobuf net name enums, to the actual peripherals.

Eg. GPIO 6 on the F4 Dev board,
```c
#include "f4dev.pb.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_adc.h"
#include "hw_adcs.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"
#include "hw_spi.h"
#include "hw_spis.h"

const Gpio *id_to_gpio[] = {
    [f4dev_GpioNetName_GPIO_6] = &gpio_6,
    ...
};

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [f4dev_AdcNetName_ADC_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure I2Cs.
const I2cDevice *id_to_i2c[] = { [f4dev_I2cNetName_I2C_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure SPIs.
const I2cDevice *id_to_spi[] = { [f4dev_SpiNetName_SPI_NET_NAME_UNSPECIFIED] = NULL };
```

> Note: Because chimera rellies on the existance of all of these peripherals, you will need a `hw_i2cs.h`, `hw_gpios.h`, `hw_spis.h`, and `hw_i2cs.h` header in every board. This will likely mean you have to add the i2c HAL headers, see other boards on how to do this.
>
> If your board has none of a given peripheral, (ie. no ADC channels, or no I2C devices), you can provide an id_to_peripheral table as follows:
>
>```c
> // TODO: Configure adcs.
> const AdcChannel *id_to_adc[] = { [f4dev_AdcNetName_ADC_NET_NAME_UNSPECIFIED] = NULL };
> 
> // TODO: Configure I2Cs.
> const I2cDevice *id_to_i2c[] = { [f4dev_I2cNetName_I2C_NET_NAME_UNSPECIFIED] = NULL };
> 
> // TODO: Configure SPIs.
> const SpiDevice *id_to_spi[] = { [f4dev_SpiNetName_SPI_NET_NAME_UNSPECIFIED] = NULL };
>```
>
> Since a lot of boards might not have I2C devices, it is sufficent to supply a `hw_i2cs.h` as follows:
> ```c
> #pragma once
> 
> #include "hw_i2c.h"
> ```
>
> And `hw_i2cs.c` as follows:
> ```c
> #include "hw_i2cs.h"
> #include "main.h"
> 
> I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
> {
>     return NULL;
> }
> ```
> 
> 
> For SPI configs for boards without SPI devices, we can do something very similar.
>
> 
> `hw_spis.h`:
> ```c
> #pragma once
> 
> #include "hw_spi.h"
> ```
>
> `hw_spis.c`:
> ```c
> #include "hw_spis.h"
> #include "main.h"
> 
> SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
> {
>     return NULL;
> }

> ```

We can finally run chimera. Include the shared `io_chimera_v2.h` library, and run `io_chimera_v2_mainOrContinue` in your desired task (You need to also include `shared.pb.h` and `io_chimeraConfig_v2.h` at the top of your file).

Eg. For the f4dev,
```c
    io_chimera_v2_mainOrContinue(
        GpioNetName_f4dev_net_name_tag, id_to_gpio, 
        AdcNetName_f4dev_net_name_tag, id_to_adc,
        I2cNetName_f4dev_net_name_tag, id_to_i2c
        SpiNetName_f4dev_net_name_tag, id_to_spi
    );
```

Note: you might want to use the provided `io_chimera_v2_enabled` flag to disable other non-chimera jobs.

`io_chimera_v2_mainOrContinue` will skip running if no USB is plugged in on boot. It should be called in a loop in one of the tasks (eg. 100 Hz).

### Development Environment
For development, start by changing to [the directory of this README](.), and installing it as a pip package. 

From the root of the project,
```sh
cd ./firmware/chimera_v2
pip install .
```

This will install a version of `chimera_v2` with all your local changes.

Open the examples directory, and write a quick example script to test Chimera out! See [`./examples/f4dev_blinky.py`](./examples/f4dev_blinky.py) for an example.

Chimera is now ready to go 🥳!

### Generated Files
To generate Proto libraries for Chimera, you can run [`./scripts/generate_proto.sh`](./scripts/generate_proto.sh). To generate user docs, you can run [`./scripts/generate_docs.sh`](./scripts/generate_docs.sh). Look inside both scripts for details on how they operate.

> Note: Generating docs will reinstall/uninstall chimera_v2 on your system. It's a kink in the system to be ironed out later.

### USB Drivers
Chimera V2 (unlike the original Chimera) is based on USB. The drivers can be found in [`hw_usb.h`](../shared/src/hw/hw_usb.h)/[`hw_usb.c`](../shared/src/hw/hw_usb.c).

We use the Communications Device Class (CDC) for our devices. This class takes 64 byte bulk input and output, which we convert to a continuous stream using a CMSIS `osMessageQueue` on the device side, and a buffer string on the Python side.

`hw_usb` exposes a `hw_usb_pushRxMsgToQueue` method, that must be called from the `CDC_Receive_FS` method in the STM32 autogenerated `usb_cdc_if.c` file.

> Note: F4s and H7s have different exposed USB APIs, since H7s support HS (high speed) and F4s support FS (full speed). If the receive callback on the F4 is called `CDC_Receive_FS`, it will be called `CDC_Receive_HS` on the H7.

### Linting
See [`./scripts/lint.sh`](./scripts/lint.sh).