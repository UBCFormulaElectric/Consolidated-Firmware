#include <stdbool.h>

/**
 * Enumerates the overall LED statuses a board can have.
 */
typedef enum
{
    BOARD_LED_STATUS_OK = 0,
    BOARD_LED_STATUS_WARNING,
    BOARD_LED_STATUS_FAULT,
    BOARD_LED_STATUS_NOT_IMPLEMENTED,
    BOARD_LED_STATUS_MISSING_HEARTBEAT
} BoardLEDStatus;

/**
 * Update all LEDs based on current CAN data and shift them out.
 */
void app_leds_update(void);
