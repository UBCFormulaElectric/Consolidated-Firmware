#include "FSM.pb.h"
#include "hw_gpios.h"
#include "hw_adcs.h"
#include "hw_uarts.h"

const Gpio *const id_to_gpio[] = { [FSM_GpioNetName_BRAKE_OCSC_OK_3V3]       = &brake_ocsc_ok_3v3,
                                   [FSM_GpioNetName_NCHIMERA]                = &n_chimera_pin,
                                   [FSM_GpioNetName_LED]                     = &led_gpio_pin,
                                   [FSM_GpioNetName_NBSPD_BRAKE_PRESSED_3V3] = &nbspd_brake_pressed_3v3,
                                   [FSM_GpioNetName_NPROGRAM_3V3]            = &nprogram_3v3,
                                   [FSM_GpioNetName_FSM_SHDN]                = &fsm_shdn };

const AdcChannel *id_to_adc[] = {
    [FSM_AdcNetName_SUSP_TRAVEL_FR_3V3] = &susp_travel_fr,
    [FSM_AdcNetName_SUSP_TRAVEL_FL_3V3] = &susp_travel_fl,
    [FSM_AdcNetName_LOAD_CELL_2_3V3]    = &lc2,
    [FSM_AdcNetName_APPS2_3V3]          = &apps2,
    [FSM_AdcNetName_BPS_F_3V3]          = &bps_f,
    [FSM_AdcNetName_BPS_B_3V3]          = &bps_b,
    [FSM_AdcNetName_LOAD_CELL_1_3V3]    = &lc1,
    [FSM_AdcNetName_APPS1_3V3]          = &apps1,
    [FSM_AdcNetName_SteeringAngle_3V3]  = &steering_angle,
};

const Gpio *n_chimera_gpio = &n_chimera_pin;
