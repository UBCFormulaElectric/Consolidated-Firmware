## Overview
## Peripheral Configuration Notes
1. **`PA4(START_STOP_SWITCH)`, `PA5(TRACTION_CONTROL_SWITCH)`, and `PA6(TORQUE_VECTORING_SWITCH)` use pull-up resistors**
   - These inputs are for active-low switches and we should make the default state to be inactive using pull-up resistors.
1. **`PA8(STATUS_R)`, `PA9(STATUS_G)`, and `PA10(STATUS_B)` don't use pull-up/down resistors**
   - The RGB LED is powered by 5V instead of the 3.3V used by the microcontroller. We would have used pull-up resistor if the RGB LED is powered by 3.3V.
1. **`PB0(START_STOP_LED)`, `PB1(TRACTION_CONTROL_LED)`, and `PB2(TORQUE_VECTORING_LED)` are push-pull outputs**
   - These LED outputs are current limited by PTC thermistors RT11, RT12, and RT13. And that's why we can use push-pull
1. **`PB4(BUZZER_EN)` and `PB5(BRAKE_LIGHT_EN)` are push-pull outputs**
   - These outputs drive a dual-channel MOSFET driver and there are series resistors in place to limit current. And that's why we can use push-pull.
1. **`PB8(INVERTER_R_REGEN)`, `PB9(INVERTER_L_REGEN)`, `PB10(INVERTER_L_RUN)`, and `PB11(INVERTER_R_RUN)` are push-pull outputs**
   - These outputs drive optoisolators and there are series resistors in place to limit current. And that's why we can use puhs-pull outputs.
1. **`PA15(BSPD_BRAKE_THRES)` uses pull-up resistor**
   - The BSPD brake threshold is active high, that is high means the brake pedal is pressed. We want the default state to be when the pedal is pressed for safety.
1. **`PB14(IMU_INT)` is EXTI input with rising edge trigger**
   - `IMU_INT`comes from LSM6DS33 and the default interrupt edge trigger is active high (See: CTRL3_C register)  
