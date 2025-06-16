#pragma once

#include "app_canUtils.h"

/**
 * get the status of the EVSE CP PWM
 */
ChargerConnectedType io_charger_getConnectionStatus();

/**
 * @return The duty cycle of the EVSE CP PWM signal, in the range [0.0, 1.0].
 */
float io_charger_getCPDutyCycle();