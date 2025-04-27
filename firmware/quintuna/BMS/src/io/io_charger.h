#pragma once

/**
 * call once at initialization to ensure state B (not ready to accept charge)
 */
void io_charger_init(void);

/**
 * vehicle inserted (pwm detected), waiting for EVSE to enable (kinda redundant to io_charger_init but different use
 * case)
 */
void io_charger_vehiclePresent(void);

/**
 * ask EVSE to start charging (high -> off, low -> on)
 */
void io_charger_requestCharge(void);

/**
 * hw interrupt to update PWM values
 */
void io_charger_inputCaptureCallback(TIM_HandleTypeDef *htim);