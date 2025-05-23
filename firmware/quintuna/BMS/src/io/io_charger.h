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
 * hw interrupt to update PWM values
 */
void io_charger_inputCaptureCallback(TIM_HandleTypeDef *htim);