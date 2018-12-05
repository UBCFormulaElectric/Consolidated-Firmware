## Overview
## Peripheral Configuration Notes
1. **`Auto-Reload Preload` is set to **Disabled** for every timer**
   - *The content of the preload register are transferred into the shadow register permanently or at each update event (UEV), depending on the auto-reload preload enable bit (ARPE). (Pg 322 in RM0091.pdf)*
   - It only needs to be **Enabled** if we are adjusting timer frequency on-the-fly, which we are not.
2. **`End of Conversion Selection` for ADC**
   - This determines whether an ADC interrupt is generated at the end of conversion or end of seqeuence
   - We don't use ADC interrupt (disabled in NVIC controller) so this selection is inconsequential
3. **ADC Sampling Time = 71.5 Cycles**
   - **VREFINT** requires a minimum sampling time of 4us, and the steering angle sensor has a minimum sampling time of about 0us (because it's buffered by an op-amp). On this F0 chip, you can only select one global sampling time for every ADC channel rather than having a custom sampling time for each ADC channel. So we must go with the largest of all minimum sampling time requirements = 4us, which translates to 4us / ( 1 / 14Mhz ) = 56 clock cycles, where 14Mhz is the ADC clock frequency. We choose the next closest option: 71.5 cycles.
4. **`Continuous Conversion Mode` in ADC**
   - *In **continuous conversion mode**, when a software or hardware trigger event occurs, the ADC performs a sequence of conversions, converting all the channels once and then automatically re-starts and continuously performs the same sequence of conversions. (Pg. 236 in RM0091.pdf)*
   - We don't require continuous conversion mode because we have TIM1 to trigger ADC conversions periodically
5. **`DMA Continuous Requests` in ADC**
   - *In **circular** mode, the ADC generates a DMA transfer request each time a new conversion data word is available in the data register, even if the DMA has reached the last DMA transfer. This allows the DMA to be configured in circular mode to handle a continuous analog input data stream. (Pg. 246 in RM0091.pdf)*
   - We use continuous mode because it is for handling a continous analog input data stream like the steering angle sensor measurements.
6. **`DMA Circular Mode` in DMA**
   - ***Circular** mode is available to handle circular buffers and continuous data flows (e.g. ADC scan mode). When circular mode is activated, the number of data to be transferred is automatically reloaded with the initial value programmed during the channel configuration phase, and the DMA requests continue to be served. (Pg. 191 in RM0091.pdf)*
   - Similar to `DMA Continuous Requests` in ADC, `DMA Circular Mode` is the appropriate mode for a continuous analog input data stream like the steering angle sensor measurements.
7. **`Counter Period = 65535` in TIM16/17 for Input Capture Mode**
   - This value is inconsequential because input capture doesn't use the timer overflow interrupt, however we should leave it at max value to minimize the number of timer overflows because the wheel speed calculation for a timer overflow is slightly more assembly instructions (See if-else clause below)
```
	// Compute difference between the two timer values
	if(CurrentTimerValue > PreviousTimerValue)
	{
		DeltaTimerValue = CurrentTimerValue - PreviousTimerValue;
	}
	else
	{
		// Handle timer overflow
		DeltaTimerValue = (0x10000 - PreviousTimerValue) + CurrentTimerValue;
	}
```
8. **Pull-up resistors for PA3 and PB7**
   - PA3 and PB7 are connected to the ALARMD outputs from MAX3097E. We should configure PA3 and PB7 with pull-up resistor so in the case that the pins are left floating or the MAX3097E is damaged, we will be able to detect it.
   - <img src="https://user-images.githubusercontent.com/16970019/48681210-e74b0100-eb55-11e8-9bf1-aec4597869e1.png" width="75%" height="75%">
9. **No need for pull-up resistors for PA6 and PA7**
   - PA6 and PA7 don't need pull-up or down because there's an external pull-up resistor already.
   - <img src="https://user-images.githubusercontent.com/16970019/48681098-39d7ed80-eb55-11e8-8037-639342a3ab7d.png" width="40%" height="40%">
10. **Pull-up resistors for PA0, PA1, PB4, PB5**
    - These are encoder inputs expecting square pulses. Encoder inputs technically don't require either a pull-up or pull-down. And having a pull-up doesn't actually help us detect hardware faults. However, we will default to using pull-up just so the input has a defined state instead of floating state.
