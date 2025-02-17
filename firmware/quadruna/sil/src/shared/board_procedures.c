#include "sil_api.h"
#include "board_procedures.h"
#include "io_log.h"

void set_pedal_percentage_50(void)
{
    _LOG_PRINTF("pedal is at 50 percent\n");
}

void turn_BMS_light_on(void)
{
    _LOG_PRINTF("lights are on!\n");
}

sil_api_Procedure VC_functions[] = { { "set_pedal_percentage_50", set_pedal_percentage_50 } };

sil_api_Procedure BMS_functions[] = { { "turn_BMS_light_on", turn_BMS_light_on } };

const BoardFunctionMapping board_tables[] = { { "VC", VC_functions }, { "BMS", BMS_functions }, { NULL, NULL } };
