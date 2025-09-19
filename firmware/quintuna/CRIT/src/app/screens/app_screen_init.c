#include "app_screens.h"
#include "app_screen_defines.h"
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
        data_buffer[i] = SEG_PATTERN_DP;
    }

    switch (shutdownNode)
    {
        case SHDN_BSPD_OK:
            data_buffer[0] = SEG_PATTERN_B;
            data_buffer[1] = SEG_PATTERN_S;
            data_buffer[2] = SEG_PATTERN_P;
            data_buffer[3] = SEG_PATTERN_D;
            break;
        case SHDN_BMS_OK:
            data_buffer[0] = SEG_PATTERN_B;
            data_buffer[1] = SEG_PATTERN_M;
            data_buffer[2] = SEG_PATTERN_S;
            break;
        case SHDN_IMD_OK:
            data_buffer[0] = SEG_PATTERN_I;
            data_buffer[1] = SEG_PATTERN_M;
            data_buffer[2] = SEG_PATTERN_D;
            break;
        case SHDN_TSMS:
            data_buffer[0] = SEG_PATTERN_T;
            data_buffer[1] = SEG_PATTERN_S;
            data_buffer[2] = SEG_PATTERN_M;
            data_buffer[3] = SEG_PATTERN_S;
            break;
        case SHDN_Cockpit_EStop:
            data_buffer[0] = SEG_PATTERN_C;
            data_buffer[1] = SEG_PATTERN_DP;
            data_buffer[2] = SEG_PATTERN_E;
            data_buffer[3] = SEG_PATTERN_S;
            data_buffer[4] = SEG_PATTERN_T;
            data_buffer[5] = SEG_PATTERN_O;
            data_buffer[6] = SEG_PATTERN_P;
            break;
        case SHDN_FL_INERTIA_ILCK:
            data_buffer[0] = SEG_PATTERN_F;
            data_buffer[1] = SEG_PATTERN_L;
            data_buffer[2] = SEG_PATTERN_DP;
            data_buffer[3] = SEG_PATTERN_I;
            data_buffer[4] = SEG_PATTERN_N;
            data_buffer[5] = SEG_PATTERN_E;
            data_buffer[6] = SEG_PATTERN_R;
            break;
        case SHDN_BOTS:
            data_buffer[0] = SEG_PATTERN_B;
            data_buffer[1] = SEG_PATTERN_O;
            data_buffer[2] = SEG_PATTERN_T;
            break;
        case SHDN_L_EStop:
            data_buffer[0] = SEG_PATTERN_L;
            data_buffer[1] = SEG_PATTERN_DP;
            data_buffer[2] = SEG_PATTERN_E;
            data_buffer[3] = SEG_PATTERN_S;
            data_buffer[4] = SEG_PATTERN_T;
            data_buffer[5] = SEG_PATTERN_O;
            data_buffer[6] = SEG_PATTERN_P;
            break;
        case SHDN_R_EStop:
            data_buffer[0] = SEG_PATTERN_R;
            data_buffer[1] = SEG_PATTERN_DP;
            data_buffer[2] = SEG_PATTERN_E;
            data_buffer[3] = SEG_PATTERN_S;
            data_buffer[4] = SEG_PATTERN_T;
            data_buffer[5] = SEG_PATTERN_O;
            data_buffer[6] = SEG_PATTERN_P;
            break;
        case SHDN_MSD_EMETER_ILCK:
            data_buffer[0] = SEG_PATTERN_M;
            data_buffer[1] = SEG_PATTERN_S;
            data_buffer[2] = SEG_PATTERN_D;
            break;
        case SHDN_FR_ILCK:
            data_buffer[0] = SEG_PATTERN_F;
            data_buffer[1] = SEG_PATTERN_R;
            data_buffer[2] = SEG_PATTERN_DP;
            data_buffer[3] = SEG_PATTERN_I;
            data_buffer[4] = SEG_PATTERN_L;
            data_buffer[5] = SEG_PATTERN_K;
            break;
        case SHDN_RL_ILCK:
            data_buffer[0] = SEG_PATTERN_R;
            data_buffer[1] = SEG_PATTERN_L;
            data_buffer[2] = SEG_PATTERN_DP;
            data_buffer[3] = SEG_PATTERN_I;
            data_buffer[4] = SEG_PATTERN_L;
            data_buffer[5] = SEG_PATTERN_K;
            break;
        case SHDN_RR_ILCK:
            data_buffer[0] = SEG_PATTERN_R;
            data_buffer[1] = SEG_PATTERN_R;
            data_buffer[2] = SEG_PATTERN_DP;
            data_buffer[3] = SEG_PATTERN_I;
            data_buffer[4] = SEG_PATTERN_L;
            data_buffer[5] = SEG_PATTERN_K;
            break;
        case SHDN_HV_P_ILCK:
            data_buffer[0] = SEG_PATTERN_H;
            data_buffer[1] = SEG_PATTERN_V;
            data_buffer[2] = SEG_PATTERN_D;
            data_buffer[3] = SEG_PATTERN_P;
            data_buffer[4] = SEG_PATTERN_I;
            data_buffer[5] = SEG_PATTERN_L;
            data_buffer[6] = SEG_PATTERN_K;
            break;
        case SHDN_HV_N_ILCK:
            data_buffer[0] = SEG_PATTERN_H;
            data_buffer[1] = SEG_PATTERN_V;
            data_buffer[2] = SEG_PATTERN_D;
            data_buffer[3] = SEG_PATTERN_N;
            data_buffer[4] = SEG_PATTERN_I;
            data_buffer[5] = SEG_PATTERN_L;
            data_buffer[6] = SEG_PATTERN_K;
            break;
        case SHDN_OK:
            data_buffer[0] = SEG_PATTERN_O;
            data_buffer[1] = SEG_PATTERN_K;
            break;
        default:
            // leave all segments blank
            break;
    }
    io_shift_register_updateSevenSegRegisters((uint8_t *)data_buffer);
}

Screen init_screen = { .ccw_callback = NULL, .cw_callback = NULL, .update = init_update };
