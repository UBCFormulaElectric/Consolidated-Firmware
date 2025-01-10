/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "app_canRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by  (i.e. the RX table).
 */
typedef struct
{
} BMS_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static BMS_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void app_canRx_init()
{
    memset(&rx_table, 0, sizeof(BMS_RxMsgs));
    // Initialize all signals to Start-up value
    
}

void app_canRx_BRUSA_IsConnected_update(bool value)
{
    rx_table.BRUSA_Vitals_signals.BRUSA_IsConnected_value = value;
}






