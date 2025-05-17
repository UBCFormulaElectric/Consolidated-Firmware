#include "app_screens.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include "io_shift_register.h"

/*********************** Function Definitions ***************************/
static void init_update(void)
{
    uint8_t      data_buffer[SEVEN_SEG_DATA_LENGTH];
    ShutdownNode shutdownNode = app_canRx_VC_FirstFaultNode_get();

    for (uint8_t i = 0; i < 8; i++)
    {
        data_buffer[i] = SEGMENT_DICT[DISP_DP];
    }

    switch (shutdownNode)
    {
        case SHDN_BSPD_OK:
            data_buffer[0] = SEGMENT_DICT[DISP_B];
            data_buffer[1] = SEGMENT_DICT[DISP_S];
            data_buffer[2] = SEGMENT_DICT[DISP_P];
            data_buffer[3] = SEGMENT_DICT[DISP_D];
            break;
        case SHDN_BMS_OK:
            data_buffer[0] = SEGMENT_DICT[DISP_B];
            data_buffer[1] = SEGMENT_DICT[DISP_M];
            data_buffer[2] = SEGMENT_DICT[DISP_S];
            break;
        case SHDN_IMD_OK:
            data_buffer[0] = SEGMENT_DICT[DISP_I];
            data_buffer[1] = SEGMENT_DICT[DISP_M];
            data_buffer[2] = SEGMENT_DICT[DISP_D];
            break;
        case SHDN_TS_Ilck:
            data_buffer[0] = SEGMENT_DICT[DISP_T];
            data_buffer[1] = SEGMENT_DICT[DISP_S];
            break;
        case SHDN_Cockpit_EStop:
            data_buffer[0] = SEGMENT_DICT[DISP_C];
            data_buffer[1] = SEGMENT_DICT[DISP_DP];
            data_buffer[2] = SEGMENT_DICT[DISP_E];
            data_buffer[3] = SEGMENT_DICT[DISP_S];
            data_buffer[4] = SEGMENT_DICT[DISP_T];
            data_buffer[5] = SEGMENT_DICT[DISP_O];
            data_buffer[6] = SEGMENT_DICT[DISP_P];
            break;
        case SHDN_Inertia:
            data_buffer[0] = SEGMENT_DICT[DISP_I];
            data_buffer[1] = SEGMENT_DICT[DISP_N];
            data_buffer[2] = SEGMENT_DICT[DISP_E];
            data_buffer[3] = SEGMENT_DICT[DISP_R];
            data_buffer[4] = SEGMENT_DICT[DISP_T];
            data_buffer[5] = SEGMENT_DICT[DISP_I];
            data_buffer[6] = SEGMENT_DICT[DISP_A];
            break;
        case SHDN_BOTS:
            data_buffer[0] = SEGMENT_DICT[DISP_B];
            data_buffer[1] = SEGMENT_DICT[DISP_O];
            data_buffer[2] = SEGMENT_DICT[DISP_T];
            break;
        case SHDN_L_EStop:
            data_buffer[0] = SEGMENT_DICT[DISP_L];
            data_buffer[1] = SEGMENT_DICT[DISP_DP];
            data_buffer[2] = SEGMENT_DICT[DISP_E];
            data_buffer[3] = SEGMENT_DICT[DISP_S];
            data_buffer[4] = SEGMENT_DICT[DISP_T];
            data_buffer[5] = SEGMENT_DICT[DISP_O];
            data_buffer[6] = SEGMENT_DICT[DISP_P];
            break;
        case SHDN_R_EStop:
            data_buffer[0] = SEGMENT_DICT[DISP_R];
            data_buffer[1] = SEGMENT_DICT[DISP_DP];
            data_buffer[2] = SEGMENT_DICT[DISP_E];
            data_buffer[3] = SEGMENT_DICT[DISP_S];
            data_buffer[4] = SEGMENT_DICT[DISP_T];
            data_buffer[5] = SEGMENT_DICT[DISP_O];
            data_buffer[6] = SEGMENT_DICT[DISP_P];
            break;
        case SHDN_TSMS:
            data_buffer[0] = SEGMENT_DICT[DISP_T];
            data_buffer[1] = SEGMENT_DICT[DISP_S];
            data_buffer[2] = SEGMENT_DICT[DISP_M];
            data_buffer[3] = SEGMENT_DICT[DISP_S];
            break;
        case SHDN_SB_ILCK:
            data_buffer[0] = SEGMENT_DICT[DISP_S];
            data_buffer[1] = SEGMENT_DICT[DISP_B];
            data_buffer[2] = SEGMENT_DICT[DISP_DP];
            data_buffer[3] = SEGMENT_DICT[DISP_I];
            data_buffer[4] = SEGMENT_DICT[DISP_L];
            data_buffer[5] = SEGMENT_DICT[DISP_K];
            break;
        case SHDN_HVD_ILCK:
            data_buffer[0] = SEGMENT_DICT[DISP_H];
            data_buffer[1] = SEGMENT_DICT[DISP_V];
            data_buffer[2] = SEGMENT_DICT[DISP_D];
            data_buffer[3] = SEGMENT_DICT[DISP_DP];
            data_buffer[4] = SEGMENT_DICT[DISP_I];
            data_buffer[5] = SEGMENT_DICT[DISP_L];
            data_buffer[6] = SEGMENT_DICT[DISP_K];
            break;
        case SHDN_OK:
            data_buffer[0] = SEGMENT_DICT[DISP_O];
            data_buffer[1] = SEGMENT_DICT[DISP_K];
            break;
        default:
            break;
    }

    io_shift_register_updateSevenSegRegisters((uint8_t *)data_buffer);
}

Screen init_screen = { .ccw_callback = NULL, .cw_callback = NULL, .update = init_update };
