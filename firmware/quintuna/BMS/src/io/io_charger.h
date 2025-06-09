#pragma once

typedef enum
{
    WALL_CONNECTED,
    EVSE_CONNECTED,
    DISCONNECTED
} ConnectionStatus;

/**
 * get the status of the EVSE CP PWM
 */
ConnectionStatus io_charger_getConnectionStatus();

/**
 * @return The duty cycle of the EVSE CP PWM signal, in the range [0.0, 1.0].
 */
float io_charger_getDutyCycle();