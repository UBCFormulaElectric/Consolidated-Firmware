#pragma once

#include <stm32f3xx_hal.h>
#include <stdbool.h>

/**
 * Initializes a given timer handle for a flow meter
 * @param htim : The timer handle for both flow meters
 */
void Io_FlowMeter_Init(TIM_HandleTypeDef *htim);
/**
 * @returns the flow rate (L/min) from the primary flow meter
 */
float Io_FlowMeters_GetFlowRate(void);
/**
 * Update the frequency for the primary and secondary flow meter's PWM outputs.
 * @note This function should be called in the input capture callback function.
 * @param htim: The handle of the timer that triggered the input capture
 *              callback function
 */
void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim);
/**
 * Check if the PWM signal of the primary flow meter is active. If it is
 * inactive (i.e. It has been unplugged or unpowered), set the frequency to NaN.
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the primary flow meter.
 */
void Io_FlowMeters_CheckIfFlowMeterIsActive(void);

/**
 * @return the voltage of the temperature A sensor
 */
float Io_Coolant_TemperatureAVoltage(void);
/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float Io_Coolant_GetTemperatureA(void);
/**
 * @return whether the temperature sensor A is open or short circuit
 */
bool Io_Coolant_TemperatureSensorA_OCSC(void);

/**
 * @return the voltage of the temperature B sensor
 */
float Io_Coolant_TemperatureBVoltage(void);
/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float Io_Coolant_GetTemperatureB(void);
/**
 * @return whether the temperature sensor B is open or short circuit
 */
bool Io_Coolant_TemperatureSensorB_OCSC(void);

/**
 * @return the voltage of the pressure A sensor
 */
float Io_Coolant_PressureAVoltage(void);
/**
 * @returns the pressure of the coolant at [sensor A] in PSI
 */
float Io_Coolant_GetPressureA(void);
/**
 * @return whether pressure sensor A is open or short circuit
 */
bool Io_Coolant_PressureSensorA_OCSC(void);

/**
 * @return the voltage of the pressure B sensor
 */
float Io_Coolant_PressureBVoltage(void);
/**
 * @returns the pressure of the coolant at [sensor B] in PSI
 */
float Io_Coolant_GetPressureB(void);
/**
 * @return whether pressure sensor B is open or short circuit
 */
bool Io_Coolant_PressureSensorB_OCSC(void);
