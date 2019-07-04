## Overview
## Peripheral Configuration Notes
*Please note that all mentions of "input" and "output" are from the perspective of the microcontroller*
1. **22XS4200 High-Side Switch (MC22XS4200.pdf Rev 5.0)**
   - **`IN0`, `IN1` (24V output):**
     - | Net Name        | Pin      | Desciption               |
       | --------------- | -------- | ------------------------ |
       | `PIN_DI_R`      |  `PA2`   | right drive inverter     |
       | `PIN_DI_L`      |  `PA3`   | left drive inverter      |
       | `PIN_AIR_SHDN`  |  `PA5`   | AIR Shutdown             |
       | `PIN_CAN`       |  `PA6`   | CAN/low voltage system   |
       | `PIN_AUX_2`     |  `PB2`   | auxiliary 1              |
       | `PIN_AUX_1`     |  `PB12`  | auxiliary 2              |
       | `PIN_COOLING`   |  `PC9`   | cooling pump             |
       | `PIN_EM`        |  `PA8`   | energy meter             |
     - These outputs directly control the switching state of each 24V channel - logic 0 turns off the channel and logic 1 turns on the channel.
     - During peripheral initialization, these outputs are asserted to low by default so we don't accidentally turn on a channel that was already short-circuit.
     - There is no current-limiting resistors so we use pull-down instead of push-pull
   - **`CSNS` (Current/Temperature Sense):**
     - | Net Name            | Pin     | Desciption               |
       | ------------------- | --------| ------------------------ |
       | `IS_AUX1_AUX2`      | `PB1`   | auxiliary 1 and 2                       |
       | `IS_EM_PUMP`        | `PB11`  | energy meter and cooling pump           |
       | `IS_DI_L_DI_R`      | `PA1`   | left/right driver inverter enable       |
       | `IS_GLV_AIR_SHDN`   | `PB0`   | CAN/low voltage system and AIR shutdown |
     - These ADC inputs read a current proportional to the channel’s output current or a voltage proportional to the temperature of the GND pin.
     - Note: Due to schematic error, `PB10` had to be reworked to connect to `PB1`, and `PC8` to `PB11`
   - **`SYNC` (Current Sense Ready):**
     - | Net Name                | Pin     | Desciption               |
       | ----------------------- | --------| ------------------------ |
       | `CUR_SYNC_AUX1_AUX2`    | `PC6`   | auxiliary 1 and 2                       |
       | `CUR_SYNC_EM_PUMP`      | `PB10`  | energy meter and cooling pump           |
       | `CUR_SYNC_DI_L_DI_R`    | `PA0`   | left/right driver inverter enable       |
       | `CUR_SYNC_GLV_AIR_SHDN` | `PA4`   | CAN/low voltage system and AIR shutdown |
     - These EXTI inputs are falling-edge triggered to notify the microcontroller that a Current Sense (CS) output signal is within the specified accuracy range.
     - It is unclear if `SYNC` is necessary for our design yet. The datasheet mentions that the `SYNC` pin is mandatory for "autonomous operating mode".
   - **`CSB`, `SCLK`, `SI`, `SO` (SPI interface):**
     - | Net Name                | Pin     | Desciption               |
       | ----------------------- | --------| ------------------------ |
       | `CSB_AUX1_AUX2`         | `PC7`   | Chip select for auxiliary 1 and 2 |
       | `CSB_EM_PUMP`           | `PA9`   | Chip select for energy meter and cooling pump           |
       | `CSB_DI_L_DI_R`         | `PC4`   | Chip select for left/right driver inverter enable       |
       | `CSB_GLV_AIR_SHDN`      | `PC5`   | Chip select for CAN/low voltage system and AIR shutdown |
       | `SCK`                   | `PB13`  | Serial Clock        |
       | `MISO`                  | `PB14`  | Master in slave out |
       | `MOSI`                  | `PB15`  | Master out slave in |
     - These are standard pins for SPI interface, and each high-side switch has its own active-low chip select pin
     - The `CSB` pins are configured as push-pull outputs because the push-pull actively drives the `CSB` pin high which is faster than open-drain and thus reduces the likelihood of timing issues
     - The `CSB` pins are active low so we set the default state to high to prevent communicating with SPI slaves by accident
     - "Master" would be the microcontroller that is controlling 4 "slaves" or high-side switches
1. **`PC1`, `PC2`, `PC3` (Voltage sense):**
     - | Net Name     | Pin     | Desciption               |
       | ------------ | --------| ------------------------ |
       | VBAT_SENSE     | `PC1` | 18650s batteries                         |
       | +24V_AUX_SENSE | `PC2` | Auxiliary 24V from boost converter       |
       | +24V_ACC_SENSE | `PC3` | Auxiliary 24V from accumulator and Vicor |
     - These are voltage sense pins for various voltage rails and must be configured as ADC pins.

## Testing Plan
### Validate CAN logging for current sense
- Start a log trace in PCAN-Explorer 6
- For each output, disconnect the load so it's **0A** for 5 seconds. Then, connect a **4Ohm/50W** resistor for another 5 seconds.
- Repeat the step above for each GLV output
- Save the log trace and plot it in a third-party software (e.g. Microsoft Excel) to verify that the values make sense within a reasonable amount of error margin:
  - The current values are either 0A or 5A
  - There is no garbage value at any point in time (i.e. Non-sensical values such as **0.5e-30A** or **130.0**)
- The CAN messages of interest are:
  - `PDM_AUX1_AUX2_CURRENT`
  - `PDM_COOLING_PDMFAN_CURRENT`
  - `PDM_AIRSHDN_CANGLV_CURRENT`
  - `PDM_ACCUMULATOR_FAN_CURRENT`
  - `PDM_INVERTER_IO_CURRENT`

### Validate CAN logging for voltage sense
- Start a log trace in **PCAN-Explorer 6**
- Log the voltage sense values for at least 5 seconds
- Save the log trace and plot it in a third-party software (e.g. Microsoft Excel) to verify that the values make sense within a reasonable amount of error margin:
  - VBAT voltage is **8.4V** and GLV voltage **12V**
  - Flywire voltage is **12V** if **12V_ACC** is connceted or **0V** if **12V_ACC** is disconnceted
  - There is no garbage value at any point in time (i.e. Non-sensical values such as **5V** or **100V**)
- Note that you can simulate **12V_ACC** by using a external power supply instead of turning on high voltage and using the Vicor output
- The CAN messages of interest are:
  - `PDM_12V_VBAT`
  - `PDM_FLYWIRE`

