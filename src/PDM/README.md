## Overview
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

